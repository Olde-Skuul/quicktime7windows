/***************************************

	Vector helpers

***************************************/

#ifndef __VECTORTOOLS_H__
#define __VECTORTOOLS_H__

#include <map>
#include <string>
#include <vector>

// Commands to process
typedef std::map<std::string, std::string> commands;

namespace vectortools {
extern bool contains(
	const std::vector<std::string>& rStrings, const std::string& rMatch);
extern bool contains(const commands& inmap, const std::string& item);
}

#endif
