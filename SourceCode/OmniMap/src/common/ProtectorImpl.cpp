#include "OmniMapBase.h"
#include "OM_Protector.h"
#include "string.h"
#include "stdio.h"
#include "md5.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

/*om_protector.unlock("Elumenati", "3622e0f");*/
class OM_ProtectorImpl 
{
public:
	OM_ProtectorImpl()
	{
		locked = true;
	}

	void lock()
	{
		locked = true;
	}

	bool unlock(const char *key, const char *result)
	{
		int iresult;
		if (key[0] >= '0' && key[0] <= 'M')
			iresult = APHash(key);
		else if (key[0] > 'M' && key[0] <= 'm')
			iresult = BPHash(key);
		else iresult = FNVHash(key);
		char cresult[11];
		sprintf(cresult, "%x", iresult);
		if (strcmp(cresult, result) == 0)
		{
			locked = false;
			return true;
		}
		return false;
	}

	bool isLocked()
	{
		return locked;
	}

	unsigned int BPHash(const char *str)
	{
	   unsigned int hash = 0;
	   for(unsigned int i = 0; i < strlen(str); i++)
	   {
		  hash = hash << 7 ^ str[i];
	   }

	   return hash;
	}



	unsigned int FNVHash(const char * str)
	{
	   const unsigned int fnv_prime = 0x811C9DC5;
	   unsigned int hash = 0;
	   for(unsigned int i = 0; i < strlen(str); i++)
	   {
		  hash *= fnv_prime;
		  hash ^= str[i];
	   }

	   return hash;
	}

	unsigned int APHash(const char *str)
	{
	   unsigned int hash = 0xAAAAAAAA;

	   for(unsigned int i = 0; i < strlen(str); i++)
	   {
		  hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ str[i] ^ (hash >> 3)) :
								   (~((hash << 11) ^ str[i] ^ (hash >> 5)));
	   }

	   return hash;
	}
private:
	bool locked;

};


OM_Protector::OM_Protector()
{
	protectorImpl = new OM_ProtectorImpl();
}

bool OM_Protector::isProtected()
{
	return protectorImpl->isLocked();
}

