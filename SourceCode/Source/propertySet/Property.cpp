/* Copyright (C) Charles Cafrelli, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Charles Cafrelli, 2001"
 */
//----- INCLUDES -----------------------------------------------//
#include <string>
#ifdef ELUMENATI_INTERNAL
#include <iostream>
#include <fstream>
#endif
#if defined(USE_VC7)  || defined(__APPLE__)
#define sprintf_s sprintf
#define vsprintf_s vsprintf
#endif
#include "Property.h"
#include "OmniMapBase.h"
#include "OM_ErrorHandling.h"
//----- DEFINES ------------------------------------------------//


//----- GLOBALS ------------------------------------------------//


//----- STATICS ------------------------------------------------//


//----- EXTERNS ------------------------------------------------//


//----- CODE ---------------------------------------------------//
typedef std::list<std::string> ListOfStrings;

bool Property::SetUnknownValue(std::string const& value)
{
	bool return_value = true;
	switch(m_type)
	{
		case INT:
			*m_data.m_int = atoi(value.c_str());
			break;
		case FLOAT:
			*m_data.m_float = (float)atof(value.c_str());
			break;
		case STRING:
			*m_data.m_string = value;
			break;
		case OMCSTRING:
			strcpy((char *) m_data.m_cstring, value.c_str());
			break;
		case BOOL:
			*m_data.m_bool = !((value.compare("false") == 0) || 
							 (value.compare("False") == 0)||
							 (value.compare("0") == 0)||
							 (value.compare("nil") == 0)||
							 (value.compare("FALSE") == 0))
							;
			break;
		default:
			return_value = false;
			break;
	}

	return return_value;
}

bool Property::Set(int value)
{
	bool return_value = false;
	if(m_type == INT)
	{
		*m_data.m_int = value;
		return_value = true;
	}

	return return_value;
}

bool Property::Set(float value)
{
	bool return_value = false;
	if(m_type == FLOAT)
	{
		*m_data.m_float = value;
		return_value = true;
	}

	return return_value;
}

bool Property::Set(bool value)
{
	bool return_value = false;
	if(m_type == BOOL)
	{
		*m_data.m_bool = value;
		return_value = true;
	}

	return return_value;
}

bool Property::Set(std::string const& value)
{
	bool return_value = false;
	if(m_type == STRING)
	{
		*m_data.m_string = value;
		return_value = true;
	}

	return return_value;
}

std::string Property::GetValue()
{
	std::string return_value;
	char buffer[64];
	switch(m_type)
	{
		case INT:
			sprintf_s(buffer,"%d",*m_data.m_int);
			return_value = buffer;
			break;
		case FLOAT:
			sprintf_s(buffer,"%f",*m_data.m_float);
			return_value = buffer;
			break;
		case STRING:
			return_value = *m_data.m_string;
			break;
		case OMCSTRING:
			return_value = m_data.m_cstring;
			break;
		case BOOL:
			if(*m_data.m_bool)
				sprintf_s(buffer,"true");
			else
				sprintf_s(buffer,"false");
			return_value = buffer;
			break;
	}

	return return_value;
}

#ifdef ELUMENATI_INTERNAL
Property::Type Property::GetPropertyType() {
	return m_type;
}
#endif





//----- PropertySet -------------------------------

PropertySet::PropertySet()
{
#ifdef ELUMENATI_INTERNAL
	propertyNameList = new ListOfStrings();
#endif
}

PropertySet::~PropertySet()
{
  for(int index= 0; index != m_properties.GetTableSize(); index++)
  {
    if(m_properties.m_hash_table[index].size() == 0)
      continue;
#ifdef __APPLE__
    std::list<HashTableNode<Property> >::iterator walker;
#else
    std::list< HashTable<Property>::HashTableNode<Property> >::iterator walker;
#endif
    walker = m_properties.m_hash_table[index].begin();
    while(walker != m_properties.m_hash_table[index].end())
    {
#ifdef __APPLE__
      HashTableNode<Property>* return_value = 0;
#else
      HashTable<Property>::HashTableNode<Property>* return_value = 0;
#endif
      return_value = &(*walker);
      if (return_value->m_data) delete return_value->m_data;
      walker++;
    }
  }
}




void PropertySet::Register(std::string const& name, int* value)
{
	Property* new_property = new Property(name,value);
	m_properties.Insert(name,new_property);
#ifdef ELUMENATI_INTERNAL
	((ListOfStrings *) propertyNameList)->push_front(name);
#endif
}

void PropertySet::Register(std::string const& name, float* value)
{
	Property* new_property = new Property(name,value);
	m_properties.Insert(name,new_property);
#ifdef ELUMENATI_INTERNAL
	((ListOfStrings *) propertyNameList)->push_front(name);
#endif
}

void PropertySet::Register(std::string const& name, const char * value)
{
	Property* new_property = new Property(name,value);
	m_properties.Insert(name,new_property);
#ifdef ELUMENATI_INTERNAL
	((ListOfStrings *) propertyNameList)->push_front(name);
#endif
}

void PropertySet::Register(std::string const& name, std::string *value)
{
	Property* new_property = new Property(name,value);
	m_properties.Insert(name,new_property);
#ifdef ELUMENATI_INTERNAL
	((ListOfStrings *) propertyNameList)->push_front(name);
#endif
}

void PropertySet::Register(std::string const& name, bool* value)
{
	Property* new_property = new Property(name,value);
	m_properties.Insert(name,new_property);
#ifdef ELUMENATI_INTERNAL
	((ListOfStrings *) propertyNameList)->push_front(name);
#endif
}

