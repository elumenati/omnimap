#pragma once
// dont export this file

#include <map>
#include <string>
#include <list>

typedef  std::map<std::string,pOmniMap_Screen>  String_OmniMap_Screen_Map;
typedef  String_OmniMap_Screen_Map::iterator String_OmniMap_Screen_Map_itor;

typedef  std::map<std::string,OmniMapChannelBase *>  String_OmniMap_Channel_Map;
typedef  String_OmniMap_Channel_Map::iterator String_OmniMap_Channel_Map_itor;

typedef  std::list<std::string> stringList;
typedef  stringList::iterator stringListItor;
