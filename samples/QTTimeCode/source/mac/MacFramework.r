#include <Dialogs.r>

resource 'ALRT' (129, "Error") {
	{40, 40, 180, 340},
	129,
	{
		OK, visible, sound1,	// 4 items
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1
	},
	alertPositionMainScreen
};

resource 'DITL' (129) {
	{
		{110, 200, 130, 290},
		Button {
			enabled, "Continue"
		},
		{10, 10, 100, 290},
		StaticText {
			disabled, "                ^0\n\n^1  ^2\n"
		}
	}
};
