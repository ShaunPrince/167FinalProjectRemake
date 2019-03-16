#include <RoboCatServerPCH.h>


YarnServer::YarnServer()
{
	//yarn lives 14 seconds...
	mTimeToDie = Timing::sInstance.GetFrameStartTime() + 14.f;
}

void YarnServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}


void YarnServer::Update()
{
	Yarn::Update();

	if( Timing::sInstance.GetFrameStartTime() > mTimeToDie )
	{
		SetDoesWantToDie( true );
	}

}

bool YarnServer::HandleCollisionWithCat( RoboCat* inCat )
{
	if( inCat->GetPlayerId() != GetPlayerId() )
	{
		//kill yourself!
		SetDoesWantToDie( true );

		static_cast< RoboCatServer* >( inCat )->TakeDamage( GetPlayerId() );

	}

	return false;
}


