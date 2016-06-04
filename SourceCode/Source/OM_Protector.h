#ifndef __PROTECTOR__H_
#define __PROTECTOR__H_

#include <string>
#include <vector>


class OM_ProtectorImpl;
class OMNIMAP_API OM_Protector
{
public :
  OM_Protector();
  bool isProtected();
  bool unlock(const char *key, const char *result);

private :
  OM_ProtectorImpl *protectorImpl;
};
extern OMNIMAP_API OM_Protector om_protector;

#endif