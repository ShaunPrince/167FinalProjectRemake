#include <RoboCatClientPCH.h>
#include <cstring>



RoboCatClient::RoboCatClient() :
	mTimeLocationBecameOutOfSync( 0.f ),
	mTimeVelocityBecameOutOfSync( 0.f )
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
	MoveList& moveList = InputManager::sInstance->GetMoveList();
	for (const Move& unprocessedMove : moveList)
	{
		const InputState& currentState = unprocessedMove.GetInputState();
		float deltaTime = unprocessedMove.GetDeltaTime();
		this->ProcessInput(deltaTime, currentState);
		this->SimulateMovement(deltaTime);
	}

	//moveList.Clear();
	//for now, we don't simulate any movement on the client side
	//we only move when the server tells us to move
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

	float oldRotation = GetRotation();
	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();

	float replicatedRotation;
	Vector3 replicatedLocation;
	Vector3 replicatedVelocity;

	inInputStream.Read( stateBit );
	if( stateBit )
	{
		MoveList& moveList = InputManager::sInstance->GetMoveList();
		float deltaTime;
		//if(moveList.GetLatestMove() != nullptr)
		if (moveList.HasMoves())
		{
			deltaTime = moveList.GetLatestMove().GetDeltaTime();
		}
		else
		{
			deltaTime = 0;
		}


		inInputStream.Read( replicatedVelocity.mX );
		inInputStream.Read( replicatedVelocity.mY );

		//for (const Move& unprocessedMove : moveList)
		//{
		//	float deltaTime = unprocessedMove.GetDeltaTime();
		//	replicatedVelocity = oldVelocity + deltaTime * (replicatedVelocity - oldVelocity);
		//	break;
		//}


		SetVelocity( replicatedVelocity );

		inInputStream.Read( replicatedLocation.mX );
		inInputStream.Read( replicatedLocation.mY );
		
		OutputDebugString((((std::to_string(oldLocation.mX) + ':').c_str())));
		OutputDebugString((((std::to_string(oldLocation.mY)+ '\n').c_str()  )));
		OutputDebugString((((std::to_string(replicatedLocation.mX) + ':').c_str() )));
		OutputDebugString((((std::to_string(replicatedLocation.mY) + '\n').c_str())));
		

		//attempt at interpolation
		for (const Move& unprocessedMove : moveList)
		{
			float deltaTime = unprocessedMove.GetDeltaTime();
			oldLocation = oldLocation + deltaTime * (replicatedLocation - oldLocation);
			SetLocation(oldLocation);
			RenderManager::sInstance->Render();
		}

		//SetLocation(replicatedLocation);
		



		inInputStream.Read( replicatedRotation );
		//for (const Move& unprocessedMove : moveList)
		//{
		//	float deltaTime = unprocessedMove.GetDeltaTime();
		//	replicatedRotation = oldRotation + deltaTime * (replicatedRotation - oldRotation);
		//	break;
		//}

		SetRotation( replicatedRotation );

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
