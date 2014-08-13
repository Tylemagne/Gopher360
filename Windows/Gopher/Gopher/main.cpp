/*-------------------------------------------------------------------------------   
    Gopher free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------------------*/

//changes 0.96 -> 0.97: speed variable is global, detects bumpers, all timed (no enter), lbumper speed toggler
//changes 0.97 -> 0.98: ...

#include <Windows.h> //for Beep()
#include <Xinput.h> //controller
#include <stdio.h> //for printf
#include <cmath> //for abs()
#include <fstream> // for config file
#pragma comment(lib, "XInput.lib")

#define INPUT_OTHER 32	// Used to define other input types

void gopherLoop();
BOOL IsElevated();
void setKeyBind(LPCWSTR key, int &keyBind);

/*To do:
* Enable/disable button
Key Codes:
http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx
xinput
http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad%28v=vs.85%29.aspx
*/

typedef struct controllerButton_T {
	enum otherFunction { disable, speed, volume };
	LPCWSTR name;
	int buttonCode = 0;	// e.g. XINPUT_GAMEPAD_A
	int keyCode = 0;	// e.g. VK_RETURN
	int mouseDownCode = 0;	// e.g. MOUSEEVENTF_LEFTDOWN
	int mouseUpCode = 0;	// e.g. MOUSEEVENTF_LEFTUP
	int type = 0;	// e.g. INPUT_MOUSE
	otherFunction other = speed;	// e.g. "speed" or "volume"
} controllerButton;

class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum;
	const float speed_low = 0.000075f;
	const float speed_med = 0.000125f;
	const float speed_high = 0.000175f;
public:
	CXBOXController(int playerNumber);
	XINPUT_STATE GetState();
	bool IsConnected();
	void createDefaultConfig();
	void buttonPressed(controllerButton button);
	void buttonReleased(controllerButton button);
	void pressKey(int keyCode);
	void releaseKey(int keyCode);
	void clickMouse(int mouseCode);
	void assignKey(controllerButton &button);
	bool isPressed(controllerButton button);
	void cycleSpeed();
	void toggleDisable();
	float speed;
	bool disabled;

	// Hold Button Booleans
	bool holdA;
	bool holdingA;
	bool holdX;
	bool holdingX;
	bool holdB;
	bool holdingB;
	bool holdY;
	bool holdingY;
	bool holdStart;
	bool holdingStart;
	bool holdBack;
	bool holdingBack;
	bool holdDUp;
	bool holdingDUp;
	bool holdDDown;
	bool holdingDDown;
	bool holdDLeft;
	bool holdingDLeft;
	bool holdDRight;
	bool holdingDRight;
	bool holdRightBumper;
	bool holdingRightBumper;
	bool holdLeftBumper;
	bool holdingLeftBumper;
	bool holdLStick;
	bool holdingLStick;
	bool holdRStick;
	bool holdingRStick;
	bool holdLeftTriggerHalf;
	bool holdingLeftTriggerHalf;
	bool holdLeftTriggerFull;
	bool holdingLeftTriggerFull;
	bool holdRightTriggerHalf;
	bool holdingRightTriggerHalf;
	bool holdRightTriggerFull;
	bool holdingRightTriggerFull;

	// Button Structs
	controllerButton aButton;
	controllerButton bButton;
	controllerButton xButton;
	controllerButton yButton;
	controllerButton startButton;
	controllerButton backButton;
	controllerButton upDButton;
	controllerButton downDButton;
	controllerButton leftDButton;
	controllerButton rightDButton;
	controllerButton leftBumper;
	controllerButton rightBumper;
	controllerButton leftStick;
	controllerButton rightStick;
	controllerButton leftTriggerHalf;
	controllerButton rightTriggerHalf;
	controllerButton leftTriggerFull;
	controllerButton rightTriggerFull;
};

CXBOXController* Controller;

