#include <RoboCatPCH.h>

Mouse::Mouse()
{
	SetScale( GetScale() * 0.5f );
	SetCollisionRadius(0.25f);
	SetSpecial(false);
	int num = rand() % 100;
	if (num < 10)
	{
		Vector3 *color = new Vector3(255.f, 223.f, 0.f);
		SetColor(*color);
		SetSpecial(true);
	}
}


bool Mouse::HandleCollisionWithCat( RoboCat* inCat )
{
	( void ) inCat;
	return false;
}



uint32_t Mouse::Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const 
{
	uint32_t writtenState = 0;

	if( inDirtyState & EMRS_Pose )
	{
		inOutputStream.Write( (bool)true );

		Vector3 location = GetLocation();
		inOutputStream.Write( location.mX );
		inOutputStream.Write( location.mY );

		inOutputStream.Write( GetRotation() );

		writtenState |= EMRS_Pose;
	}
	else
	{
		inOutputStream.Write( (bool)false );
	}

	if( inDirtyState & EMRS_Color )
	{
		inOutputStream.Write( (bool)true );

		inOutputStream.Write( GetColor() );

		writtenState |= EMRS_Color;
	}
	else
	{
		inOutputStream.Write( (bool)false );
	}


	return writtenState;
}

void Mouse::Read( InputMemoryBitStream& inInputStream )
{
	bool stateBit;

	inInputStream.Read( stateBit );
	if( stateBit )
	{
		Vector3 location;
		inInputStream.Read( location.mX );
		inInputStream.Read( location.mY );
		SetLocation( location );

		float rotation;
		inInputStream.Read( rotation );
		SetRotation( rotation );
	}


	inInputStream.Read( stateBit );
	if( stateBit )
	{	
		Vector3 color;
		inInputStream.Read( color );
		SetColor( color );
	}
}

