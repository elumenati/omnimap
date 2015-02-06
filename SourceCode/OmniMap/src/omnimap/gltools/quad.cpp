#include "../privatePrimitives.h"
#include "../GraphicsPrimitives.h"





	OmniQuad::OmniQuad()
	{
	};
	OmniQuad::OmniQuad(
		float blx,float bly,float blz,
		float tlx,float tly,float tlz,
		float trx,float tr_y,float trz,
		float brx,float bry,float brz
		)
	{
		tl.x =tlx;
		tl.y =tly;
		tl.z =tlz;

		bl.x =blx;
		bl.y =bly;
		bl.z =blz;

		tr.x =trx;
		tr.y =tr_y;
		tr.z =trz;

		br.x =brx;
		br.y =bry;
		br.z =brz;

	};
	void OmniQuad::set(
		float blx,float bly,float blz,
		float tlx,float tly,float tlz,
		float trx,float tr_y,float trz,
		float brx,float bry,float brz
		)
	{
		tl.x =tlx;
		tl.y =tly;
		tl.z =tlz;

		bl.x =blx;
		bl.y =bly;
		bl.z =blz;

		tr.x =trx;
		tr.y =tr_y;
		tr.z =trz;

		br.x =brx;
		br.y =bry;
		br.z =brz;


	};
	OmniQuad::~OmniQuad(){};

/*
Point4	QuadToPlaneEquation()
{

}*/
	void QuadToMatrix()
	{

	}

/*
*/