int main()
{
	Controller = new CXBOXController(1);

	if (!std::ifstream("keyBinds.ini"))
	{
		Controller->createDefaultConfig();
	}
	else
	{
		// Set Proper Key Binds based on ini file
		Controller->assignKey(Controller->aButton);
		Controller->assignKey(Controller->bButton);
		Controller->assignKey(Controller->xButton);
		Controller->assignKey(Controller->yButton);
		Controller->assignKey(Controller->startButton);
		Controller->assignKey(Controller->leftDButton);
		Controller->assignKey(Controller->rightDButton);
		Controller->assignKey(Controller->upDButton);
		Controller->assignKey(Controller->downDButton);
		Controller->assignKey(Controller->leftStick);
		Controller->assignKey(Controller->rightStick);
		Controller->assignKey(Controller->leftBumper);
		Controller->assignKey(Controller->rightBumper);
		Controller->assignKey(Controller->leftTriggerHalf);
		Controller->assignKey(Controller->leftTriggerFull);
		Controller->assignKey(Controller->rightTriggerHalf);
		Controller->assignKey(Controller->rightTriggerFull);
	}

	SetConsoleTitle( TEXT( "Gopher v0.97" ) );
	system("Color 1D");


	//MessageBox(NULL,L"You'll need to run Gopher as an administrator if you intend use the on-screen keyboard. Otherwise, Windows will ignore attempted keystrokes. If not, carry on!",L"Gopher", MB_OK | MB_ICONINFORMATION);
	//Add admin rights checker. If none, display this?

	printf("Welcome to Gopher/Gopher360 - a lightweight controller->keyboard & mouse tool.\nSee the GitHub repository at bit.ly/1syAhMT for more info. Copyleft 2014.\n\n-------------------------\n\n");
	printf("Gopher is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n");
	printf("\nYou should have received a copy of the GNU General Public License\nalong with this program. If not, see http://www.gnu.org/licenses/.\n\n-------------------------\n\n");
	printf("Verify controller and wait 8 seconds to begin...\n\n\n");

	//Sleep(8000);

	//getchar(); //press enter

	if(!IsElevated())
	{
		printf("Tip - Gopher isn't being ran as an administrator.\nWindows won't let you use the on-screen keyboard or games without it.\nLaunching in 3 seconds...\n\n");
		Beep(1400,100);
		Sleep(3000);
	}


	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nBeginning read sequence at ~62.5 loops per second. Prepare for main loop!\n");
	Sleep(1000);

	Beep(1400,100);
	Beep(1400,100);
	Beep(1400,100);

	gopherLoop();
}



