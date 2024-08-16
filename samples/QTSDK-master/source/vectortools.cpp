/***************************************

	Vector helpers

***************************************/

#include "vectortools.h"

// Love those 8.3 filenames
#if defined(__WATCOMC__)
#include <algorith>
#endif

using namespace std;

namespace vectortools {

/*! ************************************

	\brief Test if a string is in a vector of strings

	The comparison is case sensitive

	\param rStrings Reference to a vector of string
	\param rMatch Reference to string to match

	\return true if present, false if not

***************************************/

bool contains(const vector<string>& rStrings, const string& rMatch)
{
	// Simple, eh?
	return (
		std::find(rStrings.begin(), rStrings.end(), rMatch) != rStrings.end());
}

/*! ************************************

	\brief Test if a string is in a map of strings

	The comparison is case sensitive

	\param rMap Reference to a vector of string
	\param rMatch Reference to string to match

	\return true if present, false if not

***************************************/

bool contains(const commands& rMap, const std::string& rMatch)
{
	// Some compilers want rMap to not be const
	// go figure
	return const_cast<commands&>(rMap).find(rMatch) != rMap.end();
}

}
