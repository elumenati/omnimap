#include "omnimapbase.h"

#ifdef UNUSED_CODE
class OmniMap_Screen_Quad : public OmniMap_Screen
{
private:
	virtual	void Render();/*todo make this use display lists*/
	virtual	void SetUpPropertyAccess();
public:
	virtual void Display(){Render();/*todo make this use display lists*/}
	OmniQuad *quad;
	int quad_tesselation;

	bool RenderTexCoordinates;
	float blU;
	float blV;
	float widthU;
	float heightV;
	void SetTextureCoordinates(float bu,float bv, float w,float h);
	OmniMap_Screen_Quad();
	~OmniMap_Screen_Quad();

	void BaseInit(){}/*todo make this use display lists*/
	void BaseDestroy(){}/*todo make this use display lists*/

};


class OmniMap_Screen_FILE : public OmniMap_Screen
{
private:
	// to do add filename....
	// model....
	virtual	void Render(){};
	virtual	void SetUpPropertyAccess(){};
public:
	virtual void Display(){Render();}
/*
	bool RenderTexCoordinates;
	void OmniMap_Screen_Quad::SetTextureCoordinates(float bu,float bv, float w,float h)
	{
		RenderTexCoordinates = true;
		 blU = bu;
		 blV =bv;
		 widthU =w;
		 heightV =h;
	}
*/
	OmniMap_Screen_FILE(char * filename)
	{
		// to do
	}  
	~OmniMap_Screen_FILE()
	{
		// to do
	}  
	void BaseInit()
	{
		// to do
	}  
	void BaseDestroy()
	{
		// to do
	}  
};
#endif