void gopherLoop(){
	while (true)
	{
	//initialize variables ------------------------------------------------------------------------------------------------------------------

	int leftX;
	int leftY;
	//int rightx; //disabled to avoid 'unused' compiler warning
	//int righty;
	float addXLeft;
	float addYLeft;
	//float addXRight;
	//float addYRight;
	int deadZone = 3000; //X and Y minimum, below this is ignored since all controllers have some stick to them
	int scrollDeadZone = 7000; // Right thumbstick should be less sensitive.
	int scrollSpeed = 20; // Speed at which you scroll page.
	float range = 4.0f; //4 gives a decent range. Raising this requires a lowering of speed as well.
	int truncZone = 3; //anything below this is ignored and the mouse sits still, similar to a deadzone
	int sleepAmount = 16; //ideally 16, refreshes 60 times per second (1000/16 = ~60)
	POINT cursor; //ehh
	XINPUT_STATE controllerState = Controller->GetState();

//read input ---------------------------------------------------------------------------------------------------------------------------------
//read input ---------------------------------------------------------------------------------------------------------------------------------
//read input ---------------------------------------------------------------------------------------------------------------------------------

		Controller->holdA = Controller->isPressed(Controller->aButton);

		Controller->holdX = Controller->isPressed(Controller->xButton);

		Controller->holdB = Controller->isPressed(Controller->bButton);

		Controller->holdY = Controller->isPressed(Controller->yButton);

		Controller->holdStart = Controller->isPressed(Controller->startButton);

		Controller->holdBack = Controller->isPressed(Controller->backButton);

		Controller->holdDUp = Controller->isPressed(Controller->upDButton);

		Controller->holdDDown = Controller->isPressed(Controller->downDButton);

		Controller->holdDLeft = Controller->isPressed(Controller->leftDButton);

		Controller->holdDRight = Controller->isPressed(Controller->rightDButton);

		Controller->holdRightBumper = Controller->isPressed(Controller->rightBumper);

		Controller->holdLeftBumper = Controller->isPressed(Controller->leftBumper);

		Controller->holdLStick = Controller->isPressed(Controller->leftStick);

		Controller->holdRStick = Controller->isPressed(Controller->rightStick);

		Controller->holdLeftTriggerHalf = (Controller->GetState().Gamepad.bLeftTrigger > 0 && Controller->GetState().Gamepad.bLeftTrigger < 150);

		Controller->holdLeftTriggerFull = (Controller->GetState().Gamepad.bLeftTrigger >= 150);

		Controller->holdRightTriggerHalf = (Controller->GetState().Gamepad.bRightTrigger > 0 && Controller->GetState().Gamepad.bRightTrigger < 150);

		Controller->holdRightTriggerFull = (Controller->GetState().Gamepad.bRightTrigger >= 150);

		// Back Button Is Not Bindable
			if (Controller->holdBack && !Controller->holdingBack)
			{
				Controller->holdingBack = true;
			}
			else if (Controller->holdingBack && !Controller->holdBack)
			{
				Controller->toggleDisable();
				Controller->holdingBack = false;
			}

		if (Controller->disabled == false){
			//get LX info
			if (abs(controllerState.Gamepad.sThumbLX) > deadZone)
			{
				addXLeft = (Controller->speed * (controllerState.Gamepad.sThumbLX*range));
			}

			//zero check
			else
			{
				addXLeft = 0.0f;
			}

			//get LY info
			if (abs(controllerState.Gamepad.sThumbLY) > deadZone)
			{
				addYLeft = -(Controller->speed * (controllerState.Gamepad.sThumbLY*range));
			}

			//zero check
			else
			{
				addYLeft = 0.0f;
			}

			//process input ---------------------------------------------------------------------------------------------------------------------------------
			//process input ---------------------------------------------------------------------------------------------------------------------------------
			//process input ---------------------------------------------------------------------------------------------------------------------------------

			GetCursorPos(&cursor);

			leftX = cursor.x;
			leftY = cursor.y;

			//filter non-32768 and 32767, wireless ones can glitch sometimes and send it to the edge of the screen
			if (addYLeft > 32767) addYLeft = 0;
			if (addYLeft < -32768) addYLeft = 0;
			if (addXLeft > 32767) addXLeft = 0;
			if (addXLeft < -32768) addXLeft = 0;

			if (abs(addYLeft) > truncZone)
			{
				leftY += (int)addYLeft;
			}

			if (abs(addXLeft) > truncZone)
			{
				leftX += (int)addXLeft;
			}

			if (controllerState.Gamepad.sThumbRY > scrollDeadZone)	// Scroll Up
			{
				INPUT input;
				input.type = INPUT_MOUSE;
				input.mi.mouseData = scrollSpeed;
				input.mi.dwFlags = MOUSEEVENTF_WHEEL;
				input.mi.time = 0;
				SendInput(1, &input, sizeof(INPUT));
			}
			else if (controllerState.Gamepad.sThumbRY < -scrollDeadZone)	// Scroll Down
			{
				INPUT input;
				input.type = INPUT_MOUSE;
				input.mi.mouseData = -scrollSpeed;
				input.mi.dwFlags = MOUSEEVENTF_WHEEL;
				input.mi.time = 0;
				SendInput(1, &input, sizeof(INPUT));
			}

			// A Button
			if (Controller->holdA && !Controller->holdingA)	// Button Pressed
			{
				Controller->buttonPressed(Controller->aButton);
				Controller->holdingA = true;
			}
			else if (Controller->holdingA && !Controller->holdA)	// Button Released
			{
				Controller->buttonReleased(Controller->aButton);
				Controller->holdingA = false;
			}

			// B Button
			else if (Controller->holdB && !Controller->holdingB)
			{
				Controller->buttonPressed(Controller->bButton);
				Controller->holdingB = true;
			}
			else if (Controller->holdingB && !Controller->holdB)
			{
				Controller->buttonReleased(Controller->bButton);
				Controller->holdingB = false;
			}

			// X Button
			else if (Controller->holdX && !Controller->holdingX)
			{
				Controller->buttonPressed(Controller->xButton);
				Controller->holdingX = true;
			}
			else if (Controller->holdingX && !Controller->holdX)
			{
				Controller->buttonReleased(Controller->xButton);
				Controller->holdingX = false;
			}

			// Y Button
			else if (Controller->holdY && !Controller->holdingY)
			{
				Controller->buttonPressed(Controller->yButton);
			}
			else if (Controller->holdingY && !Controller->holdY)
			{
				Controller->buttonReleased(Controller->yButton);
			}

			// Start Button
			else if (Controller->holdStart && !Controller->holdingStart)
			{
				Controller->buttonPressed(Controller->startButton);
				Controller->holdingStart = true;
			}
			else if (Controller->holdingStart && !Controller->holdStart)
			{
				Controller->buttonReleased(Controller->startButton);
				Controller->holdingStart = false;
			}

			// Up DPad
			else if (Controller->holdDUp && !Controller->holdingDUp)
			{
				Controller->buttonPressed(Controller->upDButton);
				Controller->holdingDUp = true;
			}
			else if (Controller->holdingDUp && !Controller->holdDUp)
			{
				Controller->buttonReleased(Controller->upDButton);
				Controller->holdingDUp = false;
			}

			// Down DPad
			else if (Controller->holdDDown && !Controller->holdingDDown)
			{
				Controller->buttonPressed(Controller->downDButton);
				Controller->holdingDDown = true;
			}
			else if (Controller->holdingDDown && !Controller->holdDDown)
			{
				Controller->buttonReleased(Controller->downDButton);
				Controller->holdingDDown = false;
			}

			// Left DPad
			else if (Controller->holdDLeft && !Controller->holdingDLeft)
			{
				Controller->buttonPressed(Controller->leftDButton);
				Controller->holdingDLeft = true;
			}
			else if (Controller->holdingDLeft && !Controller->holdDLeft)
			{
				Controller->buttonReleased(Controller->leftDButton);
				Controller->holdingDLeft = false;
			}

			// Right DPad
			else if (Controller->holdDRight && !Controller->holdingDRight)
			{
				Controller->buttonPressed(Controller->rightDButton);
				Controller->holdingDRight = true;
			}
			else if (Controller->holdingDRight && !Controller->holdDRight)
			{
				Controller->buttonReleased(Controller->bButton);
				Controller->holdingDRight = false;
			}

			// Left Bumper
			else if (Controller->holdLeftBumper && !Controller->holdingLeftBumper)
			{
				Controller->buttonPressed(Controller->leftBumper);
				Controller->holdingLeftBumper = true;
			}
			else if (Controller->holdingLeftBumper && !Controller->holdLeftBumper)
			{
				Controller->buttonReleased(Controller->leftBumper);
				Controller->holdingLeftBumper = false;
			}

			// Right Bumper
			else if (Controller->holdRightBumper && !Controller->holdingRightBumper)
			{
				Controller->buttonPressed(Controller->rightBumper);
				Controller->holdingRightBumper = true;
			}
			else if (Controller->holdingRightBumper && !Controller->holdRightBumper)
			{
				Controller->buttonReleased(Controller->rightBumper);
				Controller->holdingRightBumper = false;
			}

			// Left Stick
			else if (Controller->holdLStick && !Controller->holdingLStick)
			{
				Controller->buttonPressed(Controller->leftStick);
				Controller->holdingLStick = true;
			}
			else if (Controller->holdingLStick && !Controller->holdLStick)
			{
				Controller->buttonReleased(Controller->leftStick);
				Controller->holdingLStick = false;
			}

			// Right Stick
			else if (Controller->holdRStick && !Controller->holdingRStick)
			{
				Controller->buttonPressed(Controller->rightStick);
				Controller->holdingRStick = true;
			}
			else if (Controller->holdingRStick && !Controller->holdRStick)
			{
				Controller->buttonReleased(Controller->rightStick);
				Controller->holdingRStick = false;
			}

			// Right Trigger Half
			else if (Controller->holdRightTriggerHalf && !Controller->holdingRightTriggerHalf)
			{
				Controller->buttonPressed(Controller->rightTriggerHalf);
				Controller->holdingRightTriggerHalf = true;
			}
			else if (Controller->holdingRightTriggerHalf && !Controller->holdRightTriggerHalf)
			{
				Controller->buttonReleased(Controller->rightTriggerHalf);
				Controller->holdingRightTriggerHalf = false;
			}

			// Right Trigger Full
			else if (Controller->holdRightTriggerFull && !Controller->holdingRightTriggerFull)
			{
				Controller->buttonPressed(Controller->rightTriggerFull);
				Controller->holdingRightTriggerFull = true;
			}
			else if (Controller->holdingRightTriggerFull && !Controller->holdRightTriggerFull)
			{
				Controller->buttonReleased(Controller->rightTriggerFull);
				Controller->holdingRightTriggerFull = false;
			}

			// Left Trigger Half
			else if (Controller->holdLeftTriggerHalf && !Controller->holdingLeftTriggerHalf)
			{
				Controller->buttonPressed(Controller->leftTriggerHalf);
				Controller->holdingLeftTriggerHalf = true;
			}
			else if (Controller->holdingLeftTriggerHalf && !Controller->holdLeftTriggerHalf)
			{
				Controller->buttonReleased(Controller->leftTriggerHalf);
				Controller->holdingLeftTriggerHalf = false;
			}

			// Left Trigger Full
			else if (Controller->holdLeftTriggerFull && !Controller->holdingLeftTriggerFull)
			{
				Controller->buttonPressed(Controller->leftTriggerFull);
				Controller->holdingLeftTriggerFull = true;
			}
			else if (Controller->holdingLeftTriggerFull && !Controller->holdLeftTriggerFull)
			{
				Controller->buttonReleased(Controller->leftTriggerFull);
				Controller->holdingLeftTriggerFull = false;
			}

			SetCursorPos(leftX, leftY); //after all click input processing


			printf("Move X:%d, Y:%d\n", (int)addXLeft, -(int)addYLeft);

		}
		Sleep(sleepAmount);
	}
} //..........................................................................................End GopherLoop


