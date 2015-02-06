#include <string>
#include <map>
#include "OmniMapBase.h"

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

	if( (*(ScreenRendererCreatorMAP *) ScreenRendererCreatorFunctionMAP).find(type) 
			!= (*(ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP).end())
	{
		LogSystem()->ReportError("constructor for type %s allready defined... overwriting it.",type);
	}
	(*(ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP) [type] = functionPointer;
	
}


ScreenRendererBase * OmniMapScreenRendererFactory::Create_ScreenRenderer(const char * type)
{

	LogSystem()->ReportMessage("Creating screenshape of type %s",type);

	if((*(ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP) [type])
	{
		return (*(ScreenRendererCreatorMAP *)ScreenRendererCreatorFunctionMAP) [type]();
	}
	else
	{
		LogSystem()->ReportMessage("Error Creating screenrender of type %s.",type);
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


