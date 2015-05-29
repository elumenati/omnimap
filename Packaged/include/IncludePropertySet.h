#ifndef __INCLUDE_PROPERTY_SET_H__
#define __INCLUDE_PROPERTY_SET_H__


/**
 * \brief Defines a set of properties for a class.
 *
 * Defines a set of properties that can be set using character string to
 * reference the property and define the values.  Current types supported are
 * int, float, bool, and string.
 */
class OMNIMAP_API IncludePropertySet  //  BE CAREFULL OF inhertience issues!!!!
{
private:
	void *mypropertyset;
public:
	/**
	 * Constructor.
	 */
	IncludePropertySet::IncludePropertySet();
	/**
	 * Destructor.
	 */
	IncludePropertySet::~IncludePropertySet();
	/**
	 * Set the value of property
	 * @param name The name of the property to be set
	 * @param value The value to set the property to
	 */
	//bool IncludePropertySet::SetValue(std::string const& name, std::string const& value);
    bool IncludePropertySet::SetValue(const char *name, const char * value);
	/**
	 * Get the value of a property as a string.
	 * @param name The name of the property whose value is sought.
	 * @returns The string representation of the value sought.
	 */
	const char * IncludePropertySet::GetValue(const char *name);
	/**
	 * Get the numeric value of a property.
	 * @param name The name of the property whose value is sought.
	 * @returns The numeric value of the property.
	 */
	float IncludePropertySet::GetNumber(const char * name);
	/**
	 * Register a property
	 * @param name The name of the property to register.
	 * @param value The initial value of the property being registered.
	 */
	void IncludePropertySet::Register(const char * name, int* value);
	/**
	 * Register a property
	 * @param name The name of the property to register.
	 * @param value The initial value of the property being registered.
	 */
	void IncludePropertySet::Register(const char *  name, float* value);
	/**
	 * Register a property
	 * @param name The name of the property to register.
	 * @param value The initial value of the property being registered.
	 */
	void IncludePropertySet::Register(const char * name, const char * value);
	/**
	 * Register a property
	 * @param name The name of the property to register.
	 * @param value The initial value of the property being registered.
	 */
	void IncludePropertySet::Register(const char * name, bool* value);

	/*
		make faster and have setValue overloaded for more data types....
	*/
};

#endif