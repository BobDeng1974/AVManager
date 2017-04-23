#include "stdafx.h"
#include "Settings.h"
#include "SessionSettings.h"


SessionSettings::SessionSettings( const std::string& file )
{
	std::ifstream fstream( file.c_str() );
	if ( fstream.is_open() )	
		fstream >> *this;
}

std::istream& operator>>( std::istream& stream, SessionSettings& s )
{
	Settings settings;
	stream >> settings;

	Settings::Sections section;
	section = settings.GetSections();
	Dictionary dict;

	for ( unsigned i = 0; i < section.size(); ++i )
	{
		dict = section[ i ];
		s.set( dict.GetName() , dict );
	}
	return stream;
}

std::ostream& operator<<( std::ostream& stream, const SessionSettings& s )
{
	return stream;
}

const bool SessionSettings::has( const std::string& sessionID ) const
{
	return m_settings.find( sessionID ) != m_settings.end();
}

const Dictionary& SessionSettings::get( const std::string& sessionID )
{
	Dictionaries::const_iterator i;
	i = m_settings.find( sessionID );
	if ( i != m_settings.end() )
		return i->second;
}

void SessionSettings::set( const std::string& sessionID, Dictionary settings )					
{
	m_settings[ sessionID ] = settings;
}

std::set < std::string > SessionSettings::getSessions() const
{
	std::set < std::string > result;
	Dictionaries::const_iterator i;
	for ( i = m_settings.begin(); i != m_settings.end(); ++i )
		result.insert( i->first );
	return result;
}