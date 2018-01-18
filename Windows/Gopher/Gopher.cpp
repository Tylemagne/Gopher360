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

	//Whether to enable the automatic repeat (autofire) functionality for each button
	GAMEPAD_DPAD_UP_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_UP_REPEAT").c_str(), 0, 0);
	GAMEPAD_DPAD_DOWN_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_DOWN_REPEAT").c_str(), 0, 0);
	GAMEPAD_DPAD_LEFT_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_LEFT_REPEAT").c_str(), 0, 0);
	GAMEPAD_DPAD_RIGHT_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_RIGHT_REPEAT").c_str(), 0, 0);
	GAMEPAD_START_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_START_REPEAT").c_str(), 0, 0);
	GAMEPAD_BACK_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_BACK_REPEAT").c_str(), 0, 0);
	GAMEPAD_LEFT_THUMB_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_THUMB_REPEAT").c_str(), 0, 0);
	GAMEPAD_RIGHT_THUMB_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_THUMB_REPEAT").c_str(), 0, 0);
	GAMEPAD_LEFT_SHOULDER_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_SHOULDER_REPEAT").c_str(), 0, 0);
	GAMEPAD_RIGHT_SHOULDER_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_SHOULDER_REPEAT").c_str(), 0, 0);
	GAMEPAD_A_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_A_REPEAT").c_str(), 0, 0);
	GAMEPAD_B_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_B_REPEAT").c_str(), 0, 0);
	GAMEPAD_X_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_X_REPEAT").c_str(), 0, 0);
	GAMEPAD_Y_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_Y_REPEAT").c_str(), 0, 0);
	GAMEPAD_TRIGGER_LEFT_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_LEFT_REPEAT").c_str(), 0, 0);
	GAMEPAD_TRIGGER_RIGHT_REPEAT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_RIGHT_REPEAT").c_str(), 0, 0);

	// Acceleration factor
	acceleration_factor = strtof(cfg.getValueOfKey<std::string>("ACCELERATION_FACTOR").c_str(), 0);

	//Dead zones
	DEAD_ZONE = strtol(cfg.getValueOfKey<std::string>("DEAD_ZONE").c_str(), 0, 0);
	if (DEAD_ZONE == 0)
		DEAD_ZONE = 6000;
	SCROLL_DEAD_ZONE = strtol(cfg.getValueOfKey<std::string>("SCROLL_DEAD_ZONE").c_str(), 0, 0);
	if (SCROLL_DEAD_ZONE == 0)
		SCROLL_DEAD_ZONE = 5000;
	SCROLL_SPEED = strtof(cfg.getValueOfKey<std::string>("SCROLL_SPEED").c_str(), 0);
	if (SCROLL_SPEED < 0.00001f)
		SCROLL_SPEED = 0.1f;

	//Speeds
	std::istringstream cursor_speed = std::istringstream(cfg.getValueOfKey<std::string>("CURSOR_SPEED"));
	int cur_speed_idx = 1;
	for (std::string cur_speed; std::getline(cursor_speed, cur_speed, ',');)
	{
		std::istringstream cursor_speed_entry = std::istringstream(cur_speed);
		std::string cur_name, cur_speed_s;
		if (cur_speed.find_first_of('=') != std::string::npos)
		{
			std::getline(cursor_speed_entry, cur_name, '=');
		}
		else
		{
			std::ostringstream tmp_name;
			tmp_name << cur_speed_idx++;
			cur_name = tmp_name.str();
		}
		std::getline(cursor_speed_entry, cur_speed_s);
		float cur_speedf = strtof(cur_speed_s.c_str(), 0);
		if (cur_speedf > 0.0001f && cur_speedf <= 1.0f)
		{
			speeds.push_back(cur_speedf);
			speed_names.push_back(cur_name);
		}
	}
	if (speeds.size() == 0)
	{
		speeds.push_back(0.005f);
		speeds.push_back(0.015f);
		speeds.push_back(0.025f);
		speeds.push_back(0.004f);
		speed_names.push_back("ULTRALOW");
		speed_names.push_back("LOW");
		speed_names.push_back("MED");
		speed_names.push_back("HIGH");
	}
	speed = speeds[0];

	//Set Initial States
	setWindowVisibility(_hidden);

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

	//Vibration
	handleVibrationButton();

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
		speed_idx++;
		if (speed_idx >= speeds.size())
			speed_idx = 0;
		speed = speeds[speed_idx];
		printf("Setting speed to %f (%s)...\n", speed, speed_names[speed_idx].c_str());
	}

	//Set all controller keys.
	handleTriggers(GAMEPAD_TRIGGER_LEFT, GAMEPAD_TRIGGER_RIGHT);
	if (GAMEPAD_DPAD_UP)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_UP, GAMEPAD_DPAD_UP, GAMEPAD_DPAD_UP_REPEAT);
	if (GAMEPAD_DPAD_DOWN)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_DOWN, GAMEPAD_DPAD_DOWN, GAMEPAD_DPAD_DOWN_REPEAT);
	if (GAMEPAD_DPAD_LEFT)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_LEFT, GAMEPAD_DPAD_LEFT, GAMEPAD_DPAD_LEFT_REPEAT);
	if (GAMEPAD_DPAD_RIGHT)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_RIGHT, GAMEPAD_DPAD_RIGHT, GAMEPAD_DPAD_RIGHT_REPEAT);
	if (GAMEPAD_START)
		mapKeyboard(XINPUT_GAMEPAD_START, GAMEPAD_START, GAMEPAD_START_REPEAT);
	if (GAMEPAD_BACK)
		mapKeyboard(XINPUT_GAMEPAD_BACK, GAMEPAD_BACK, GAMEPAD_BACK_REPEAT);
	if (GAMEPAD_LEFT_THUMB)
		mapKeyboard(XINPUT_GAMEPAD_LEFT_THUMB, GAMEPAD_LEFT_THUMB, GAMEPAD_LEFT_THUMB_REPEAT);
	if (GAMEPAD_RIGHT_THUMB)
		mapKeyboard(XINPUT_GAMEPAD_RIGHT_THUMB, GAMEPAD_RIGHT_THUMB, GAMEPAD_RIGHT_THUMB_REPEAT);
	if (GAMEPAD_LEFT_SHOULDER)
		mapKeyboard(XINPUT_GAMEPAD_LEFT_SHOULDER, GAMEPAD_LEFT_SHOULDER, GAMEPAD_LEFT_SHOULDER_REPEAT);
	if (GAMEPAD_RIGHT_SHOULDER)
		mapKeyboard(XINPUT_GAMEPAD_RIGHT_SHOULDER, GAMEPAD_RIGHT_SHOULDER, GAMEPAD_RIGHT_SHOULDER_REPEAT);
	if (GAMEPAD_A)
		mapKeyboard(XINPUT_GAMEPAD_A, GAMEPAD_A, GAMEPAD_A_REPEAT);
	if (GAMEPAD_B)
		mapKeyboard(XINPUT_GAMEPAD_B, GAMEPAD_B, GAMEPAD_B_REPEAT);
	if (GAMEPAD_X)
		mapKeyboard(XINPUT_GAMEPAD_X, GAMEPAD_X, GAMEPAD_X_REPEAT);
	if (GAMEPAD_Y)
		mapKeyboard(XINPUT_GAMEPAD_Y, GAMEPAD_Y, GAMEPAD_Y_REPEAT);
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

