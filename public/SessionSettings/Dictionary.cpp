#include "stdafx.h"
#include "Dictionary.h"


std::string Dictionary::GetString( const std::string& key, bool capitalize) const
{
	Data::const_iterator i = m_data.find( str_tolower(key.c_str()) );
	if ( i == m_data.end() ) return "";

	std::string result = i->second;
	if( capitalize )
		std::transform(result.begin(), result.end(), result.begin(), toupper);

	return result;
}

int Dictionary::GetInt( const std::string& key) const
{
	Data::const_iterator i = m_data.find( str_tolower(key.c_str()) );
	if ( i == m_data.end() ) return 0;

	int result = stoi( i->second);
	return result;
}

double Dictionary::GetDouble( const std::string& key) const
{
	Data::const_iterator i = m_data.find( str_tolower(key.c_str()) );
	if ( i == m_data.end() ) return 0.0f;

	double result = stof( i->second);
	return result;
}

bool Dictionary::GetBool( const std::string& key) const
{
	Data::const_iterator i = m_data.find( str_tolower(key.c_str()) );
	if ( i == m_data.end() ) return false;

	bool result;
	if( i->second.size() != 1 ) return false;
	switch( i->second[0] )
	{
	case 'Y': result = true; break;
	case 'N': result = false; break;
	default: return false;
	}
	return result;
}

void Dictionary::SetString( const std::string& key, const std::string& value)
{
	m_data[ str_trim(str_toupper(key.c_str())) ] = str_trim(value);
}

void Dictionary::SetInt( const std::string& key, int value)
{
	m_data[ str_trim(str_toupper(key.c_str())) ] = to_string(value);
}

void Dictionary::SetDouble( const std::string& key, double value)
{
	m_data[ str_trim(str_toupper(key.c_str())) ] = to_string(value);
}

void Dictionary::SetBool( const std::string& key, bool value)
{
	char ch= value ? 'Y': 'N';
	m_data[ str_trim(str_toupper(key.c_str())) ] = std::string(1,ch);
}