BOOL IsElevated()
{
    BOOL   fRet = FALSE;
    HANDLE hToken = NULL;

    if(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken))
	{
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof( TOKEN_ELEVATION );

        if(GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof( Elevation), &cbSize))
		{
            fRet = Elevation.TokenIsElevated;
        }
    }

    if(hToken)
	{
        CloseHandle(hToken);
    }

    return fRet;
}


CXBOXController::CXBOXController(int playerNumber)
{
	_controllerNum = playerNumber - 1; //set number
	this->speed = CXBOXController::speed_med;
	this->disabled = false;

	this->aButton.buttonCode = XINPUT_GAMEPAD_A;
	this->aButton.name = L"A";

	this->bButton.buttonCode = XINPUT_GAMEPAD_B;
	this->bButton.name = L"B";

	this->xButton.buttonCode = XINPUT_GAMEPAD_X;
	this->xButton.name = L"X";

	this->yButton.buttonCode = XINPUT_GAMEPAD_Y;
	this->yButton.name = L"Y";

	this->startButton.buttonCode = XINPUT_GAMEPAD_START;
	this->startButton.name = L"start";

	this->backButton.buttonCode = XINPUT_GAMEPAD_BACK;
	this->backButton.name = L"back";

	this->upDButton.buttonCode = XINPUT_GAMEPAD_DPAD_UP;
	this->upDButton.name = L"dUp";

	this->downDButton.buttonCode = XINPUT_GAMEPAD_DPAD_DOWN;
	this->downDButton.name = L"dDown";

	this->leftDButton.buttonCode = XINPUT_GAMEPAD_DPAD_LEFT;
	this->leftDButton.name = L"dLeft";

	this->rightDButton.buttonCode = XINPUT_GAMEPAD_DPAD_RIGHT;
	this->rightDButton.name = L"dRight";
	
	this->leftBumper.buttonCode = XINPUT_GAMEPAD_LEFT_SHOULDER;
	this->leftBumper.name = L"leftBumper";
	
	this->rightBumper.buttonCode = XINPUT_GAMEPAD_RIGHT_SHOULDER;
	this->rightBumper.name = L"rightBumper";
	
	this->leftStick.buttonCode = XINPUT_GAMEPAD_LEFT_THUMB;
	this->leftStick.name = L"leftStick";
	
	this->rightStick.buttonCode = XINPUT_GAMEPAD_RIGHT_THUMB;
	this->rightStick.name = L"rightStick";

	this->leftTriggerHalf.buttonCode = -1;
	this->leftTriggerHalf.name = L"leftTriggerHalf";

	this->leftTriggerFull.buttonCode = -1;
	this->leftTriggerFull.name = L"leftTriggerFull";

	this->rightTriggerHalf.buttonCode = -1;
	this->rightTriggerHalf.name = L"rightTriggerHalf";

	this->rightTriggerFull.buttonCode = -1;
	this->rightTriggerFull.name = L"rightTriggerFull";

	this->holdingA = false;

	this->holdingX = false;

	this->holdingB = false;

	this->holdingY = false;

	this->holdingStart = false;

	this->holdingBack = false;

	this->holdingDUp = false;

	this->holdingDDown = false;

	this->holdingDLeft = false;

	this->holdingDRight = false;

	this->holdingRightBumper = false;

	this->holdingLeftBumper = false;

	this->holdingLStick = false;

	this->holdingRStick = false;

	this->holdingLeftTriggerHalf = false;

	this->holdingLeftTriggerFull = false;

	this->holdingRightTriggerHalf = false;

	this->holdingRightTriggerFull = false;
}