void Gopher::handleVibrationButton()
{
	//DPadUp + Start will disable.
	setXboxClickState(CONFIG_DISABLE_VIBRATION);
	if (_xboxClickIsDown[CONFIG_DISABLE_VIBRATION])
	{
		_vibrationDisabled = !_vibrationDisabled;
		printf("Vibration %s\n", _vibrationDisabled ? "Disabled" : "Enabled");
		Sleep(1000);
	}
}

void Gopher::toggleWindowVisibility()
{
	_hidden = !_hidden;
	printf("Window %s\n", _hidden ? "hidden" : "unhidden");
	setWindowVisibility(_hidden);
}

void Gopher::setWindowVisibility(const bool &hidden) const
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, _hidden ? SW_HIDE : SW_SHOW);
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

	return t;
}

float Gopher::getMult(float lengthsq, float deadzone, float accel)
{
	float mult = (sqrt(lengthsq) - deadzone) / (MAXSHORT - deadzone);
	if (accel > 0.0001f)
		mult = pow(mult, accel);
	return mult / FPS;
}

void Gopher::handleMouseMovement()
{
	POINT cursor;
	GetCursorPos(&cursor);

	short tx = _currentState.Gamepad.sThumbLX;
	short ty = _currentState.Gamepad.sThumbLY;

	float x = cursor.x + _xRest;
	float y = cursor.y + _yRest;

	float dx = 0;
	float dy = 0;

	float lengthsq = tx * tx + ty * ty;
	if (lengthsq > DEAD_ZONE * DEAD_ZONE)
	{
		float mult = speed * getMult(lengthsq, DEAD_ZONE, acceleration_factor);

		dx = getDelta(tx) * mult;
		dy = getDelta(ty) * mult;
	}

	x += dx;
	_xRest = x - (float)((int)x);

	y -= dy;
	_yRest = y - (float)((int)y);

	SetCursorPos((int)x, (int)y); //after all click input processing
}

