#include "Gopher.h"
#include "ConfigFile.h"

void inputKeyboard(WORD cmd, DWORD flag)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = cmd;
	input.ki.dwFlags = flag;
	SendInput(1, &input, sizeof(INPUT));
}

void inputKeyboardDown(WORD cmd)
{
	inputKeyboard(cmd, 0);
}

void inputKeyboardUp(WORD cmd)
{
	inputKeyboard(cmd, KEYEVENTF_KEYUP);
}

void mouseEvent(DWORD dwFlags, DWORD mouseData = 0)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.dwFlags = dwFlags;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));
}

Gopher::Gopher(CXBOXController * controller)
	: _controller(controller)
{
}

void Gopher::loadConfigFile()
{
	ConfigFile cfg("config.ini");
	
	//Configuration bindings
	CONFIG_MOUSE_LEFT = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_LEFT").c_str(), 0, 0);
	CONFIG_MOUSE_RIGHT = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_RIGHT").c_str(), 0, 0);
	CONFIG_MOUSE_MIDDLE = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_MIDDLE").c_str(), 0, 0);
	CONFIG_HIDE = strtol(cfg.getValueOfKey<std::string>("CONFIG_HIDE").c_str(), 0, 0);
	CONFIG_DISABLE = strtol(cfg.getValueOfKey<std::string>("CONFIG_DISABLE").c_str(), 0, 0);
	CONFIG_DISABLE_VIBRATION = strtol(cfg.getValueOfKey<std::string>("CONFIG_DISABLE_VIBRATION").c_str(), 0, 0);
	CONFIG_SPEED_CHANGE = strtol(cfg.getValueOfKey<std::string>("CONFIG_SPEED_CHANGE").c_str(), 0, 0);

	//Controller bindings
	GAMEPAD_DPAD_UP = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_UP").c_str(), 0, 0);
	GAMEPAD_DPAD_DOWN = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_DOWN").c_str(), 0, 0);
	GAMEPAD_DPAD_LEFT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_LEFT").c_str(), 0, 0);
	GAMEPAD_DPAD_RIGHT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_RIGHT").c_str(), 0, 0);
	GAMEPAD_START = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_START").c_str(), 0, 0);
	GAMEPAD_BACK = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_BACK").c_str(), 0, 0);
	GAMEPAD_LEFT_THUMB = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_THUMB").c_str(), 0, 0);
	GAMEPAD_RIGHT_THUMB = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_THUMB").c_str(), 0, 0);
	GAMEPAD_LEFT_SHOULDER = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_SHOULDER").c_str(), 0, 0);
	GAMEPAD_RIGHT_SHOULDER = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_SHOULDER").c_str(), 0, 0);
	GAMEPAD_A = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_A").c_str(), 0, 0);
	GAMEPAD_B = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_B").c_str(), 0, 0);
	GAMEPAD_X = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_X").c_str(), 0, 0);
	GAMEPAD_Y = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_Y").c_str(), 0, 0);
	GAMEPAD_TRIGGER_LEFT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_LEFT").c_str(), 0, 0);
	GAMEPAD_TRIGGER_RIGHT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_RIGHT").c_str(), 0, 0);

	//Set Initial States
	setWindowVisibility(_hidden, false);

	//LOOP over all the other keys
}

