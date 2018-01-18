#include <windows.h> //for Beep()
#include <iostream>
#include <vector>
#include <xinput.h> //controller
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
	int DEAD_ZONE = 6000; //X and Y minimum, below this is ignored since all controllers have some stick to them
	int SCROLL_DEAD_ZONE = 5000; // Right thumbstick should be less sensitive.
	const int TRIGGER_DEAD_ZONE = 0;
	float SCROLL_SPEED = 0.1; // Speed at which you scroll page.
	const int FPS = 150;
	const int SLEEP_AMOUNT = 1000/FPS; // number of milliseconds to sleep per iteration

	XINPUT_STATE _currentState;

	const float SPEED_ULTRALOW = 0.005f;
	const float SPEED_LOW = 0.015f;
	const float SPEED_MED = 0.025f;
	const float SPEED_HIGH = 0.04f;
	float speed = SPEED_MED;
	float acceleration_factor = 0.0f;

	float _xRest = 0.0f;
	float _yRest = 0.0f;

	bool _disabled = false; //use for Select sleep mode
	bool _vibrationDisabled = false; //use for Select sleep mode
	bool _hidden = false; //press Y to hide, check this var
	bool _lTriggerPrevious = false;
	bool _rTriggerPrevious = false;
	int _lTriggerLongPressCount = 0;
	int _rTriggerLongPressCount = 0;
	int _lTriggerLongPressMax = 500;		// number of milliseconds until next pulsing of the key - automatically decreased with each press
	int _rTriggerLongPressMax = 500;

	std::vector<float> speeds;	// contains actual speeds to choose
	std::vector<std::string> speed_names; // contains pretty names of speeds to display
	int speed_idx = 0;

	//Mouse Clicks
	DWORD CONFIG_MOUSE_LEFT = NULL;
	DWORD CONFIG_MOUSE_RIGHT = NULL;
	DWORD CONFIG_MOUSE_MIDDLE = NULL;
	
	//Gopher Settings
	DWORD CONFIG_HIDE = NULL;
	DWORD CONFIG_DISABLE = NULL;
	DWORD CONFIG_DISABLE_VIBRATION = NULL;
	DWORD CONFIG_SPEED_CHANGE = NULL;

	//Gamepad bindings
	DWORD GAMEPAD_DPAD_UP = NULL;
	DWORD GAMEPAD_DPAD_DOWN = NULL;
	DWORD GAMEPAD_DPAD_LEFT = NULL;
	DWORD GAMEPAD_DPAD_RIGHT = NULL;
	DWORD GAMEPAD_START = NULL;
	DWORD GAMEPAD_BACK = NULL;
	DWORD GAMEPAD_LEFT_THUMB = NULL;
	DWORD GAMEPAD_RIGHT_THUMB = NULL;
	DWORD GAMEPAD_LEFT_SHOULDER = NULL;
	DWORD GAMEPAD_RIGHT_SHOULDER = NULL;
	DWORD GAMEPAD_A = NULL;
	DWORD GAMEPAD_B = NULL;
	DWORD GAMEPAD_X = NULL;
	DWORD GAMEPAD_Y = NULL;
	DWORD GAMEPAD_TRIGGER_LEFT = NULL;
	DWORD GAMEPAD_TRIGGER_RIGHT = NULL;

	// Key repeats (autofire)
	int GAMEPAD_DPAD_UP_REPEAT = 0;
	int GAMEPAD_DPAD_DOWN_REPEAT = 0;
	int GAMEPAD_DPAD_LEFT_REPEAT = 0;
	int GAMEPAD_DPAD_RIGHT_REPEAT = 0;
	int GAMEPAD_START_REPEAT = 0;
	int GAMEPAD_BACK_REPEAT = 0;
	int GAMEPAD_LEFT_THUMB_REPEAT = 0;
	int GAMEPAD_RIGHT_THUMB_REPEAT = 0;
	int GAMEPAD_LEFT_SHOULDER_REPEAT = 0;
	int GAMEPAD_RIGHT_SHOULDER_REPEAT = 0;
	int GAMEPAD_A_REPEAT = 0;
	int GAMEPAD_B_REPEAT = 0;
	int GAMEPAD_X_REPEAT = 0;
	int GAMEPAD_Y_REPEAT = 0;
	int GAMEPAD_TRIGGER_LEFT_REPEAT = 0;
	int GAMEPAD_TRIGGER_RIGHT_REPEAT = 0;

	std::map<DWORD, bool> _xboxClickStateLastIteration;
	std::map<DWORD, bool> _xboxClickIsDown;
	std::map<DWORD, bool> _xboxClickIsDownLong;
	std::map<DWORD, int> _xboxClickDownLength;
	std::map<DWORD, int> _xboxClickDownLongMax;
	std::map<DWORD, bool> _xboxClickIsUp;

	CXBOXController* _controller;

public:

	Gopher(CXBOXController* controller);

	void loadConfigFile();

	void loop();

	void pulseVibrate(const int duration, const int l, const int r) const;

	void toggleWindowVisibility();

	void setWindowVisibility(const bool& hidden) const;

	float getDelta(short tx);

	float getMult(float length, float deadzone, float accel);

	void handleMouseMovement();

	void handleDisableButton();

	void handleVibrationButton();

	void handleScrolling();

	void handleTriggers(WORD lKey, WORD rKey);

	bool xboxClickStateExists(DWORD xinput);

	void mapKeyboard(DWORD STATE, WORD key, int repeat);

	void mapMouseClick(DWORD STATE, DWORD keyDown, DWORD keyUp);

	void setXboxClickState(DWORD state);
};
