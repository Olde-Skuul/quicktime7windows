/***************************************

	String helpers

***************************************/

#include "stringtools.h"

using namespace std;

namespace stringtools {

/*! ************************************

	\brief Split a string using a delimiter

	Iterate over the string for a delimiter match. Create a
	vector of strings that are separated by this delimiter

	\note If the delimiter is not present in the string, return
		an empty vector

	\param rInstring String to slice up
	\param rDelim String to use as a delimiter

	\return vector of strings that are seperated by delimiter

***************************************/

vector<string> split(const string& rInstring, const string& rDelim)
{
	vector<string> returnvector;

	int offset = (int)rInstring.find(rDelim);

	if (offset == string::npos) {
		return returnvector;
	}

	int start = 0;
	while (offset != string::npos) {
		returnvector.push_back(rInstring.substr(
			(string::size_type)start, (string::size_type)(offset - start)));
		start = offset + 1;
		offset = (int)rInstring.find(rDelim, (string::size_type)start);

		if (offset == string::npos) {
			returnvector.push_back(
				rInstring.substr((string::size_type)start, rInstring.size()));
		}
	}

	return returnvector;
}

}