void Gopher::loop() {
	Sleep(SLEEP_AMOUNT);

	_currentState = _controller->GetState();

	handleDisableButton();
	if (_disabled)
	{
		return;
	}

	if (CONFIG_DISABLE_VIBRATION)
		_vibrationDisabled = !_vibrationDisabled;

	handleMouseMovement();
	handleScrolling();

	//Mouse functions
	if (CONFIG_MOUSE_LEFT)
		mapMouseClick(CONFIG_MOUSE_LEFT, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
	if (CONFIG_MOUSE_RIGHT)
		mapMouseClick(CONFIG_MOUSE_RIGHT, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
	if (CONFIG_MOUSE_MIDDLE)
		mapMouseClick(CONFIG_MOUSE_MIDDLE, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP);

	//Hides the console
	if (CONFIG_HIDE)
	{
		setXboxClickState(CONFIG_HIDE);
		if (_xboxClickIsDown[CONFIG_HIDE])
		{
			toggleWindowVisibility();
		}
	}

	//Will change between the current speed values
	setXboxClickState(CONFIG_SPEED_CHANGE);
	if (_xboxClickIsDown[CONFIG_SPEED_CHANGE]) {

		if (speed == SPEED_LOW)
		{
			speed = SPEED_MED;
			pulseVibrate(400, 0, 3000);
		}
		else if (speed == SPEED_MED)
		{
			speed = SPEED_HIGH;
			pulseVibrate(400, 0, 65000);
		}
		else if (speed == SPEED_HIGH)
		{
			speed = SPEED_LOW;
			pulseVibrate(400, 0, 10000);
		}
	}

	//Set all controller keys.
	handleTriggers(GAMEPAD_TRIGGER_LEFT, GAMEPAD_TRIGGER_RIGHT);
	if (GAMEPAD_DPAD_UP)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_UP, GAMEPAD_DPAD_UP);
	if (GAMEPAD_DPAD_DOWN)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_DOWN, GAMEPAD_DPAD_DOWN);
	if (GAMEPAD_DPAD_LEFT)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_LEFT, GAMEPAD_DPAD_LEFT);
	if (GAMEPAD_DPAD_RIGHT)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_RIGHT, GAMEPAD_DPAD_RIGHT);
	if (GAMEPAD_START)
		mapKeyboard(XINPUT_GAMEPAD_START, GAMEPAD_START);
	if (GAMEPAD_BACK)
		mapKeyboard(XINPUT_GAMEPAD_BACK, GAMEPAD_BACK);
	if (GAMEPAD_LEFT_THUMB)
		mapKeyboard(XINPUT_GAMEPAD_LEFT_THUMB, GAMEPAD_LEFT_THUMB);
	if (GAMEPAD_RIGHT_THUMB)
		mapKeyboard(XINPUT_GAMEPAD_RIGHT_THUMB, GAMEPAD_RIGHT_THUMB);
	if (GAMEPAD_LEFT_SHOULDER)
		mapKeyboard(XINPUT_GAMEPAD_LEFT_SHOULDER, GAMEPAD_LEFT_SHOULDER);
	if (GAMEPAD_RIGHT_SHOULDER)
		mapKeyboard(XINPUT_GAMEPAD_RIGHT_SHOULDER, GAMEPAD_RIGHT_SHOULDER);
	if (GAMEPAD_A)
		mapKeyboard(XINPUT_GAMEPAD_A, GAMEPAD_A);
	if (GAMEPAD_B)
		mapKeyboard(XINPUT_GAMEPAD_B, GAMEPAD_B);
	if (GAMEPAD_X)
		mapKeyboard(XINPUT_GAMEPAD_X, GAMEPAD_X);
	if (GAMEPAD_Y)
		mapKeyboard(XINPUT_GAMEPAD_Y, GAMEPAD_Y);
}

void Gopher::pulseVibrate(const int duration, const int l, const int r) const
{
	if(!_vibrationDisabled)
	{
		_controller->Vibrate(l, r);
		Sleep(duration);
		_controller->Vibrate(0, 0);
	}
}

void Gopher::handleDisableButton()
{
	//Select + Start will disable.
	setXboxClickState(CONFIG_DISABLE);
	if (_xboxClickIsDown[CONFIG_DISABLE])
	{
		_disabled = !_disabled;

		if (_disabled) {
			pulseVibrate(400, 10000, 10000);
		}
		else {
			pulseVibrate(400, 65000, 65000);
		}
	}
}

void Gopher::toggleWindowVisibility()
{
	_hidden = !_hidden;
	setWindowVisibility(_hidden, true);
}

void Gopher::setWindowVisibility(const bool &hidden, const bool& verbose) const
{
	if (_hidden)
	{
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);
		if(verbose)
			printf("Window hidden\n");
	}
	else
	{
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_SHOW);
		if (verbose)
			printf("Window unhidden\n");
	}
}

template <typename T>
int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

