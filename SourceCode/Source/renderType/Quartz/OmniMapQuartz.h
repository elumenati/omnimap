#ifndef __OMNIMAP_QUARTZ_H__
#define __OMNIMAP_QUARTZ_H__
/*
 *  OmniMapQuartz.h
 *  QuartzGLTest
 *
 *  Created by Jim Terhorst on 1/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <OmniMapAPI/omnimap.h>
#include <OpenGL/gl.h>
class OM_FlatScreen;
class OmniMapChannelQuartz;

class OmniMapQuartz : public OmniMap
{
public:
	OmniMapQuartz(int _resW, int _resH, 
		const char* strStartUpScriptFile= OMNIMAP_DEFAULT_STARTUPSCRIPT,
		const char *luaResDir = 0);
	void SetUpPropertyAccess();
		
	void init();
		
	OmniMapChannelBase *Create_Channel(const char * name);
	void setLensOffsetParams(float x, float y, float z);
	void setFlatScreenOrientation(double x, double y, double z);
	void setProjectorValues(double posX, double posY, double posZ, 
		double lookPtX, double lookPtY, double lookPtZ,
		double upX, double upY, double upZ);
	void rotateProjector(double axisX, double axisY, double axisZ, double rads);
	void rollProjector(double angle);
	void setDoWireframe(bool doit) { doWireframe = doit; }
	
	void LoadAndPush_ProjectiveTexturing_Matricies();
	void setupProjTextMats(int startChan, int endChan); 
	void PostRender(); 
	
	
private:
	GLint textUnitTID[12];
	OM_FlatScreen *flatScreen;
	bool useFlatScreen;
	bool doWireframe;
	float cropOffsetX, cropOffsetY, cropOffsetZ;
};


class OmniMapChannelQuartz : public OmniMap_Channel
{

public:
	OmniMapChannelQuartz();
	
	void SetTexureID(unsigned int id);
	
	virtual void beginRenderToChannel();
	
	virtual void endRenderToChannel();
	
	void BindTexture(int index);
	
	void UnbindTexture(int index);
	
	void Initialize();

	void DrawHeadsUpDisplay();
	
	void SetAudiencePos(float x, float y, float z);
	virtual void ApplyViewMatrix();
	void TransformPoint(float mat[16], float pt[3], float transformedPt[3]);
private:
	GLint textureID;
	float audPosX, audPosY, audPosZ;

	
	
};
#endif