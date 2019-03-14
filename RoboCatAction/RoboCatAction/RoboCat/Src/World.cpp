#include <RoboCatPCH.h>

#include <ctime>



std::unique_ptr< World >	World::sInstance;

void World::StaticInit()
{
	sInstance.reset( new World() );
}

World::World()
{
}


void World::AddGameObject( GameObjectPtr inGameObject )
{
	mGameObjects.push_back( inGameObject );
	inGameObject->SetIndexInWorld( mGameObjects.size() - 1 );
}


void World::RemoveGameObject( GameObjectPtr inGameObject )
{
	int index = inGameObject->GetIndexInWorld();

	int lastIndex = mGameObjects.size() - 1;
	if( index != lastIndex )
	{
		mGameObjects[ index ] = mGameObjects[ lastIndex ];
		mGameObjects[ index ]->SetIndexInWorld( index );
	}

	inGameObject->SetIndexInWorld( -1 );

	mGameObjects.pop_back();
}


void World::Update()
{
	//std::time_t now = std::time(0);
	//char* dt = std::ctime(&now);
	//string s = "World::Update() at: ";
	//s.append(string(dt));
	//s.append("\n");
	//OutputDebugString(s.c_str());// " + *dt + '\n');
	//update all game objects- sometimes they want to die, so we need to tread carefully...

	for( int i = 0, c = mGameObjects.size(); i < c; ++i )
	{
		GameObjectPtr go = mGameObjects[ i ];
		

		if( !go->DoesWantToDie() )
		{
			go->Update();
		}
		//you might suddenly want to die after your update, so check again
		if( go->DoesWantToDie() )
		{
			RemoveGameObject( go );
			go->HandleDying();
			--i;
			--c;
		}
	}
}