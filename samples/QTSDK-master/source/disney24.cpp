/***************************************

	Constants for Disney Quicktime movies

***************************************/

#include "disney24.h"

// Needed for std::out_of_range
#if defined(__WATCOMC__)
// Darn 8.3 filenames
#include <stdexcep>
#else
#include <stdexcept>
#endif

// Mono is 42

namespace Disney {
static const int gConfigs[][2] = { //
	{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 1}, {8, 2}, {9, 1},
	{10, 2}, {11, 3}, {12, 4}, {13, 5}, {14, 6}, {15, 1}, {16, 2}, {17, 1},
	{18, 2}, {19, 1}, {20, 2}, {21, 1}, {22, 2}, {23, 42}, {24, 42}};

/*! ************************************

	\brief Convert an index to a Disney config value

	42 is returned as the mono audio channel

	\param iIndex Value from 1 to 24

	\return Return the channel configuration value

	\exception std::out_of_range if iIndex isn't 1-24

***************************************/

int Disney24ChConfig(int iIndex)
{
	int i = 0;
	do {
		if (gConfigs[i][0] == iIndex) {
			return gConfigs[i][1];
		}
	} while (++i < (sizeof(gConfigs) / sizeof(gConfigs[0])));
	throw std::out_of_range("Disney24ChConfig out of range");
}

}
