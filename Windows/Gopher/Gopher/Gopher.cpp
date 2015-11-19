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

void Gopher::loop() {
	Sleep(sleepAmount);

	currentState = _controller->GetState();
	
	handleDisableButton();

	if (disabled)
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
	if (ButtonIsDown[XINPUT_GAMEPAD_Y])
	{
		toggleWindowVisibility();
	}

	mapKeyboard(XINPUT_GAMEPAD_START, VK_LWIN);
	mapKeyboard(XINPUT_GAMEPAD_B, VK_RETURN);

	setXboxClickState(XINPUT_GAMEPAD_LEFT_SHOULDER);


	if (ButtonIsDown[XINPUT_GAMEPAD_LEFT_SHOULDER]) {

		if (speed == speed_low)
		{
			Beep(240, 210);
			speed = speed_med;
		}
		else if (speed == speed_med)
		{
			Beep(260, 210);
			speed = speed_high;
		}
		else if (speed == speed_high)
		{
			Beep(200, 210);
			speed = speed_low;
		}
	}
}

void Gopher::handleDisableButton()
{
	setXboxClickState(XINPUT_GAMEPAD_BACK);
	if (ButtonIsDown[XINPUT_GAMEPAD_BACK])
	{
		disabled = !disabled;

		if (disabled) {
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
	hidden = !hidden;

	if (hidden)
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

	int addXLeft = (speed * (currentState.Gamepad.sThumbLX * range));
	int addYLeft = -(speed * (currentState.Gamepad.sThumbLY * range));

	//filter non-32768 and 32767, wireless ones can glitch sometimes and send it to the edge of the screen, it'll toss out some HUGE integer even when it's centered
	if (addYLeft > 32767) addYLeft = 0;
	if (addYLeft < -32768) addYLeft = 0;
	if (addXLeft > 32767) addXLeft = 0;
	if (addXLeft < -32768) addXLeft = 0;

	int leftX = cursor.x;
	int leftY = cursor.y;

	int dist = addXLeft * addXLeft + addYLeft * addYLeft;

	if (dist > truncZone * truncZone)
	{
		leftY += (int)addYLeft;
		leftX += (int)addXLeft;
	}

	SetCursorPos(leftX, leftY); //after all click input processing
}

void Gopher::handleScrolling()
{
	bool holdScrollUp = currentState.Gamepad.sThumbRY > scrollDeadZone;
	bool holdScrollDown = currentState.Gamepad.sThumbRY < -scrollDeadZone;

	if (holdScrollUp)
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = scrollSpeed;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		input.mi.time = 0;
		SendInput(1, &input, sizeof(INPUT));
		mouseEvent(MOUSEEVENTF_WHEEL, scrollSpeed);
	}

	if (holdScrollDown)
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = -scrollSpeed;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		input.mi.time = 0;
		SendInput(1, &input, sizeof(INPUT));
	}
}

void Gopher::setXboxClickState(DWORD STATE)
{
	ButtonIsDown[STATE] = false;
	ButtonIsUp[STATE] = false;

	if (!this->xboxClickStateExists(STATE))
	{
		ButtonStateLastIteration[STATE] = false;
	}

	bool isDown = currentState.Gamepad.wButtons == STATE;

	if (isDown && !ButtonStateLastIteration[STATE])
	{
		ButtonStateLastIteration[STATE] = true;
		ButtonIsDown[STATE] = true;
	}

	if (!isDown && ButtonStateLastIteration[STATE])
	{
		ButtonStateLastIteration[STATE] = false;
		ButtonIsUp[STATE] = true;
	}

	ButtonStateLastIteration[STATE] = isDown;
}

bool Gopher::xboxClickStateExists(DWORD xinput)
{
	auto it = ButtonStateLastIteration.find(xinput);
	if (it == ButtonStateLastIteration.end())
	{
		return false;
	}

	return true;
}

void Gopher::mapKeyboard(DWORD STATE, DWORD key)
{
	setXboxClickState(STATE);
	if (ButtonIsDown[STATE])
	{
		inputKeyboardDown(key);
	}

	if (ButtonIsUp[STATE])
	{
		inputKeyboardUp(key);
	}
}

void Gopher::mapMouseClick(DWORD STATE, DWORD keyDown, DWORD keyUp)
{
	setXboxClickState(STATE);
	if (ButtonIsDown[STATE])
	{
		mouseEvent(keyDown);
	}

	if (ButtonIsUp[STATE])
	{
		mouseEvent(keyUp);
	}
}
