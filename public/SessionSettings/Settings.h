#ifndef SETTINGS_H_
#define SETTINGS_H_


#include "Dictionary.h"
#include "c_string.h"

using namespace std;
using namespace utility;

class Settings
{
public:
	typedef std::vector < Dictionary > Sections;

	Sections GetSections() const;

	friend std::istream& operator>>( std::istream&, Settings& );
private:
	Sections m_sections;
};

std::istream& operator>>( std::istream&, Settings& );


#endif // !SETTINGS_H_
