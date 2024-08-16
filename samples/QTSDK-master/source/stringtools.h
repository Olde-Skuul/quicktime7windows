/***************************************

	String helpers

***************************************/

#ifndef __STRINGTOOLS_H__
#define __STRINGTOOLS_H__

#include <string>
#include <vector>

namespace stringtools {
extern std::vector<std::string> split(
	const std::string& rInstring, const std::string& rDelim);
}

#endif