XINPUT_STATE CXBOXController::GetState()
{
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
	XInputGetState(_controllerNum, &_controllerState);
	return _controllerState;
}

bool CXBOXController::IsConnected()
{
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	return (Result == ERROR_SUCCESS);
}

void CXBOXController::createDefaultConfig()
{
	LPCWSTR keyBinds = L"A=leftMouse\0X=rightMouse\0B=enter\0Y= \0start=windows\0dLeft=left\0dRight=right\0dUp=up\0dDown=down\0leftBumper=speed\0rightBumper=volume\0leftStick=middleMouse\0rightStick= \0rightTriggerHalf= \0rightTriggerFull= \0leftTriggerHalf= \0leftTriggerFull= \0";
	WritePrivateProfileSection(L"keybinds", keyBinds, L".\\keyBinds.ini");

	// Initialize Default Values
	this->aButton.mouseDownCode = MOUSEEVENTF_LEFTDOWN;
	this->aButton.mouseUpCode = MOUSEEVENTF_LEFTUP;
	this->aButton.type = INPUT_MOUSE;

	this->xButton.mouseDownCode = MOUSEEVENTF_RIGHTDOWN;
	this->xButton.mouseUpCode = MOUSEEVENTF_RIGHTUP;
	this->xButton.type = INPUT_MOUSE;

	this->bButton.keyCode = VK_RETURN;
	this->bButton.type = INPUT_KEYBOARD;

	this->yButton.keyCode = 0;	// No default for Y yet
	this->yButton.type = 0;

	this->upDButton.keyCode = VK_UP;
	this->upDButton.type = INPUT_KEYBOARD;

	this->downDButton.keyCode = VK_DOWN;
	this->downDButton.type = INPUT_KEYBOARD;

	this->leftDButton.keyCode = VK_LEFT;
	this->leftDButton.type = INPUT_KEYBOARD;

	this->rightDButton.keyCode = VK_RIGHT;
	this->rightDButton.type = INPUT_KEYBOARD;

	this->startButton.keyCode = VK_LWIN;
	this->startButton.type = INPUT_KEYBOARD;

	this->backButton.type = INPUT_OTHER;	// 0 Used for other functions like speed/volume/disabling
	this->backButton.other = controllerButton::otherFunction::disable;

	this->leftBumper.type = INPUT_OTHER;
	this->leftBumper.other = controllerButton::otherFunction::speed;

	this->rightBumper.type = INPUT_OTHER;
	this->rightBumper.other = controllerButton::otherFunction::volume;

	this->leftStick.mouseDownCode = MOUSEEVENTF_MIDDLEDOWN;
	this->leftStick.mouseUpCode = MOUSEEVENTF_MIDDLEUP;
	this->leftStick.type = INPUT_MOUSE;

	this->rightStick.keyCode = 0;	// No default for right stick yet
	this->rightStick.type = 0;

	this->rightTriggerHalf.keyCode = -1;
	this->rightTriggerHalf.type = 0;

	this->leftTriggerHalf.keyCode = -1;
	this->leftTriggerHalf.type = 0;

	this->rightTriggerFull.keyCode = -1;
	this->rightTriggerFull.type = 0;

	this->leftTriggerFull.keyCode = -1;
	this->leftTriggerFull.type = 0;
}

