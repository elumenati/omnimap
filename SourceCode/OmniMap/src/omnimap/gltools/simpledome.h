#pragma once
#include "../omnimap.h"

// Dome Needs to be recompiled after every tweak
class displayHyperbola
{
private:

public:
	displayHyperbola::displayHyperbola();
	displayHyperbola::~displayHyperbola();

	void display();



	OmniVec3 Center;
	OmniVec3 ApexDirection;
	int tesselation;
	float Radius;


};
