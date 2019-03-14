#include <RoboCatServerPCH.h>

namespace
{
	const float kRespawnDelay = 3.f;
}

ClientProxy::ClientProxy( const SocketAddress& inSocketAddress, const string& inName, int inPlayerId ) :
mSocketAddress( inSocketAddress ),
mName( inName ),
mPlayerId( inPlayerId ),
mIsLastMoveTimestampDirty( false ),
mTimeToRespawn( 0.f )
{
	UpdateLastPacketTime();
}


void ClientProxy::UpdateLastPacketTime()
{
	mLastPacketFromClientTime = Timing::sInstance.GetTimef(); 
}

void	ClientProxy::HandleCatDied()
{
	mTimeToRespawn = Timing::sInstance.FrameToTime(Timing::sInstance.GetFixedSteps()) + kRespawnDelay;
}

void	ClientProxy::RespawnCatIfNecessary()
{
	if( mTimeToRespawn != 0.f && Timing::sInstance.GetFixedSteps() > Timing::sInstance.TimeToFrame(mTimeToRespawn) )
	{
		static_cast< Server* > ( Engine::sInstance.get() )->SpawnCatForPlayer( mPlayerId );
		mTimeToRespawn = 0.f;
	}
}
