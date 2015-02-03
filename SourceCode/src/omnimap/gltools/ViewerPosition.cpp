#include "../GraphicsPrimitives.h"
#include "../omnimap.h"

#include <windows.h>
#include <gl/glu.h>

Omni_ViewerPosition::Omni_ViewerPosition()
{
	pos		= OmniVec3(0,0,0);
	lookAtpos	= OmniVec3(0,0,0);
	headsUp	= OmniVec3(0,0,0);

}
/*
Omni_ViewerPosition::Omni_ViewerPosition(const OmniVec3 &_user_pos,const OmniVec3 &	_user_lookAtpos ,const OmniVec3 &_user_headsUp)
{
	pos		= _user_pos;
	lookAtpos  = _user_lookAtpos;
	headsUp	= _user_headsUp;

	//user_lookvec	= _user_lookvec;
}*/
/*
Omni_ViewerPosition::Omni_ViewerPosition(const OmniVec3 &_user_pos,const OmniVec3 &_user_lookatposition)
{
	setLookAt_ByPosition(_user_lookatposition,&_user_pos);
}


void Omni_ViewerPosition::setLookAt_ByPosition(const OmniVec3 &_user_lookatposition, const OmniVec3 &_user_position)
{
	user_lookAtpos = _user_lookatposition;

//	if(_user_position)
	//{
		user_pos =_user_position;
	//}

	OmniVec3 user_lookvec= (_user_lookatposition -user_pos ).returnNormalized();

	OmniVec3 floor_underViewpoint = _user_lookatposition; floor_underViewpoint.y=0;
	OmniVec3 user_Floor_vec= (floor_underViewpoint -user_pos ).returnNormalized();
	
	OmniVec3 SideVector = user_Floor_vec.cross(user_lookvec);
	
	user_headsUp = SideVector.cross(user_lookvec);

}

*/
	Omni_ViewerPosition::~Omni_ViewerPosition()
	{

	}	

	void Omni_ViewerPosition::doGluLookAt()
	{
			gluLookAt (
				pos.x,
				pos.y,
				pos.z,

				lookAtpos.x,
				lookAtpos.y,
				lookAtpos.z,
				headsUp.x,
				headsUp.y,
				headsUp.z);

	}