
//  BE CAREFULL OF inhertience!!!!
//  BE CAREFULL OF inhertience!!!!
//  BE CAREFULL OF inhertience!!!!
//  BE CAREFULL OF inhertience!!!!
//  BE CAREFULL OF performance....
// 


#include "PropertySet/Property.h"
#include "omnimap.h"
#include <map>

//bool IncludePropertySet::SetValue(std::string const& name, std::string const& value)
bool IncludePropertySet::SetValue(const char *name, const char *value)
{
    //std::string strName = name;
    //std::string strValue = value;

	return ((PropertySet *)mypropertyset)->SetValue(name, value);// do error handling here??
}
const char * IncludePropertySet::GetValue(const char * name)
{
	return ((PropertySet *)mypropertyset)->Lookup(name)->GetValue().c_str();// do error handling here??
}

float IncludePropertySet::GetNumber(const char * name)
{
	return ((PropertySet *)mypropertyset)->Lookup(name)->GetNumber(); // do error handling here??
}

IncludePropertySet::IncludePropertySet()
{
	mypropertyset  = new PropertySet();
}
IncludePropertySet::~IncludePropertySet()
{
	delete mypropertyset;
}


void IncludePropertySet::Register(const char * name, int* value)
{
	((PropertySet *)mypropertyset)->Register(name, value);
}

void IncludePropertySet::Register(const char * name, float* value)	
{
	((PropertySet *)mypropertyset)->Register(name, value);
}



void IncludePropertySet::Register(const char * name, const char * value)	
{
	((PropertySet *)mypropertyset)->Register(name, new std::string(value));
}


void IncludePropertySet::Register(const char * name, bool* value)	
{
	((PropertySet *)mypropertyset)->Register(name, value);
}









//////////////////////////////////////////////////////////   /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////    ......GENERIC DATA CONTAINER......
//////////////////////////////////////////////////////////   /////////////////////////////////////////////////////////
typedef GenericDataContainer *pGenericDataContainer;
typedef std::map<std::string,pGenericDataContainer> StringPGDC_MAP;
typedef StringPGDC_MAP *pStringPGDC_MAP;
typedef StringPGDC_MAP::iterator StringPGDC_MAPiterator ;


ExtraDataMapContainer::ExtraDataMapContainer()
	{
		ExtraDataMap = new StringPGDC_MAP;
	}
	ExtraDataMapContainer::~ExtraDataMapContainer()
	{
		pStringPGDC_MAP temp = (pStringPGDC_MAP ) ExtraDataMap; 



		for(StringPGDC_MAPiterator itor = temp->begin() ;itor != temp->end();itor++)
			delete itor->second;
		delete temp;
	}
	GenericDataContainer * ExtraDataMapContainer::IndexDataMap(const char * key)
	{
		pStringPGDC_MAP temp = (pStringPGDC_MAP ) ExtraDataMap; 
		return (*temp)[key];
	}

GenericDataContainer * ExtraDataMapContainer::AddFLOAT(const char * key)
{
		pStringPGDC_MAP temp = (pStringPGDC_MAP ) ExtraDataMap; 
		if((*temp)[key]) LogSystem()->ReportError("Overwriting Metadata %s",key);
		return (*temp)[key] = new GenericDataContainer(GenericDataContainer::FLOAT);	
}
GenericDataContainer * ExtraDataMapContainer::AddBOOL(const char * key)		
{
		pStringPGDC_MAP temp = (pStringPGDC_MAP ) ExtraDataMap; 

		if((*temp)[key]) LogSystem()->ReportError("Overwriting Metadata %s",key);


		return (*temp)[key] = new GenericDataContainer(GenericDataContainer::BOOL);	
}

GenericDataContainer * ExtraDataMapContainer::AddINT(const char * key)
{
		pStringPGDC_MAP temp = (pStringPGDC_MAP ) ExtraDataMap; 
		if((*temp)[key]) LogSystem()->ReportError("Overwriting Metadata %s",key);
		return (*temp)[key] = new GenericDataContainer(GenericDataContainer::INT);	
}
GenericDataContainer * ExtraDataMapContainer::AddSTRING(const char * key)
{
		pStringPGDC_MAP temp = (pStringPGDC_MAP ) ExtraDataMap; 
		if((*temp)[key]) LogSystem()->ReportError("Overwriting Metadata %s",key);
		return (*temp)[key] = new GenericDataContainer(GenericDataContainer::STRING);	
}

//////////////////////////////////////////////////////////   /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////    ......GENERIC DATA CONTAINER......
//////////////////////////////////////////////////////////   /////////////////////////////////////////////////////////

