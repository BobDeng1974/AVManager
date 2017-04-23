#ifndef DICTIONARY_H_
#define DICTIONARY_H_


#include <map>
#include <algorithm>
#include <c_string.h>
using namespace utility;


class Dictionary
{
public:
	Dictionary() {}
	Dictionary( const std::string& name ) : m_name( name ){}
	virtual ~Dictionary() {}

	typedef std::map < std::string, std::string > Data;
	typedef Data::const_iterator iterator;
	typedef iterator const_iterator;

	size_t size() const { return m_data.size(); }

	std::string GetName() const { return m_name; }
	/// Get a value as a string.
	std::string GetString( const std::string&, bool capitalize = false ) const;
	/// Get a value as a int.
	int GetInt( const std::string& ) const;
	/// Get a value as a double.
	double GetDouble( const std::string& ) const;
	/// Get a value as a bool
	bool GetBool( const std::string& ) const;

	/// Set a value from a string.
	void SetString( const std::string&, const std::string& );
	/// Set a value from a int.
	void SetInt( const std::string&, int );
	/// Set a value from a double.
	void SetDouble( const std::string&, double );
	/// Set a value from a bool
	void SetBool( const std::string&, bool );

	iterator begin() const { return m_data.begin(); }
	iterator end() const { return m_data.end(); }

private:
	Data m_data;
	std::string m_name;
};



#endif