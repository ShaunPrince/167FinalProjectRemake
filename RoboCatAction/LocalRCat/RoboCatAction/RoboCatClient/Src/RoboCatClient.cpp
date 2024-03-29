#include <RoboCatClientPCH.h>



RoboCatClient::RoboCatClient() :
	mTimeLocationBecameOutOfSync( 0.f ),
	mTimeVelocityBecameOutOfSync( 0.f ),
	timeElapsedLocal(0.f)
{
	mSpriteComponent.reset( new SpriteComponent( this ) );
	mSpriteComponent->SetTexture( TextureManager::sInstance->GetTexture( "cat" ) );
}

void RoboCatClient::HandleDying()
{
	RoboCat::HandleDying();

	//and if we're local, tell the hud so our health goes away!
	if( GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId() )
	{
		HUD::sInstance->SetPlayerHealth( 0 );
	}
}


void RoboCatClient::Update()
{
	timeElapsedLocal += Timing::sInstance.GetDeltaTime();

	//for now, we don't simulate any movement on the client side
	//we only move when the server tells us to move
	float timeStep = .2f;


	if (timeElapsedLocal >= deltaUpdateTime)
	{
		SetRotation(newNewRotation);
		SetVelocity(newNewVelocity);
		SetLocation(newNewLocation);
		oldLocation = newNewLocation;
		oldRotation = newNewRotation;
		oldVelocity = newNewVelocity;
		timeElapsedLocal -= deltaUpdateTime;
	}
	else
	{
		SetRotation(oldRotation + ((newNewRotation - oldRotation) * (timeElapsedLocal / deltaUpdateTime  )));
		SetVelocity(oldVelocity + ((newNewVelocity - oldVelocity) * (timeElapsedLocal / deltaUpdateTime )));
		SetLocation(oldLocation + ((newNewLocation - oldLocation) * (timeElapsedLocal / deltaUpdateTime  )));
	}


}

void RoboCatClient::Read( InputMemoryBitStream& inInputStream )
{
	bool stateBit;
	
	uint32_t readState = 0;

	inInputStream.Read( stateBit );
	if( stateBit )
	{
		uint32_t playerId;
		inInputStream.Read( playerId );
		SetPlayerId( playerId );
		readState |= ECRS_PlayerId;
	}

	oldRotation = GetRotation();
	oldLocation = GetLocation();
	oldVelocity = GetVelocity();

	float replicatedRotation;
	Vector3 replicatedLocation;
	Vector3 replicatedVelocity;

	if (timeOfUpdateFirst == 0)
	{
		if (timeOfUpdateSecond == 0)
		{
			timeOfUpdateSecond = Timing::sInstance.GetTimef();
			deltaUpdateTime = 0;
		}
		else
		{
			timeOfUpdateFirst = Timing::sInstance.GetTimef();
			deltaUpdateTime = 0;
		}
	}
	else
	{
		deltaUpdateTime = timeOfUpdateSecond - timeOfUpdateFirst;
		timeOfUpdateFirst = timeOfUpdateSecond;
		timeOfUpdateSecond = Timing::sInstance.GetTimef();
	}

	

	inInputStream.Read( stateBit );
	if( stateBit )
	{
		inInputStream.Read( replicatedVelocity.mX );
		inInputStream.Read( replicatedVelocity.mY );
		newNewVelocity = replicatedVelocity;
		//SetVelocity( replicatedVelocity );

		inInputStream.Read( replicatedLocation.mX );
		inInputStream.Read( replicatedLocation.mY );

		newNewLocation = replicatedLocation;
		//SetLocation( replicatedLocation );


		inInputStream.Read( replicatedRotation );
		newNewRotation = replicatedRotation;
		//SetRotation( replicatedRotation );

		readState |= ECRS_Pose;
	}

	inInputStream.Read( stateBit );
	if( stateBit )
	{
		inInputStream.Read( stateBit );
		mThrustDir = stateBit ? 1.f : -1.f;
	}
	else
	{
		mThrustDir = 0.f;
	}

	inInputStream.Read( stateBit );
	if( stateBit )
	{
		Vector3 color;
		inInputStream.Read( color );
		SetColor( color );
		readState |= ECRS_Color;
	}

	inInputStream.Read( stateBit );
	if( stateBit )
	{
		mHealth = 0;
		inInputStream.Read( mHealth, 4 );
		readState |= ECRS_Health;
	}

	if( GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId() )
	{
		//did we get health? if so, tell the hud!
		if( ( readState & ECRS_Health ) != 0 )
		{
			HUD::sInstance->SetPlayerHealth( mHealth );
		}
	}	
}