void CXBOXController::buttonPressed(controllerButton button)
{
	if (button.type == INPUT_MOUSE)
	{
		this->clickMouse(button.mouseDownCode);
	}
	else if (button.type == INPUT_KEYBOARD)
	{
		this->pressKey(button.keyCode);
	}
	else if (button.type == INPUT_OTHER)
	{
		if (button.other == controllerButton::otherFunction::disable)
		{
			this->toggleDisable();
		}
		else if (button.other == controllerButton::otherFunction::speed)
		{
			this->cycleSpeed();
		}
		else if (button.other == controllerButton::otherFunction::volume)
		{

		}
	}
}

void CXBOXController::buttonReleased(controllerButton button)
{
	if (button.type == INPUT_MOUSE)
	{
		this->clickMouse(button.mouseUpCode);
	}
	else if (button.type == INPUT_KEYBOARD)
	{
		this->releaseKey(button.keyCode);
	}
}

void CXBOXController::cycleSpeed()
{
	if (speed == speed_low) { Beep(240, 250); speed = speed_med; }
	else if (speed == speed_med) { Beep(260, 250); speed = speed_high; }
	else if (speed == speed_high){ Beep(200, 250); speed = speed_low; }
}

void CXBOXController::toggleDisable()
{
	if (!this->disabled)
	{
		printf("---------------DISABLE-UP - Toggled off, ignoring all input but 'Back'.\n");
		this->disabled = true;
		Beep(1800, 200);
		Beep(1600, 200);
		Beep(1400, 200);
		Beep(1200, 200);
		Beep(1000, 200);
		//Sleep(1000);
	}
	else
	{
		printf("---------------DISABLE-UP - Toggled on, taking all input.\n");
		this->disabled = false;
		Beep(1000, 200);
		Beep(1200, 200);
		Beep(1400, 200);
		Beep(1600, 200);
		Beep(1800, 200);
		//Sleep(1000);
	}
}