typedef std::string  * pstring;

	 GenericDataContainer::GenericDataContainer(Type t)
	{
		m_string = new std::string;
		m_type = t;
	}
	 GenericDataContainer::~GenericDataContainer()
	{

		std::string * temp = (pstring) m_string;
		delete temp;
		temp= 0;

	}


	float  GenericDataContainer::GetFLOAT()
	{
		switch(m_type)
		{
		case INT: return  (float)m_int;
		case BOOL: return  m_bool?1.0f:0.0f;
		case FLOAT: return (float)m_float;

		case STRING:
		default:
			LogSystem()->ReportError("Error: BAD type:\t");
			LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
			assert("BAD type\n");
			
			break;

		}
		return 0;

	}

	int  GenericDataContainer::GetINT()
	{
		switch(m_type)
		{
		case INT: return  m_int;
		case BOOL: return  (int)m_bool;
		case FLOAT: return (int)m_float;

		case STRING:
		default:
			LogSystem()->ReportError("Error: BAD type:\t");
			LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
			assert("BAD type\n");
			
			break;

		}
		return 0;

	}

	bool  GenericDataContainer::GetBOOL()
	{
		switch(m_type)
		{
		case INT: return  m_int==0?false:true;
		case BOOL: return  m_bool;

		case FLOAT: return (m_float==0.0);

		case STRING:
		default:
			LogSystem()->ReportError("Error:BAD type:");
			LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
			assert("BAD type\n");
			return 0;
			break;
		}
	}

	const char *  GenericDataContainer::GetSTRING()
	{
		switch(m_type)
		{
		case INT: 
			/*
			{
				
				char buffer[100];
				sprintf(buffer,"%d",m_int);
				char *retVal = new char[strlen(buffer)+1];
				return  retVal;
			}
			*/
		case FLOAT:
			/*
			{
				char buffer[100];
				sprintf(buffer,"%f",m_float);
				return  std::string ( buffer);
			}
			*/
		case BOOL: 
			/*
			{
				return m_bool?std::string ( "true"):std::string ( "false");
			}
			*/
		case STRING:
			{
				return ((pstring) m_string)->c_str();
			}
		default:
			LogSystem()->ReportError("Error:BAD type:");
			LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
			assert("BAD type\n");
			return "BAD type";
			break;
		}
	}


	void GenericDataContainer::Set(int value)
	{
		char buffer[256];
		sprintf(buffer,"%d",value);
		*((pstring)m_string) = buffer;
		switch(m_type)
		{
		case INT:
			m_int = value;
		break;
		case FLOAT: 
			m_float= (float)value;
		break;
		case BOOL: 
			m_bool = value!=0;
		break;
		
		case STRING:
			/*
			{
				char buffer[256];
				sprintf(buffer,"%d",value);
				*((pstring)m_string) = buffer;
			}
			*/
		break;
		default:
			LogSystem()->ReportError("Error:BAD type:");
			LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
			assert("BAD type\n");

			break;
		}



	}

void GenericDataContainer::Set(bool  value)
	{
		*((pstring)m_string)  = value? "true":"false";
		switch(m_type)
		{
		case INT:
			m_int = value;
		break;
		case FLOAT: 
			m_float= value;
		break;
		case BOOL: 
			m_bool = value;
		break;
		/*
		case STRING:
			{
					*((pstring)m_string)  = value? "true":"false";
			}
		*/
		break;
		default:
			LogSystem()->ReportError("Error:BAD type:");
			LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
			assert("BAD type\n");

			break;
		}

}
	void GenericDataContainer::Set(float  value)
	{
		char buffer[256];
		sprintf(buffer,"%f",value);
		*((pstring)m_string) = buffer;
		switch(m_type)
		{
		case INT:
			m_int = (int) value;
		break;
		case FLOAT: 
			m_float= value;
		break;
		case BOOL: 
			m_bool = 0.0f!=value;
		break;
		case STRING:
			/*
			{
				char buffer[256];
				sprintf(buffer,"%f",value);
				*((pstring)m_string) = buffer;
			}
			*/
		break;
		default:
			LogSystem()->ReportError("Error:BAD type:");
			LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
			assert("BAD type\n");
			break;
		}
	}

/*
		void GenericDataContainer::Set( char const * const value)
		{
			GenericDataContainer::Set(std::string(value));
		}
*/

	void GenericDataContainer::Set(const char * value)
	{
		float f = 0;
		(*((std::string *)m_string)) = value;
		int nscaned = sscanf(value,"%f",&f);
		switch(m_type)
		{
		case INT:
			if(nscaned>0)
			{
				m_int=(int)f;
			}
			else
			{
			LogSystem()->ReportError("Error:BAD type:");
			LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
			assert("BAD type\n");
			}
		break;
		case FLOAT: 
			if(nscaned>0)
			{
				m_float=f;
			}
			else
			{
				LogSystem()->ReportError("Error:BAD type:");
				LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
				assert("BAD type\n");
			}

		break;
		case BOOL:

			{
				std::string *strValue = (pstring) m_string;
			m_bool = !((strValue->compare("false") == 0) || 
							 (strValue->compare("False") == 0)||
							 (strValue->compare("0") == 0)||
							 (strValue->compare("nil") == 0)||
							 (strValue->compare("FALSE") == 0))
							;

			}
			LogSystem()->ReportError("Carefull setting a bool value with a string...  automatic conversion not tested yet:");

			/*
			LogSystem()->ReportError("Error:BAD type:");
			LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
			assert("BAD type\n");
			*/
		break;
		
		case STRING:
		/*
		{
			(*((std::string *)m_string)) = value;
		}
		*/
		break;
		
		default:
			LogSystem()->ReportError("Error:BAD type:");
			LogSystem()->ReportError("MAKE THIS FUNCTION MORE GENERAL\t");
			assert("BAD type\n");
			break;
		}
	}



