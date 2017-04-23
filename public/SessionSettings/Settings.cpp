#include "stdafx.h"
#include "Settings.h"

bool isComment( const std::string& line )
{
	if( line.size() == 0 )
		return false;

	return line[0] == '#';
}

bool isSection( const std::string& line )
{
	if( line.size() == 0 )
		return false;

	return line[0] == '[' && line[line.size()-1] == ']';
}

std::string splitSection( const std::string& line )
{
	return str_trim(std::string( line, 1, line.size() - 2 ));
}

bool isKeyValue( const std::string& line )
{
	return line.find( '=' ) != std::string::npos;
}

std::pair<std::string, std::string> splitKeyValue( const std::string& line )
{
	size_t equals = line.find( '=' );
	std::string key = std::string( line, 0, equals );
	std::string value = std::string( line, equals + 1, std::string::npos );
	return std::pair<std::string, std::string>( key, value );
}

std::istream& operator>>( std::istream& stream, Settings& s )
{
	char buffer[1024];
	std::string line;
	Settings::Sections::iterator section = s.m_sections.end();;

	while( stream.getline(buffer, 1024) )
	{
		if (strlen(buffer) == 0)
		{
			continue;
		}

		line = str_trim( buffer );
		if( isComment(line) )
		{
			continue;
		}
		else if( isSection(line) )
		{
			section = s.m_sections.insert( s.m_sections.end(), Dictionary(splitSection(line)) );
		}
		else if( isKeyValue(line) )
		{
			std::pair<std::string, std::string> keyValue = splitKeyValue( line );
			if( section == s.m_sections.end() )
				continue;
			(*section).SetString( keyValue.first, keyValue.second );
		}
	}
	return stream;
}

Settings::Sections Settings::GetSections() const
{
	return m_sections;
}