void CXBOXController::pressKey(int keyCode)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = keyCode;
	input.ki.dwFlags = 0;
	SendInput(1, &input, sizeof(INPUT));
}

void CXBOXController::releaseKey(int keyCode)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = keyCode;
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

void CXBOXController::clickMouse(int mouseCode)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.dwFlags = mouseCode;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));
}

bool CXBOXController::isPressed(controllerButton button)
{
	return (this->GetState().Gamepad.wButtons == button.buttonCode);
}

void CXBOXController::assignKey(controllerButton &button)
{
	TCHAR fileValue[15];
	GetPrivateProfileString(L"keybinds", button.name, NULL, fileValue, 15, L".\\keyBinds.ini");
	std::wstring keyValue = fileValue;

	// Mouse Buttons
	if (keyValue == L"leftMouse")
	{
		button.mouseDownCode = MOUSEEVENTF_LEFTDOWN;
		button.mouseUpCode = MOUSEEVENTF_LEFTUP;
		button.type = INPUT_MOUSE;
	}
	else if (keyValue == L"rightMouse")
	{
		button.mouseDownCode = MOUSEEVENTF_RIGHTDOWN;
		button.mouseUpCode = MOUSEEVENTF_RIGHTUP;
		button.type = INPUT_MOUSE;
	}
	else if (keyValue == L"middleMouse")
	{
		button.mouseDownCode = MOUSEEVENTF_MIDDLEDOWN;
		button.mouseUpCode = MOUSEEVENTF_MIDDLEUP;
		button.type = INPUT_MOUSE;
	}

	// Keyboard Buttons

	// Key codes for letters and numbers are equal to their ASCII values
	else if (keyValue.length() == 1)
	{
		button.keyCode = (int)keyValue[0];
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"enter")
	{
		button.keyCode = VK_RETURN;
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"control")
	{
		button.keyCode = VK_CONTROL;
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"shift")
	{
		button.keyCode = VK_LSHIFT;
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"alt")
	{
		button.keyCode = VK_MENU;
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"windows")
	{
		button.keyCode = VK_LWIN;
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"up")
	{
		button.keyCode = VK_UP;
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"down")
	{
		button.keyCode = VK_DOWN;
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"left")
	{
		button.keyCode = VK_LEFT;
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"right")
	{
		button.keyCode = VK_RIGHT;
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"backspace")
	{
		button.keyCode = VK_BACK;
		button.type = INPUT_KEYBOARD;
	}
	else if (keyValue == L"tab")
	{
		button.keyCode = VK_TAB;
		button.type = INPUT_KEYBOARD;
	}

	// Other
	else if (keyValue == L"speed")
	{
		button.type = INPUT_OTHER;
		button.other = controllerButton::otherFunction::speed;
	}
	else if (keyValue == L"disable")
	{
		button.type = INPUT_OTHER;
		button.other = controllerButton::otherFunction::disable;
	}
	else if (keyValue == L"volume")
	{
		button.type = INPUT_OTHER;
		button.other = controllerButton::otherFunction::volume;
	}

	else
	{
		button.keyCode = VK_SPACE;
		button.type = INPUT_KEYBOARD;
	}
}