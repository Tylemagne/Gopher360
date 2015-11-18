#include <Windows.h> //for Beep()
#include <iostream>
#include <Xinput.h> //controller
#include <stdio.h> //for printf
#include <cmath> //for abs()
#include <mmdeviceapi.h> //vol
#include <endpointvolume.h> //vol

#include "CXBOXController.h"

#pragma once
class Gopher
{
private:
	const int deadZone = 3000; //X and Y minimum, below this is ignored since all controllers have some stick to them
	const int scrollDeadZone = 7000; // Right thumbstick should be less sensitive.
	const int scrollSpeed = 20; // Speed at which you scroll page.
	const float range = 4.0f; //4 gives a decent range. Raising this requires a lowering of speed as well.
	const int truncZone = 1; //anything below this is ignored and the mouse sits still, similar to a deadzone
	const int sleepAmount = 6; //ms to sleep. 16 = 60fps, 6 = 144fps

	XINPUT_STATE currentState;

	float speed_low = 0.000055f;
	float speed_med = 0.000085f;
	float speed_high = 0.000125f;
	float speed = speed_low; //multiplied by integer value of analog X and Y (32,000). NEEDS TO EQUAL ONE OF THE 3 SPEEDS.

	bool holdLeft; //instructed to hold
	bool holdingLeft; //is actually holding

	bool holdRight;
	bool holdingRight;

	bool holdY;
	bool holdingY;
	bool stillHoldingY; //to check if it's still being held from the last loop

	bool holdEnter;
	bool holdingEnter;

	bool holdBack;
	bool holdingBack;

	bool holdStart;
	bool holdingStart;

	bool holdLThumb;
	bool holdingLThumb;

	bool holdDUp;
	bool holdingDUp;

	bool holdDDown;
	bool holdingDDown;

	bool holdDLeft;
	bool holdingDLeft;

	bool holdDRight;
	bool holdingDRight;

	bool holdBLeft; //bumpers
	bool holdingBLeft;

	bool holdBRight;
	bool holdingBRight;

	bool disabled = false; //use for Select sleep mode
	bool hidden = false; //press Y to hide, check this var

public:
	CXBOXController* Controller; //begin declaring variables. TODO: Make them hold their control name rather than designated input (B instead of Enter)

	void loop();
	void hideWindow();

	void handleDPad();
	void handleMouse();

	void handleDisableButton();

	void handleScrolling();

};
