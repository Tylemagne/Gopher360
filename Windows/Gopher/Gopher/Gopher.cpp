#include "Gopher.h"

void Gopher::hideWindow()
{
	if (!hidden) {
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);
		hidden = true;
		printf("Window hidden\n");
	}
	else if (hidden) {
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_SHOW);
		hidden = false;
		printf("Window unhidden\n");
	}
}

void Gopher::loop() {
	currentState = Controller->GetState();

	holdBack = currentState.Gamepad.wButtons == XINPUT_GAMEPAD_BACK;

	//XINPUT_GAMEPAD_BACK - disable/enable
	if (holdBack && !holdingBack) { // && !holdingLeftMouseButton

		holdingBack = true;
		printf("---------------BACK-DOWN\n");
	}
	else if (!holdBack && holdingBack) {
		holdingBack = false;
		if (!disabled) {
			printf("---------------BACK-UP - Toggled off, ignoring all input but 'Back'.\n");
			disabled = true;
			Beep(1800, 200);
			Beep(1600, 200);
			Beep(1400, 200);
			Beep(1200, 200);
			Beep(1000, 200);
			//Sleep(1000);
		}
		else if (disabled) {
			printf("---------------BACK-UP - Toggled on, taking all input.\n");
			disabled = false;
			Beep(1000, 200);
			Beep(1200, 200);
			Beep(1400, 200);
			Beep(1600, 200);
			Beep(1800, 200);
			//Sleep(1000);
		}
	}

	if (!disabled) { //only listen to these if Gopher is enabled, otherwise only listens for the button that enables it, up ^

							 //get LX info
		if (abs(currentState.Gamepad.sThumbLX) > deadZone)
		{
			addXLeft = (speed * (currentState.Gamepad.sThumbLX * range));
		}

		//zero check
		else
		{
			addXLeft = 0.0f;
		}

		//get LY info
		if (abs(currentState.Gamepad.sThumbLY) > deadZone)
		{
			addYLeft = -(speed * (currentState.Gamepad.sThumbLY * range));
		}

		//zero check
		else
		{
			addYLeft = 0.0f;
		}

		//Get RY info
		holdScrollUp = (currentState.Gamepad.sThumbRY > scrollDeadZone);
		holdScrollDown = (currentState.Gamepad.sThumbRY < -scrollDeadZone);

		//XINPUT_GAMEPAD_A
		holdLeft = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_A);

		if (holdLeft)
		{
			//printf("HOLDING MOUSE1..........................\n");
		}
		else
		{
			//printf("NOT HOLDING..........................\n");
		}

		//XINPUT_GAMEPAD_Y                 TODO: Make it not spam
		holdY = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_Y);

		if (holdY)
		{
			if (!stillHoldingY) {
				hideWindow();
				stillHoldingY = true;
			}

			//printf("HOLDING Y..........................\n");
		}
		else
		{
			stillHoldingY = false;
			//printf("NOT HOLDING Y..........................\n");
		}

		//XINPUT_GAMEPAD_X
		holdRight = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_X);

		if (holdRight)
		{
			//printf("HOLDING X..........................\n");
		}
		else
		{
			//printf("NOT HOLDING X..........................\n");
		}

		//XINPUT_GAMEPAD_B
		holdEnter = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_B);

		if (holdEnter)
		{
			//printf("HOLDING B..........................\n");
		}
		else
		{
			//printf("NOT HOLDING B..........................\n");
		}

		//bumpers/shoulders
		holdBLeft = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER);
		holdBRight = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER);

		//XINPUT_GAMEPAD_START
		holdStart = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_START);

		//XINPUT_GAMEPAD_LEFT_THUMB
		holdLThumb = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_THUMB);

		//XINPUT_GAMEPAD_DPAD_UP
		holdDUp = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP);

		//XINPUT_GAMEPAD_DPAD_DOWN
		holdDDown = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN);

		//XINPUT_GAMEPAD_DPAD_LEFT
		holdDLeft = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_LEFT);

		//XINPUT_GAMEPAD_DPAD_RIGHT
		holdDRight = (currentState.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_RIGHT);

		//process input ---------------------------------------------------------------------------------------------------------------------------------
		//process input ---------------------------------------------------------------------------------------------------------------------------------
		//process input ---------------------------------------------------------------------------------------------------------------------------------

		GetCursorPos(&cursor);

		leftX = cursor.x;
		leftY = cursor.y;

		if (abs(addYLeft) > truncZone)
		{
			leftY += (int)addYLeft;
		}
		else // truncate extremely low values
		{
			//printf("Truncated Y\n");
		}

		if (abs(addXLeft) > truncZone)
		{
			leftX += (int)addXLeft;
		}
		else // truncate extremely low values
		{
			//printf("Truncated X\n");
		}

		//filter non-32768 and 32767, wireless ones can glitch sometimes and send it to the edge of the screen, it'll toss out some HUGE integer even when it's centered
		if (addYLeft > 32767) addYLeft = 0;
		if (addYLeft < -32768) addYLeft = 0;
		if (addXLeft > 32767) addXLeft = 0;
		if (addXLeft < -32768) addXLeft = 0;

		//lmouse click
		if (holdLeft && !holdingLeft) { // && !holdingLeftMouseButton
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData = 0;
			input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingLeft = true;
			printf("---------------L-DOWN\n");
		}
		else if (!holdLeft && holdingLeft) {
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData = 0;
			input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingLeft = false;
			printf("---------------L-UP\n");
		}

		//rmouse click
		if (holdRight && !holdingRight) { // && !holdingLeftMouseButton
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData = 0;
			input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingRight = true;
			printf("---------------R-DOWN\n");
		}
		else if (!holdRight && holdingRight) {
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData = 0;
			input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingRight = false;
			printf("---------------R-UP\n");
		}

		//middlemouse click
		if (holdLThumb && !holdingLThumb) {
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData = 0;
			input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingLThumb = true;
			printf("---------------MM-DOWN\n");
		}
		else if (!holdLThumb && holdingLThumb) {
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData = 0;
			input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingLThumb = false;
			printf("---------------MM-UP\n");
		}

		//scrollwheel up
		if (holdScrollUp)
		{
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData = scrollSpeed;
			input.mi.dwFlags = MOUSEEVENTF_WHEEL;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			printf("---------------WHEEL-UP\n");
		}

		//scrollwheel down
		if (holdScrollDown)
		{
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData = -scrollSpeed;
			input.mi.dwFlags = MOUSEEVENTF_WHEEL;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			printf("---------------WHEEL-DOWN\n");
		}

		//arrow up
		if (holdDUp && !holdingDUp) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_UP;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingDUp = true;
			printf("---------------DPU-DOWN\n");
		}
		else if (!holdDUp && holdingDUp) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_UP;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingDUp = false;
			printf("---------------DPU-UP\n");
		}

		//arrow down
		if (holdDDown && !holdingDDown) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_DOWN;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingDDown = true;
			printf("---------------DPD-DOWN\n");
		}
		else if (!holdDDown && holdingDDown) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_DOWN;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingDDown = false;
			printf("---------------DPD-UP\n");
		}

		//arrow left
		if (holdDLeft && !holdingDLeft) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_LEFT;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingDLeft = true;
			printf("---------------DPL-DOWN\n");
		}
		else if (!holdDLeft && holdingDLeft) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_LEFT;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingDLeft = false;
			printf("---------------DPL-UP\n");
		}

		//arrow right
		if (holdDRight && !holdingDRight) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_RIGHT;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingDRight = true;
			printf("---------------DPR-DOWN\n");
		}
		else if (!holdDRight && holdingDRight) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_RIGHT;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingDRight = false;
			printf("---------------DPR-UP\n");
		}

		//start
		if (holdStart && !holdingStart) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_LWIN;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingStart = true;
			printf("---------------START-DOWN\n");
		}
		else if (!holdStart && holdingStart) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_LWIN;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingStart = false;
			printf("---------------START-UP\n");
		}

		//enter
		if (holdEnter && !holdingEnter) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_RETURN;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingEnter = true;
			printf("---------------B-DOWN\n");
		}
		else if (!holdEnter && holdingEnter) {
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_RETURN;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingEnter = false;
			printf("---------------B-UP\n");
		}

		//left bumper/shoulder (speed cycler)
		if (holdBLeft && !holdingBLeft) {
			holdingBLeft = true;
			printf("---------------BLEFT-DOWN\n");
		}
		else if (!holdBLeft && holdingBLeft) {

			holdingBLeft = false;
			printf("---------------BLEFT-UP - CURSOR SPEED CHANGED\n");
			if (speed == speed_low) { Beep(240, 210); speed = speed_med; }
			else if (speed == speed_med) { Beep(260, 210); speed = speed_high; }
			else if (speed == speed_high) { Beep(200, 210); speed = speed_low; }

			//ChangeVolume(0.5,true); //works
		}

		//left bumper/shoulder
		if (holdBRight && !holdingBRight) {

			holdingBRight = true;
			printf("---------------BRIGHT-DOWN\n");
		}
		else if (!holdBRight && holdingBRight) {

			holdingBRight = false;
			printf("---------------BRIGHT-UP\n");
		}

		SetCursorPos(leftX, leftY); //after all click input processing

									//printf("Move X:%d, Y:%d\n", (int)addXLeft, -(int)addYLeft); //disabled for being annoying

	}
	Sleep(sleepAmount);

} //..........................................................................................End GopherLoop
