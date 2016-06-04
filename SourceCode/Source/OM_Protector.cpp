#include "OmniMapBase.h"
#include "OM_Protector.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

OMNIMAP_API OM_Protector om_protector;

OM_Protector::OM_Protector()
{
}

bool OM_Protector::isProtected()
{
	return false;
}

#ifdef OMNIMAP_DEMO
bool OM_Protector::unlock(const char *key, const char *result)
{
	return false;
}
#else
bool OM_Protector::unlock(const char *key, const char *result)
{
	return true;
}
#endif


