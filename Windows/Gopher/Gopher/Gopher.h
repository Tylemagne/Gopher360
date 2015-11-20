#include <Windows.h> //for Beep()
#include <iostream>
#include <Xinput.h> //controller
#include <stdio.h> //for printf
#include <cmath> //for abs()
#include <mmdeviceapi.h> //vol
#include <endpointvolume.h> //vol

#include <map>

#include "CXBOXController.h"

#pragma once
class Gopher
{
private:
	const int DEAD_ZONE = 3000; //X and Y minimum, below this is ignored since all controllers have some stick to them
	const int SCROLL_DEAD_ZONE = 7000; // Right thumbstick should be less sensitive.
	const int SCROLL_SPEED = 20; // Speed at which you scroll page.
	const int TRUNC_ZONE = 5000; //anything below this is ignored and the mouse sits still, similar to a deadzone
	const int SLEEP_AMOUNT = 6; //ms to sleep. 16 = 60fps, 6 = 144fps
	const float RANGE = 4.0f; //4 gives a decent range. Raising this requires a lowering of speed as well.

	XINPUT_STATE _currentState;

	const float SPEED_LOW = 0.000055f;
	const float SPEED_MED = 0.000075f;
	const float SPEED_HIGH = 0.000125f;
	float speed = SPEED_MED;

	float _xRest = 0.0f;
	float _yRest = 0.0f;

	bool _disabled = false; //use for Select sleep mode
	bool _hidden = false; //press Y to hide, check this var

	std::map<DWORD, bool> _xboxClickStateLastIteration;
	std::map<DWORD, bool> _xboxClickIsDown;
	std::map<DWORD, bool> _xboxClickIsUp;

	CXBOXController* _controller;

public:

	Gopher(CXBOXController* controller);

	void loop();

	void toggleWindowVisibility();

	float getDelta(short tx);

	void handleMouseMovement();

	void handleDisableButton();

	void handleScrolling();

	bool xboxClickStateExists(DWORD xinput);

	void mapKeyboard(DWORD STATE, DWORD key);

	void mapMouseClick(DWORD STATE, DWORD keyDown, DWORD keyUp);

	void setXboxClickState(DWORD state);
};
