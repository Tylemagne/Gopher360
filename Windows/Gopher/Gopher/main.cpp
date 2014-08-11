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
#pragma comment(lib, "XInput.lib")

void gopherLoop();
BOOL IsElevated();

/*To do:
* Enable/disable button
Key Codes:
http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx
xinput
http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad%28v=vs.85%29.aspx
*/

class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum;
public:
	CXBOXController(int playerNumber);
	XINPUT_STATE GetState();
	bool IsConnected();
};

CXBOXController* Controller;

	float speed = 0.000125f; //multiplied by integer value of analog X and Y (32,000).
	float speed_low  = 0.000075f;
	float speed_med  = 0.000125f;
	float speed_high = 0.000175f;

	bool holdLeft; //instructed to hold
	bool holdingLeft; //is actually holding

	bool holdRight;
	bool holdingRight;
	
	bool holdScrollUp;
	bool holdingScrollUp = false;

	bool holdScrollDown;
	bool holdingScrollDown = false;

	bool holdEnter;
	bool holdingEnter;

	bool holdBack;
	bool holdingBack;

	bool holdStart;
	bool holdingStart;

	bool holdLThumb;
	bool holdingLThumb;

	bool holdDUp; //dpad
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

int main()
{
	SetConsoleTitle( TEXT( "Gopher v0.97" ) );
	Controller = new CXBOXController(1);

	system("Color 1D");


	//MessageBox(NULL,L"You'll need to run Gopher as an administrator if you intend use the on-screen keyboard. Otherwise, Windows will ignore attempted keystrokes. If not, carry on!",L"Gopher", MB_OK | MB_ICONINFORMATION);
	//Add admin rights checker. If none, display this?

	printf("Welcome to Gopher/Gopher360 - a lightweight controller->keyboard & mouse tool.\nSee the GitHub repository at bit.ly/1syAhMT for more info. Copyleft 2014.\n\n-------------------------\n\n");
	printf("Gopher is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n");
	printf("\nYou should have received a copy of the GNU General Public License\nalong with this program. If not, see http://www.gnu.org/licenses/.\n\n-------------------------\n\n");
	printf("Verify controller and wait 8 seconds to begin...\n\n\n");

	Sleep(8000);

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

	while(true){
		gopherLoop();
	}
}



