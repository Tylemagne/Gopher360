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
	const int DEAD_ZONE = 9000; //X and Y minimum, below this is ignored since all controllers have some stick to them
	const int SCROLL_DEAD_ZONE = 9000; // Right thumbstick should be less sensitive.
	const int TRIGGER_DEAD_ZONE = 0;
	const int SCROLL_SPEED = 20; // Speed at which you scroll page.
	const int FPS = 150;
	const int SLEEP_AMOUNT = 1000/FPS; // number of milliseconds to sleep per iteration

	XINPUT_STATE _currentState;

	const float SPEED_LOW = 0.032f;
	const float SPEED_MED = 0.04f;
	const float SPEED_HIGH = 0.072f;
	float speed = SPEED_MED;

	float _xRest = 0.0f;
	float _yRest = 0.0f;

	bool _disabled = false; //use for Select sleep mode
	bool _hidden = false; //press Y to hide, check this var
	bool _lTriggerPrevious = false;
	bool _rTriggerPrevious = false;

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

	void handleTriggers(WORD lKey, WORD rKey);

	bool xboxClickStateExists(DWORD xinput);

	void mapKeyboard(DWORD STATE, WORD key);

	void mapMouseClick(DWORD STATE, DWORD keyDown, DWORD keyUp);

	void setXboxClickState(DWORD state);
};