Property* PropertySet::Lookup(std::string const& name)
{
  EH_DECLARE;
	Property* p =  m_properties.Lookup(name);

	if(!p)
	{
    EH_SetErrorCode(EH_ERR_FAILED);
		EH_Log("Error: property %s not found...",name.c_str());
	}
	return p;
}



bool PropertySet::SetValue(std::string const& name, std::string const& value)
{
	bool return_value = false;
	Property* property = Lookup(name);
	if(property)
	{
		return_value = property->SetUnknownValue(value);
	}

	return return_value;
}



bool PropertySet::Set(std::string const& name, std::string const& value)
{
	bool return_value = false;
	Property* property = Lookup(name);
	if(property)
	{
		return_value = property->Set(value);
	}

	return return_value;
}



bool PropertySet::Set(std::string const& name, int value)
{
	bool return_value = false;
	Property* property = Lookup(name);
	if(property)
	{
		return_value = property->Set(value);
	}

	return return_value;

}



bool PropertySet::Set(std::string const& name, float value)
{
	bool return_value = false;
	Property* property = Lookup(name);
	if(property)
	{
		return_value = property->Set(value);
	}

	return return_value;
}



bool PropertySet::Set(std::string const& name, bool value)
{
	bool return_value = false;
	Property* property = Lookup(name);
	if(property)
	{
		return_value = property->Set(value);
	}

	return return_value;
}



bool PropertySet::Set(std::string const& name, char* value)
{
	bool return_value = false;
	Property* property = Lookup(name);
	if(property)
	{
		return_value = property->Set(std::string(value));
	}

	return return_value;
}





void PropertySet::CopyAllRegisteredVariblesTo(PropertySet *to)
{
  for(int index= 0; index != m_properties.GetTableSize(); index++)
  {
    if(m_properties.m_hash_table[index].size() == 0)
      continue;
#ifdef __APPLE__
    std::list<HashTableNode<Property> >::iterator walker;
#else
    std::list< HashTable<Property>::HashTableNode<Property> >::iterator walker;
#endif
    walker = m_properties.m_hash_table[index].begin();
    while(walker != m_properties.m_hash_table[index].end())
    {
#ifdef __APPLE__
      HashTableNode<Property>* return_value = 0;
#else
      HashTable<Property>::HashTableNode<Property>* return_value = 0;
#endif
      return_value = &(*walker);
      to->SetValue(walker->m_key,return_value->m_data->GetValue());
      //			LogSystem()->ReportMessage("Varible %s = %s",walker->m_key.c_str(),return_value->m_data->GetValue().c_str());
      walker++;
    }
  }
}




void PropertySet::LogAllVariblesAndValues()
{
  EH_DECLARE;

  for(int index= 0; index != m_properties.GetTableSize(); index++)
  {
    if(m_properties.m_hash_table[index].size() == 0)
      continue;


    //	m_properties.m_hash_table::iterator walker;
#ifdef __APPLE__
    std::list<HashTableNode<Property> >::iterator walker;
#else
    std::list< HashTable<Property>::HashTableNode<Property> >::iterator walker;
#endif
    walker = m_properties.m_hash_table[index].begin();

    while(walker != m_properties.m_hash_table[index].end())
    {

#ifdef __APPLE__
      HashTableNode<Property>* return_value = 0;
#else
      HashTable<Property>::HashTableNode<Property>* return_value = 0;
#endif


      //			return_value->m_data


      return_value = &(*walker);
      EH_Log("Varible %s = %s",walker->m_key.c_str(),return_value->m_data->GetValue().c_str());
      //			

      walker++;
    }
  }
}




#ifdef ELUMENATI_INTERNAL
void PropertySet::writeVariables(void *filePtr, const char *name)
{
	std::ofstream *configFile = (std::ofstream *) filePtr;

	for(int index= 0; index != m_properties.GetTableSize(); index++)
	{
		if(m_properties.m_hash_table[index].size() == 0)
			continue;


	//	m_properties.m_hash_table::iterator walker;
#ifdef __APPLE__
	std::list<HashTableNode<Property> >::iterator walker;
#else
        std::list< HashTable<Property>::HashTableNode<Property> >::iterator walker;
#endif
		walker = m_properties.m_hash_table[index].begin();

		while(walker != m_properties.m_hash_table[index].end())
		{

#ifdef __APPLE__
			HashTableNode<Property>* return_value = 0;
#else
                        HashTable<Property>::HashTableNode<Property>* return_value = 0;
#endif

	
			return_value = &(*walker);
			
			*configFile << name << "." << walker->m_key << " = " << return_value->m_data->GetValue() << "\n";
//			
			
			walker++;
		}
	}


}

int PropertySet::getNumProperties() {
	return((int) ((ListOfStrings *) propertyNameList)->size());
}

const char *PropertySet::getPropertyName(int which) {
	ListOfStrings *propNames = (ListOfStrings *) propertyNameList;
	int i = 0;
	for (ListOfStrings::iterator itr = propNames->begin(); itr != propNames->end(); itr++, i++) {
		if (i == which) {
			std::string s = *itr;
			char *pc = new char[strlen(s.c_str())+1];
			strcpy(pc, s.c_str());
			return pc;
		}
	}
	return(NULL);

}

Property::Type PropertySet::GetPropertyType(char *name)
{
	Property* prop = Lookup(name);
	if(prop)
	{
		return(prop->GetPropertyType());
	}
	else return(Property::Type::EMPTY);
}
#endif
