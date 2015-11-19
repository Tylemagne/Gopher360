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

void mouseEvent(WORD dwFlags, DWORD mouseData=0)
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

	setXboxClickState(XINPUT_GAMEPAD_LEFT_SHOULDER);


	if (_xboxClickIsDown[XINPUT_GAMEPAD_LEFT_SHOULDER]) {

		if (speed == SPEED_LOW)
		{
			Beep(240, 210);
			speed = SPEED_MED;
		}
		else if (speed == SPEED_MED)
		{
			Beep(260, 210);
			speed = SPEED_HIGH;
		}
		else if (speed == SPEED_HIGH)
		{
			Beep(200, 210);
			speed = SPEED_LOW;
		}
	}
}

void Gopher::handleDisableButton()
{
	setXboxClickState(XINPUT_GAMEPAD_BACK);
	if (_xboxClickIsDown[XINPUT_GAMEPAD_BACK])
	{
		_disabled = !_disabled;

		if (_disabled) {
			Beep(1800, 200);
			Beep(1600, 200);
			Beep(1400, 200);
			Beep(1200, 200);
			Beep(1000, 200);
		}
		else
		{
			Beep(1000, 200);
			Beep(1200, 200);
			Beep(1400, 200);
			Beep(1600, 200);
			Beep(1800, 200);
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

void Gopher::handleMouseMovement()
{
	POINT cursor;
	GetCursorPos(&cursor);

	short tx = _currentState.Gamepad.sThumbLX;
	short ty = _currentState.Gamepad.sThumbLY;

	float dx = speed * tx * RANGE;
	float dy = -speed * ty * RANGE;

	//filter non-32768 and 32767, wireless ones can glitch sometimes and send it to the edge of the screen, it'll toss out some HUGE integer even when it's centered
	if (dy > 32767) dy = 0;
	if (dy < -32768) dy = 0;
	if (dx > 32767) dx = 0;
	if (dx < -32768) dx = 0;

	float x = cursor.x + _xRest;
	float y = cursor.y + _yRest;

	int dist = dx * dx + dy * dy;

	if (dist > TRUNC_ZONE * TRUNC_ZONE)
	{
		x += dx;
		y += dy;

		_xRest = x - (float)((int)x);
		_yRest = y - (float)((int)y);

		SetCursorPos((int)x, (int)y); //after all click input processing
	}
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

void Gopher::mapKeyboard(DWORD STATE, DWORD key)
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
