class Client : public Engine
{
public:

	static bool StaticInit( );
	shared_ptr<RoboCatClient> mRoboCat;

protected:

	Client();

	virtual void	DoFrame() override;
	virtual void	HandleEvent( SDL_Event* inEvent ) override;

private:



};