void Gopher::handleScrolling()
{
	short tx = _currentState.Gamepad.sThumbRX;
	short ty = _currentState.Gamepad.sThumbRY;

	if ((ty * ty + tx * tx) > SCROLL_DEAD_ZONE * SCROLL_DEAD_ZONE)
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = getDelta(tx) * getMult(tx * tx, SCROLL_DEAD_ZONE, 0.0f) * SCROLL_SPEED;
		input.mi.dwFlags = MOUSEEVENTF_HWHEEL;
		input.mi.time = 0;
		SendInput(1, &input, sizeof(INPUT));
		input.mi.mouseData = getDelta(ty) * getMult(ty * ty, SCROLL_DEAD_ZONE, 0.0f) * SCROLL_SPEED;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
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
			_lTriggerLongPressCount = 0;
		}
		else
		{
			inputKeyboardUp(lKey);
			_lTriggerLongPressMax = 500;		// reset long press max trigger
		}
	}
	else if (GAMEPAD_TRIGGER_LEFT_REPEAT && lTriggerIsDown)
	{
		_lTriggerLongPressCount++;
		if (_lTriggerLongPressCount * SLEEP_AMOUNT > _lTriggerLongPressMax)
		{
			// pulse the key
			inputKeyboardUp(lKey);
			inputKeyboardDown(lKey);
			_lTriggerLongPressCount = 0;
			if (_lTriggerLongPressMax == 500)
				_lTriggerLongPressMax = 200;
			else
				_lTriggerLongPressMax = 70;
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
			_rTriggerLongPressMax = 500;
		}
	}
	else if (GAMEPAD_TRIGGER_RIGHT_REPEAT && rTriggerIsDown)
	{
		_rTriggerLongPressCount++;
		if (_rTriggerLongPressCount * SLEEP_AMOUNT > _rTriggerLongPressMax)
		{
			// pulse the key
			inputKeyboardUp(rKey);
			inputKeyboardDown(rKey);
			_rTriggerLongPressCount = 0;
			if (_rTriggerLongPressMax == 500)
				_rTriggerLongPressMax = 200;
			else
				_rTriggerLongPressMax = 70;
		}
	}
}

void Gopher::setXboxClickState(DWORD STATE)
{
	_xboxClickIsDown[STATE] = false;
	_xboxClickIsUp[STATE] = false;
	_xboxClickIsDownLong[STATE] = false;

	if (!this->xboxClickStateExists(STATE))
	{
		_xboxClickStateLastIteration[STATE] = false;
	}

	bool isDown = (_currentState.Gamepad.wButtons & STATE) == STATE;

	if (isDown && !_xboxClickStateLastIteration[STATE])
	{
		_xboxClickStateLastIteration[STATE] = true;
		_xboxClickIsDown[STATE] = true;
		_xboxClickDownLength[STATE] = 0;
		_xboxClickDownLongMax[STATE] = 500;
	}

	if (isDown && _xboxClickStateLastIteration[STATE])
	{
		// _xboxClickDownLong toggles every 200 ms of button depression
		_xboxClickDownLength[STATE] = _xboxClickDownLength[STATE] + 1;
		if (_xboxClickDownLength[STATE] * SLEEP_AMOUNT > _xboxClickDownLongMax[STATE])
		{
			_xboxClickIsDownLong[STATE] = true;
			_xboxClickDownLength[STATE] = 0;
			if (_xboxClickDownLongMax[STATE] == 500)
				_xboxClickDownLongMax[STATE] = 200;
			else
				_xboxClickDownLongMax[STATE] = 70;
		}
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

void Gopher::mapKeyboard(DWORD STATE, WORD key, int repeat)
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

	if (repeat && _xboxClickIsDownLong[STATE])
	{
		// pulse key off and on
		inputKeyboardUp(key);
		inputKeyboardDown(key);
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

	/*if (_xboxClickIsDownLong[STATE])
	{
		mouseEvent(keyDown | keyUp);
		mouseEvent(keyDown | keyUp);
	}*/
}
