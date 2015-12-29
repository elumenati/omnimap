#include <string>
#include <map>
#include "OmniMapBase.h"
#include "OM_ErrorHandling.h"


typedef std::map<std::string,fpScreenRendererCreatorFunction> ScreenRendererCreatorMAP;

void ScreenGeometryBase::SetRenderer(ScreenRendererBase *aRenderer)
{
	renderer = aRenderer;
}



ScreenRendererBase *ScreenGeometryBase::GetRenderer()
{
	return(renderer);
}



void OmniMapScreenRendererFactory::addScreenRendererCreator(const char * type,
								fpScreenRendererCreatorFunction functionPointer)
{
  EH_DECLARE;

	if( (*(ScreenRendererCreatorMAP *) ScreenRendererCreatorFunctionMAP).find(type) 
			!= (*(ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP).end())
	{
		EH_Log("constructor for type %s allready defined... overwriting it.",type);
	}
	(*(ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP) [type] = functionPointer;
	
}




ScreenRendererBase * OmniMapScreenRendererFactory::Create_ScreenRenderer(const char * type)
{
  EH_DECLARE;

	EH_Log("Creating screenshape of type %s",type);

	if((*(ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP) [type])
	{
		return (*(ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP) [type]();
	}
	else
	{
		EH_Log("Error Creating screenrender of type %s.",type);
		return NULL;
	}


}



OmniMapScreenRendererFactory::OmniMapScreenRendererFactory()
{
	ScreenRendererCreatorFunctionMAP = (ScreenRendererCreatorMAP *) new ScreenRendererCreatorMAP();
	((ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP)->clear();	

}

OmniMapScreenRendererFactory::~OmniMapScreenRendererFactory()
{
	((ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP)->clear();	
	delete ((ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP);	
	ScreenRendererCreatorFunctionMAP = 0;
}


