#include <RoboCatServerPCH.h>


YarnServer::YarnServer()
{
	//yarn lives a second...
	mTimeToDie = Timing::sInstance.FrameToTime(Timing::sInstance.GetFixedSteps()) + 1.f;
}

void YarnServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}


void YarnServer::Update()
{
	Yarn::Update();

	if( Timing::sInstance.GetFixedSteps() > Timing::sInstance.TimeToFrame(mTimeToDie) )
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


