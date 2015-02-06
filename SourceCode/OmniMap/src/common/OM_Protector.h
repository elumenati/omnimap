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


class OM_ProtectorImpl_Cobra 
{	
public:
	OM_ProtectorImpl_Cobra ::OM_ProtectorImpl_Cobra ();
	bool IsCobraRegistered();

	void OM_ProtectorImpl_Cobra ::INIT();
	
private:
	std::string serialFile; //= "%LOCALAPPDATA%";
	
	bool _IsCobraRegistered();
	bool caseInsensitiveStringCompare(const std::string& str1, const std::string& str2);
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
	void myReplace(std::string& str, const std::string& oldStr, const std::string& newStr);
	std::string readFile(const char* filename);
	

	int cobraRegistered;
	std::string serialFileXP;
	std::string serialFileWin7;

};

#endif