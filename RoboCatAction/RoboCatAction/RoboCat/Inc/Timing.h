class Timing
{
public:

	Timing();

	int Update();

	float GetDeltaTime() const { return mDeltaTime; }

	double GetTime() const;

	float GetTimef() const
	{
		return static_cast<float>(GetTime());
	}

	float GetFrameStartTime() const { return mFrameStartTimef; }

	int GetFixedSteps() const { return mFixedSteps; }

	float GetFixedFrame() const { return kFixedFrameTime; }

	int TimeToFrame(float time) const
	{ 
		return static_cast<int>(time / kFixedFrameTime); 
	}

	float FrameToTime(int frame) const 
	{ 
		return static_cast<float>(frame * kFixedFrameTime);
	}


	static Timing sInstance;

private:
	float		mDeltaTime;
	uint64_t	mDeltaTick;

	double		mLastFrameStartTime;
	float		mFrameStartTimef;
	double		mPerfCountDuration;
	int         mFixedSteps;
	float       accumulator;
	float       kFixedFrameTime = 0.0166f;

};