#ifdef OMNIMAP_DEMO
bool OM_Protector::unlock(const char *key, const char *result)
{
	return false;
}
#else
bool OM_Protector::unlock(const char *key, const char *result)
{
	return protectorImpl->unlock(key, result);
}
#endif



	OM_ProtectorImpl_Cobra ::OM_ProtectorImpl_Cobra (){
		INIT();
	}
	void OM_ProtectorImpl_Cobra ::INIT (){
		cobraRegistered = -1;

	const char* USERPROFILE = getenv("USERPROFILE"); // do not free this string!

	serialFileXP = std::string(USERPROFILE) +std::string("/Local Settings/Application Data")+ std::string("/Navegar Foundation/CobraTrueDimension/serial.ini");
	serialFileWin7 = std::string(USERPROFILE) +std::string("/AppData/Local")+  std::string("/Navegar Foundation/CobraTrueDimension/serial.ini");

//	LogSystem()->ReportError("do readFile : %s",serialFileXP.c_str());
//	LogSystem()->ReportError("do readFile: %s",serialFileWin7.c_str());

	}

	bool OM_ProtectorImpl_Cobra ::IsCobraRegistered(){
		return _IsCobraRegistered();
	}

	bool OM_ProtectorImpl_Cobra ::_IsCobraRegistered(){
		if(cobraRegistered<0){

			try{
			// Get Cobra File
				std::string cobraFile = readFile(serialFileWin7.c_str());
				if(cobraFile.length() <10){
					cobraFile = readFile(serialFileXP.c_str());

				}

				if(cobraFile.length() <10){
					LogSystem()->ReportError("FATAL ERROR: Error loading serial file %s or %s please contact support@elumenati.com to register CobraTrueDimension and enable Omnimap/Omnity warp features.",serialFileXP.c_str(),serialFileWin7.c_str());
					LogSystem()->ReportMessage("FATAL ERROR: Error loading serial file %s or %s please contact support@elumenati.com to register CobraTrueDimension and enable Omnimap/Omnity warp features.",serialFileXP.c_str(),serialFileWin7.c_str());
					cobraRegistered =0;
					return false;
				}
				// process file
				myReplace(cobraFile,"\r","\n");
				myReplace(cobraFile,"\n\n","\n");
				myReplace(cobraFile,"\n\n","\n");
				 std::string line;
				 std::istringstream f  (cobraFile.c_str());

				 std::string mySerial("");
				 std::string myElumenatiActivation("");

				 bool foundSerial =false;
				 bool foundActivation =false;
			    while(std::getline(f, line,'\n')) {
					std::vector<std::string> vec = split(line, '=');
					if(vec.size() >= 2){
						if(caseInsensitiveStringCompare(vec[0],"serial")){
							mySerial = vec[1];
							//LogSystem()->ReportMessage("[ ] Parsing serial(%s)",mySerial.c_str());

							foundSerial =true;
						}
						if(caseInsensitiveStringCompare(vec[0],"ElumenatiActivation")){
							myElumenatiActivation = vec[1];
							foundActivation=true;
							//LogSystem()->ReportMessage("[ ] Parsing ElumenatiActivation(%s)",myElumenatiActivation.c_str());
						}
					}
				}

				if(foundActivation && foundSerial && mySerial.length() >4 && myElumenatiActivation.length()>4){
				// Compute Salt + serial 
				std::string combo = "ElumenatiCobra"+mySerial;
				//LogSystem()->ReportMessage("[ ] computing combo %s ",combo.c_str());

			// compute md5 as expected activation
				std::string expectedActivation = md5(combo);
				////LogSystem()->ReportMessage("[ ] computing expected %s",expectedActivation.c_str());
			// compare to acutal activation
				//LogSystem()->ReportMessage("[ ] comparing %s vs %s",expectedActivation.c_str(),myElumenatiActivation.c_str());


				if(expectedActivation == myElumenatiActivation){
					cobraRegistered = 1;
				}else{
					cobraRegistered = 0;
				}

			// set cobra registered
				//LogSystem()->ReportMessage("[ ] setting cobraRegistered == %d",cobraRegistered);

				}else{
					if(mySerial.length()>4){
						LogSystem()->ReportMessage("Warning the cobra with serial number %s is not registered properly yet with elumenati registration system, please contact support@elumenati.com",mySerial.c_str());
						LogSystem()->ReportError("Warning the cobra with serial number %s is not registered properly yet with elumenati registration system, please contact support@elumenati.com",mySerial.c_str());
					}else{
						LogSystem()->ReportMessage("Warning the cobra is not registered properly yet, please contact support@elumenati.com");
						LogSystem()->ReportError("Warning the cobra is not registered properly yet, please contact support@elumenati.com");
					}
					cobraRegistered = 0;
				}
			}catch(...){
				cobraRegistered = 0;
			}


		}
		return cobraRegistered > 0;
	}

bool OM_ProtectorImpl_Cobra ::caseInsensitiveStringCompare(const std::string& str1, const std::string& str2) {
    if (str1.size() != str2.size()) {
        return false;
    }
	for (std::string::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2) {
        if (tolower(*c1) != tolower(*c2)) {
            return false;
        }
    }
    return true;
}
std::vector<std::string> &OM_ProtectorImpl_Cobra ::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> OM_ProtectorImpl_Cobra ::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
void OM_ProtectorImpl_Cobra ::myReplace(std::string& str, const std::string& oldStr, const std::string& newStr)
{
  size_t pos = 0;
  while((pos = str.find(oldStr, pos)) != std::string::npos)
  {
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
}	


std::string OM_ProtectorImpl_Cobra ::readFile(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
	LogSystem()->ReportMessage("Success reading serial file: %s",filename);
    return(contents);
  }
   return std::string("");
}
/*
	std::string OM_ProtectorImpl_Cobra ::readFile(const char* filename)
{	
	try{
	using namespace std;
	ifstream file;
	file.open(filename);
	
	// open file
	if(!file.is_open())
	{
		LogSystem()->ReportError("Error opening file: %s",filename);
		return "";
	}



	// read file into a string
	string contents = "";
	char *intermediate = new char[1025];
	while(!file.eof())
	{
		for(int i = 0; i < 1025; i++)
		{
			intermediate[i] = '\0';
		}

		file.read(intermediate, 1024);
		contents += intermediate;
	}

	// close file
	file.close();
	delete file;
	delete[] intermediate;
	

	return contents;
	}catch(...){
		return "";
	}
	
}
*/