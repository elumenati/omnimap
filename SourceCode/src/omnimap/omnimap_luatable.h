// OmniMap Geometry Correction Libs
// (c)2006 Elumenati
#pragma once
#include "cLua/cLua.h"
extern bool SAVE_AFTER_EVERY_MESSAGE;


#ifdef __OMNIMAP_BASE_H
void LoadLuaFunctionTable(CLua *l,OmniMapBase * _pointertoomnimap);
#else
void LoadLuaFunctionTable(CLua *l,OmniMap * _pointertoomnimap);
#endif
