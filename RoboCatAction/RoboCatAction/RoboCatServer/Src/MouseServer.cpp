#include <RoboCatServerPCH.h>


MouseServer::MouseServer()
{
}

void MouseServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}


bool MouseServer::HandleCollisionWithCat( RoboCat* inCat )
{
	//kill yourself!
	SetDoesWantToDie( true );

	if (IsSpecial())
		ScoreBoardManager::sInstance->IncScore( inCat->GetPlayerId(), 1000);
	else
		ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 100);

	return false;
}