void gopherLoop(){

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


//read input ---------------------------------------------------------------------------------------------------------------------------------
//read input ---------------------------------------------------------------------------------------------------------------------------------
//read input ---------------------------------------------------------------------------------------------------------------------------------


		//XINPUT_GAMEPAD_BACK
	if(Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_BACK)
	{
		holdBack = true;
	}
	else if (Controller->GetState().Gamepad.wButtons != XINPUT_GAMEPAD_BACK)
	{
		holdBack = false;
	}


	//XINPUT_GAMEPAD_BACK - disable/enable
	if(holdBack == true && holdingBack == false){ // && holdingLeftMouseButton == false

		holdingBack = true;
		printf("---------------BACK-DOWN\n");
	}
	else if(holdBack == false && holdingBack == true){
		holdingBack = false;
		if(disabled == false){
				printf("---------------BACK-UP - Toggled off, ignoring all input but 'Back'.\n");
				disabled = true;
				Beep(1800,200);
				Beep(1600,200);
				Beep(1400,200);
				Beep(1200,200);
				Beep(1000,200);
				//Sleep(1000);
		}
		else if(disabled == true){
				printf("---------------BACK-UP - Toggled on, taking all input.\n");
				disabled = false;
				Beep(1000,200);
				Beep(1200,200);
				Beep(1400,200);
				Beep(1600,200);
				Beep(1800,200);
				//Sleep(1000);
		}
	}


	if(disabled == false){



	//get LX info
	if(abs(Controller->GetState().Gamepad.sThumbLX) > deadZone)
	{
		addXLeft = (speed * (Controller->GetState().Gamepad.sThumbLX*range));
	}

	//zero check
	else
	{
		addXLeft = 0.0f;
	}



	//get LY info
	if(abs(Controller->GetState().Gamepad.sThumbLY) > deadZone)
	{
		addYLeft = -(speed * (Controller->GetState().Gamepad.sThumbLY*range));
	}

	//zero check
	else
	{
		addYLeft = 0.0f;
	}

	//Get RY info
	holdScrollUp = (Controller->GetState().Gamepad.sThumbRY > scrollDeadZone);
	holdScrollDown = (Controller->GetState().Gamepad.sThumbRY < -scrollDeadZone);



	//XINPUT_GAMEPAD_A
	holdLeft = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_A);

	if (holdLeft)
	{
		//printf("HOLDING MOUSE1..........................\n");
	}
	else
	{
		//printf("NOT HOLDING..........................\n");
	}



	//XINPUT_GAMEPAD_X
	holdRight = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_X);

	if (holdRight)
	{
		//printf("HOLDING X..........................\n");
	}
	else
	{
		//printf("NOT HOLDING X..........................\n");
	}


	//XINPUT_GAMEPAD_B
	holdEnter = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_B);
	
	if (holdEnter)
	{
		//printf("HOLDING B..........................\n");
	}
	else
	{
		//printf("NOT HOLDING B..........................\n");
	}

	//bumpers/shoulders
	holdBLeft = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER);
	holdBRight = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER);



	//XINPUT_GAMEPAD_START
	holdStart = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_START);

	//XINPUT_GAMEPAD_LEFT_THUMB
	holdLThumb = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_THUMB);

	//XINPUT_GAMEPAD_DPAD_UP
	holdDUp = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP);

	//XINPUT_GAMEPAD_DPAD_DOWN
	holdDDown = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN);

	//XINPUT_GAMEPAD_DPAD_LEFT
	holdDLeft = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_LEFT);

	//XINPUT_GAMEPAD_DPAD_RIGHT
	holdDRight = (Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_RIGHT);


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


		//filter non-32768 and 32767, wireless ones can glitch sometimes and send it to the edge of the screen
		if(addYLeft > 32767) addYLeft = 0;
		if(addYLeft < -32768) addYLeft = 0;
		if(addXLeft > 32767) addXLeft = 0;
		if(addXLeft < -32768) addXLeft = 0;


		//lmouse click
		if(holdLeft && !holdingLeft){ // && holdingLeftMouseButton == false
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_LEFTDOWN;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingLeft = true;
			printf("---------------L-DOWN\n");
		}
		else if(!holdLeft && holdingLeft){
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_LEFTUP;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingLeft = false;
			printf("---------------L-UP\n");
		}

		//rmouse click
		if(holdRight && !holdingRight){ // && holdingLeftMouseButton == false
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_RIGHTDOWN;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingRight = true;
			printf("---------------R-DOWN\n");
		}
		else if(!holdRight && holdingRight){
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_RIGHTUP;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingRight = false;
			printf("---------------R-UP\n");
		}

		//middlemouse click
		if(holdLThumb && !holdingLThumb){
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_MIDDLEDOWN;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingLThumb = true;
			printf("---------------MM-DOWN\n");
		}
		else if(!holdLThumb && holdingLThumb){
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_MIDDLEUP;
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
		if(holdDUp && !holdingDUp){
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
		else if(!holdDUp && holdingDUp){
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
		if(holdDDown && !holdingDDown){
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
		else if(!holdDDown && holdingDDown){
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
		if(holdDLeft && !holdingDLeft){
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
		else if(!holdDLeft && holdingDLeft){
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
		if(holdDRight && !holdingDRight){
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
		else if(!holdDRight && holdingDRight){
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
		if(holdStart && !holdingStart){
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
		else if(!holdStart && holdingStart){
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
		if(holdEnter && !holdingEnter){
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
		else if(!holdEnter && holdingEnter){
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
		if(holdBLeft && !holdingBLeft){
			holdingBLeft = true;
			printf("---------------BLEFT-DOWN\n");
		}
		else if(!holdBLeft && holdingBLeft){

			holdingBLeft = false;
			printf("---------------BLEFT-UP\n");
			if(speed == speed_low) {Beep(240,250); speed = speed_med;}
			else if(speed == speed_med) {Beep(260,250); speed = speed_high;}
			else if(speed == speed_high){Beep(200,250); speed = speed_low;}
			
		}


		//left bumper/shoulder
		if(holdBRight && !holdingBRight){
	

			holdingBRight = true;
			printf("---------------BRIGHT-DOWN\n");
		}
		else if(!holdBRight && holdingBRight){

			holdingBRight = false;
			printf("---------------BRIGHT-UP\n");
		}


		
		SetCursorPos(leftX,leftY); //after all click input processing
		

		printf("Move X:%d, Y:%d\n", (int)addXLeft, -(int)addYLeft);

		}
		Sleep(sleepAmount);

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
