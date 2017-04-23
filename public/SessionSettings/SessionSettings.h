#ifndef SESSION_SETTINGS_H_
#define SESSION_SETTINGS_H_

#include "Dictionary.h"
#include <map>
#include <set>
#include <string>
#include <fstream>


class SessionSettings
{
public:
	SessionSettings() {}
	SessionSettings( const std::string& file );

	const bool has( const std::string& ) const;
	const Dictionary& get( const std::string& );
	void set( const std::string&, Dictionary );

	size_t size() const { return m_settings.size(); }

	typedef std::map < std::string, Dictionary > Dictionaries;
	std::set < std::string > getSessions() const;

private:
	Dictionaries m_settings;
	std::string	 m_sErrMsg;
	friend std::ostream& operator<<( std::ostream&, const SessionSettings& );
};


std::istream& operator>>( std::istream&, SessionSettings& );
std::ostream& operator<<( std::ostream&, const SessionSettings& );



#endif