float Gopher::getDelta(short t)
{
	//filter non-32768 and 32767, wireless ones can glitch sometimes and send it to the edge of the screen, it'll toss out some HUGE integer even when it's centered
	if (t > 32767) t = 0;
	if (t < -32768) t = 0;

	float delta = 0.0;

	if (abs(t) > DEAD_ZONE)
	{
		t = sgn(t) * (abs(t) - DEAD_ZONE);
		delta = speed * t / FPS;
	}

	return delta;
}

void Gopher::handleMouseMovement()
{
	POINT cursor;
	GetCursorPos(&cursor);

	short tx = _currentState.Gamepad.sThumbLX;
	short ty = _currentState.Gamepad.sThumbLY;

	float x = cursor.x + _xRest;
	float y = cursor.y + _yRest;

	float dx = getDelta(tx);
	float dy = getDelta(ty);

	x += dx;
	_xRest = x - (float)((int)x);

	y -= dy;
	_yRest = y - (float)((int)y);

	SetCursorPos((int)x, (int)y); //after all click input processing
}

void Gopher::handleScrolling()
{
	bool holdScrollUp = _currentState.Gamepad.sThumbRY > SCROLL_DEAD_ZONE;
	bool holdScrollDown = _currentState.Gamepad.sThumbRY < -SCROLL_DEAD_ZONE;

	if (holdScrollUp)
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = SCROLL_SPEED;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		input.mi.time = 0;
		SendInput(1, &input, sizeof(INPUT));
	}

	if (holdScrollDown)
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = -SCROLL_SPEED;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		input.mi.time = 0;
		SendInput(1, &input, sizeof(INPUT));
	}
}

void Gopher::handleTriggers(WORD lKey, WORD rKey)
{
	bool lTriggerIsDown = _currentState.Gamepad.bLeftTrigger > TRIGGER_DEAD_ZONE;
	bool rTriggerIsDown = _currentState.Gamepad.bRightTrigger > TRIGGER_DEAD_ZONE;


	if (lTriggerIsDown != _lTriggerPrevious)
	{
		_lTriggerPrevious = lTriggerIsDown;
		if (lTriggerIsDown)
		{
			inputKeyboardDown(lKey);
		}
		else
		{
			inputKeyboardUp(lKey);
		}
	}

	if (rTriggerIsDown != _rTriggerPrevious)
	{
		_rTriggerPrevious = rTriggerIsDown;
		if (rTriggerIsDown)
		{
			inputKeyboardDown(rKey);
		}
		else
		{
			inputKeyboardUp(rKey);
		}
	}
}

void Gopher::setXboxClickState(DWORD STATE)
{
	_xboxClickIsDown[STATE] = false;
	_xboxClickIsUp[STATE] = false;

	if (!this->xboxClickStateExists(STATE))
	{
		_xboxClickStateLastIteration[STATE] = false;
	}

	bool isDown = _currentState.Gamepad.wButtons == STATE;

	if (isDown && !_xboxClickStateLastIteration[STATE])
	{
		_xboxClickStateLastIteration[STATE] = true;
		_xboxClickIsDown[STATE] = true;
	}

	if (!isDown && _xboxClickStateLastIteration[STATE])
	{
		_xboxClickStateLastIteration[STATE] = false;
		_xboxClickIsUp[STATE] = true;
	}

	_xboxClickStateLastIteration[STATE] = isDown;
}

bool Gopher::xboxClickStateExists(DWORD xinput)
{
	auto it = _xboxClickStateLastIteration.find(xinput);
	if (it == _xboxClickStateLastIteration.end())
	{
		return false;
	}

	return true;
}

void Gopher::mapKeyboard(DWORD STATE, WORD key)
{
	setXboxClickState(STATE);
	if (_xboxClickIsDown[STATE])
	{
		inputKeyboardDown(key);
	}

	if (_xboxClickIsUp[STATE])
	{
		inputKeyboardUp(key);
	}
}

void Gopher::mapMouseClick(DWORD STATE, DWORD keyDown, DWORD keyUp)
{
	setXboxClickState(STATE);
	if (_xboxClickIsDown[STATE])
	{
		mouseEvent(keyDown);
	}

	if (_xboxClickIsUp[STATE])
	{
		mouseEvent(keyUp);
	}
}
