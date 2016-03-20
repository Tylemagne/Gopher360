#include "Gopher.h"

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

void mouseEvent(DWORD dwFlags, DWORD mouseData=0)
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

void Gopher::loop() {
	Sleep(SLEEP_AMOUNT);

	_currentState = _controller->GetState();
	
	handleDisableButton();

	if (_disabled)
	{
		return;
	}

	handleMouseMovement();
	handleScrolling();
	handleTriggers(VK_SPACE,VK_BACK);

	mapMouseClick(XINPUT_GAMEPAD_A, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
	mapMouseClick(XINPUT_GAMEPAD_X, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
	mapMouseClick(XINPUT_GAMEPAD_LEFT_THUMB, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP);

	mapKeyboard(XINPUT_GAMEPAD_DPAD_UP, VK_UP);
	mapKeyboard(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN);
	mapKeyboard(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT);
	mapKeyboard(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT);

	setXboxClickState(XINPUT_GAMEPAD_Y);
	if (_xboxClickIsDown[XINPUT_GAMEPAD_Y])
	{
		toggleWindowVisibility();
	}

	mapKeyboard(XINPUT_GAMEPAD_START, VK_LWIN);
	mapKeyboard(XINPUT_GAMEPAD_B, VK_RETURN);
	mapKeyboard(XINPUT_GAMEPAD_RIGHT_SHOULDER, VK_BROWSER_FORWARD);
	mapKeyboard(XINPUT_GAMEPAD_LEFT_SHOULDER, VK_BROWSER_BACK);
	mapKeyboard(XINPUT_GAMEPAD_BACK, VK_BROWSER_REFRESH);

	//Left and Right Shoulders will change speed.
	setXboxClickState(XINPUT_GAMEPAD_LEFT_SHOULDER | XINPUT_GAMEPAD_RIGHT_SHOULDER);
	if (_xboxClickIsDown[XINPUT_GAMEPAD_LEFT_SHOULDER | XINPUT_GAMEPAD_RIGHT_SHOULDER]) {

		if (speed == SPEED_LOW)
		{
			speed = SPEED_MED;
			_controller->Vibrate(0, 30000);
			Sleep(400);
			_controller->Vibrate(0, 0);
		}
		else if (speed == SPEED_MED)
		{
			speed = SPEED_HIGH;
			_controller->Vibrate(0, 65000);
			Sleep(400);
			_controller->Vibrate(0, 0);
		}
		else if (speed == SPEED_HIGH)
		{
			speed = SPEED_LOW;
			_controller->Vibrate(0, 10000);
			Sleep(400);
			_controller->Vibrate(0, 0);
		}
	}
}

void Gopher::handleDisableButton()
{
	//Select + Start will disable.
    setXboxClickState(XINPUT_GAMEPAD_BACK | XINPUT_GAMEPAD_START);
	if (_xboxClickIsDown[XINPUT_GAMEPAD_BACK | XINPUT_GAMEPAD_START])
	{
		_disabled = !_disabled;

		if (_disabled) {
			_controller->Vibrate(10000, 10000);
			Sleep(400);
			_controller->Vibrate(0, 0);
		}
		else {
			_controller->Vibrate(65000, 65000);
			Sleep(400);
			_controller->Vibrate(0, 0);
		}
	}
}

void Gopher::toggleWindowVisibility()
{
	_hidden = !_hidden;

	if (_hidden)
	{
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);
		printf("Window hidden\n");
	}
	else
	{
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_SHOW);
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
	

	if(lTriggerIsDown != _lTriggerPrevious)
	{
		_lTriggerPrevious = lTriggerIsDown;
		if (lTriggerIsDown)
		{
 			inputKeyboardDown(lKey);
		}else
		{
			inputKeyboardUp(lKey);
		}
	}

	if(rTriggerIsDown != _rTriggerPrevious)
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
