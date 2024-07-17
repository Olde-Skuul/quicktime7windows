#include "general.h"

#include <string>
#include <vector>
using namespace std;

namespace stringtools {
vector<string> split(const string& instring, const string& delim)
{
	vector<string> returnvector;
	int counter = 0;
	int start = 0;
	int offset = instring.find(delim);

	if (offset == string::npos) {
		return returnvector;
	}
	while (offset != string::npos) {
		returnvector.push_back(instring.substr(start, offset - start));
		start = offset + 1;
		offset = instring.find(delim, start);
		++counter;
		if (offset == string::npos) {

			returnvector.push_back(instring.substr(start, instring.size()));
		}
	}

	return returnvector;
}
}