/* Copyright (C) Charles Cafrelli, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Charles Cafrelli, 2001"
 */
#ifndef Property_H
#define Property_H

//----- INCLUDES -----------------------------------------------//
#include <string>
#include "HashTable.h"
#include "assert.h"
		
//----- DEFINES ------------------------------------------------//

//----- GLOBALS ------------------------------------------------//


//----- EXTERNS ------------------------------------------------//

				   
//----- FORWARD DECLARATIONS -----------------------------------//

				   
//----- CLASS --------------------------------------------------//



class Property
{
	//----- Constants -----//
protected:
	union Data
	{
		int* m_int;
		float* m_float;
		std::string* m_string;
		bool* m_bool;
		const char *m_cstring;
	};

#ifdef ELUMENATI_INTERNAL
	public:
#endif
	enum Type
	{
		INT,
		FLOAT,
		STRING,
		BOOL,
		OMCSTRING,
		EMPTY
	};




	//----- Properties -----//
private:
protected:
	Data	m_data;
	Type	m_type;
	std::string m_name;
public:

	//----- Friends -----//

	//----- Methods -----//
private:
	// declared to prevent unintentional use...
	// (Don't forget to move to public access if you declare them!)
	Property& Copy(const Property& source_object)
	{
		m_data = source_object.m_data;
		m_type = source_object.m_type;
		m_name = source_object.m_name;

		return *this;
	}

	Property& operator= (const Property& source_object)
	{
		return Copy(source_object);
	}
protected:
	void EraseType()
	{
		m_type = EMPTY;
		m_data.m_int = 0;
		m_name = "";
	}

	void Register(int* value)
	{
		m_type = INT;
		m_data.m_int = value;
	}

	void Register(float* value)
	{
		m_type = FLOAT;
		m_data.m_float = value;
	}

	void Register(std::string* new_string)
	{
		m_type = STRING;
		m_data.m_string = new_string;
	}

	void Register(const char* new_string)
	{
		m_type = OMCSTRING;
		m_data.m_cstring = new_string;
	}

	void Register(bool* value)
	{
		m_type = BOOL;
		m_data.m_bool = value;
	}



	//----- overridables -----//

	//----- pure virtual overridables -----//

public:
	Property()
	{
		EraseType();
	}
	Property(const Property& source)
	{
		Copy(source);
	}

	Property(std::string const& name)
	{
		EraseType();
		m_name = name;
	}

	Property(std::string const& name, int* value)
	{
		m_name = name;
		Register(value);
	}

	Property(std::string const& name, float* value)
	{
		m_name = name;
		Register(value);
	}

	Property(std::string const& name, std::string* value)
	{
		m_name = name;
		Register(value);
	}

	Property(std::string const& name, const char* value)
	{
		m_name = name;
		Register(value);
	}

	Property(std::string const& name, bool* value)
	{
		m_name = name;
		Register(value);
	}


	// remove virtual if class will not be inherited to improve efficiency
//	virtual ~Property();

	bool SetUnknownValue(std::string const& value);
	bool Set(int value);
	bool Set(float value);
	bool Set(bool value);
	bool Set(std::string const& value);


	void SetName(std::string const& name)
	{
		m_name = name;
	}

	std::string GetName() const
	{
		return m_name;
	}

	int GetInt()
	{
		assert(m_type == INT);
		return *m_data.m_int;
	}

	float GetFloat()
	{
		assert(m_type == FLOAT);
		return *m_data.m_float;
	}

	float GetNumber()
	{
		assert(m_type == BOOL || m_type == INT || m_type == FLOAT);

		if(m_type== INT)
			return (float)*m_data.m_int;
		else if(m_type == FLOAT)
			return (float)*m_data.m_float;
		else if(m_type == BOOL)
			return (float)*m_data.m_bool;
		else
			return (float)911.911f;

			
	}

	std::string GetString()
	{
		assert(m_type == STRING);
		return *m_data.m_string;
	}

	bool GetBool()
	{
		assert(m_type == BOOL);
		return *m_data.m_bool;
	}

	// generic return...
	std::string GetValue();

#ifdef ELUMENATI_INTERNAL
	Type GetPropertyType();
#endif


	//----- overridables -----//

	//----- pure virtual overridables -----//
};



class PropertySet
{
	//----- Constants -----//
protected:
public:

	//----- Properties -----//
private:
protected:
	HashTable<Property>	m_properties;
public:

	//----- Friends -----//

	//----- Methods -----//
private:
	// declared to prevent unintentional use...
	// (Don't forget to move to public access if you declare them!)
//	PropertySet& Copy(const PropertySet& source_object);
//	PropertySet& operator= (const PropertySet& source_object);
//	{
//		return Copy(source_object);
//	}
protected:
	//----- overridables -----//

	//----- pure virtual overridables -----//
public:
	PropertySet();
	/*PropertySet(const PropertySet& source)
	{
		Copy(source);
	}*/

	// remove virtual if class will not be inherited to improve efficiency
	virtual ~PropertySet();

	Property* Lookup(std::string const& name);


	void Register(std::string const& name, int* value); 
	void Register(std::string const& name, float* value); 
	void Register(std::string const& name, std::string* value); /// switch to safer char * , int size
	void Register(std::string const& name, bool* value); 
	void Register(std::string const& name, const char* value); 

	// use this for generic access when the data type is unknown
	// this will convert to the proper format.
	bool SetValue(std::string const& name, std::string const& value);
    //bool SetValue(const char *name, const char *value);
	// use these if you know the data type
	bool Set(std::string const& name, std::string const& value);
	bool Set(std::string const& name, int value);
	bool Set(std::string const& name, float value);
	bool Set(std::string const& name, bool value);
	bool Set(std::string const& name, char* value);



	void LogAllVariblesAndValues();
	void CopyAllRegisteredVariblesTo(PropertySet *to);
#ifdef ELUMENATI_INTERNAL
	void writeVariables(void *filePtr, const char *name);
	int getNumProperties();
	const char *getPropertyName(int i);
	Property::Type GetPropertyType(char *name);
	void *propertyNameList;
#endif

	//----- overridables -----//

	//----- pure virtual overridables -----//
};



#endif
