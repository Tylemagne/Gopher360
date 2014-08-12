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

	bool holdLeftMouse; //instructed to hold
	bool holdingLeftMouse; //is actually holding

	bool holdRightMouse;
	bool holdingRightMouse;
	
	bool holdScrollUp;
	bool holdingScrollUp = false;

	bool holdScrollDown;
	bool holdingScrollDown = false;

	bool holdEnter;
	bool holdingEnter;

	bool holdDisable;
	bool holdingDisable;

	bool holdWindows;
	bool holdingWindows;

	bool holdLThumb;
	bool holdingLThumb;

	bool holdUp; //dpad
	bool holdingUp;

	bool holdDown;
	bool holdingDown;

	bool holdLeft;
	bool holdingLeft;

	bool holdRight;
	bool holdingRight;

	bool holdSpeed; //bumpers
	bool holdingSpeed;

	bool holdVolume;
	bool holdingVolume;

	bool disabled = false; //use for Select sleep mode

	// Ints used to store proper key binds
	int leftMouseBind,
	rightMouseBind,
	disableBind,
	windowsBind,
	enterBind,
	leftBind,
	rightBind,
	upBind,
	downBind,
	speedBind,
	volumeBind;

int main()
{
	if (!std::ifstream("keyBinds.ini"))
	{
		LPCWSTR keyBinds = L"leftMouse=a\0rightMouse=x\0disable=back\0windows=start\0enter=b\0left=dLeft\0right=dRight\0up=dUp\0down=dDown\0speed=lBumper\0volume=rBumper\0";
		WritePrivateProfileSection(L"keybinds", keyBinds, L".\\keyBinds.ini");

		// Initialize Default Values
		leftMouseBind = XINPUT_GAMEPAD_A;
		rightMouseBind = XINPUT_GAMEPAD_X;
		disableBind = XINPUT_GAMEPAD_BACK;
		windowsBind = XINPUT_GAMEPAD_START;
		enterBind = XINPUT_GAMEPAD_B;
		leftBind = XINPUT_GAMEPAD_DPAD_LEFT;
		rightBind = XINPUT_GAMEPAD_DPAD_RIGHT;
		upBind = XINPUT_GAMEPAD_DPAD_UP;
		downBind = XINPUT_GAMEPAD_DPAD_DOWN;
		speedBind = XINPUT_GAMEPAD_LEFT_SHOULDER;
		volumeBind = XINPUT_GAMEPAD_RIGHT_SHOULDER;
	}
	else
	{
		// Set Proper Key Binds based on ini file
		setKeyBind(L"leftMouse", leftMouseBind);
		setKeyBind(L"rightMouse", rightMouseBind);
		setKeyBind(L"disable", disableBind);
		setKeyBind(L"windows", windowsBind);
		setKeyBind(L"enter", enterBind);
		setKeyBind(L"left", leftBind);
		setKeyBind(L"right", rightBind);
		setKeyBind(L"up", upBind);
		setKeyBind(L"down", downBind);
		setKeyBind(L"speed", speedBind);
		setKeyBind(L"volume", volumeBind);
	}

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
	XINPUT_STATE controllerState = Controller->GetState();


//read input ---------------------------------------------------------------------------------------------------------------------------------
//read input ---------------------------------------------------------------------------------------------------------------------------------
//read input ---------------------------------------------------------------------------------------------------------------------------------


	//XINPUT_GAMEPAD_BACK
	holdDisable = (controllerState.Gamepad.wButtons == disableBind);

	//XINPUT_GAMEPAD_BACK - disable/enable
	if(holdDisable == true && holdingDisable == false){ // && holdingLeftMouseButton == false
		holdingDisable = true;
		printf("---------------DISABLE-DOWN\n");
	}
	else if(holdDisable == false && holdingDisable == true){
		holdingDisable = false;
		if(disabled == false){
				printf("---------------DISABLE-UP - Toggled off, ignoring all input but 'Back'.\n");
				disabled = true;
				Beep(1800,200);
				Beep(1600,200);
				Beep(1400,200);
				Beep(1200,200);
				Beep(1000,200);
				//Sleep(1000);
		}
		else if(disabled == true){
				printf("---------------DISABLE-UP - Toggled on, taking all input.\n");
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
	if(abs(controllerState.Gamepad.sThumbLX) > deadZone)
	{
		addXLeft = (speed * (controllerState.Gamepad.sThumbLX*range));
	}

	//zero check
	else
	{
		addXLeft = 0.0f;
	}

	//get LY info
	if(abs(controllerState.Gamepad.sThumbLY) > deadZone)
	{
		addYLeft = -(speed * (controllerState.Gamepad.sThumbLY*range));
	}

	//zero check
	else
	{
		addYLeft = 0.0f;
	}

	//Get RY info
	holdScrollUp = (controllerState.Gamepad.sThumbRY > scrollDeadZone);
	holdScrollDown = (controllerState.Gamepad.sThumbRY < -scrollDeadZone);

	//XINPUT_GAMEPAD_A
	holdLeftMouse = (controllerState.Gamepad.wButtons == leftMouseBind);

	//XINPUT_GAMEPAD_X
	holdRightMouse = (controllerState.Gamepad.wButtons == rightMouseBind);

	//XINPUT_GAMEPAD_B
	holdEnter = (controllerState.Gamepad.wButtons == enterBind);
	
	//bumpers/shoulders
	holdSpeed = (controllerState.Gamepad.wButtons == speedBind);
	holdVolume = (controllerState.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER);

	//XINPUT_GAMEPAD_START
	holdWindows = (controllerState.Gamepad.wButtons == windowsBind);

	//XINPUT_GAMEPAD_LEFT_THUMB
	holdLThumb = (controllerState.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_THUMB);

	//XINPUT_GAMEPAD_DPAD_UP
	holdUp = (controllerState.Gamepad.wButtons == upBind);

	//XINPUT_GAMEPAD_DPAD_DOWN
	holdDown = (controllerState.Gamepad.wButtons == downBind);

	//XINPUT_GAMEPAD_DPAD_LEFT
	holdLeft = (controllerState.Gamepad.wButtons == leftBind);

	//XINPUT_GAMEPAD_DPAD_RIGHT
	holdRight = (controllerState.Gamepad.wButtons == rightBind);


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

		//lmouse click
		if(holdLeftMouse && !holdingLeftMouse){ // && holdingLeftMouseButton == false
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_LEFTDOWN;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingLeftMouse = true;
			printf("---------------LM-DOWN\n");
		}
		else if(!holdLeftMouse && holdingLeftMouse){
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_LEFTUP;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingLeftMouse = false;
			printf("---------------LM-UP\n");
		}

		//rmouse click
		if(holdRightMouse && !holdingRightMouse){ // && holdingLeftMouseButton == false
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_RIGHTDOWN;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingRightMouse = true;
			printf("---------------RM-DOWN\n");
		}
		else if(!holdRightMouse && holdingRightMouse){
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_RIGHTUP;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
			holdingRightMouse = false;
			printf("---------------RM-UP\n");
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
		if(holdUp && !holdingUp){
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_UP;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingUp = true;
			printf("---------------U-ARROW-DOWN\n");
		}
		else if(!holdUp && holdingUp){
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_UP;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingUp = false;
			printf("---------------U-ARROW-UP\n");
		}

		//arrow down
		if(holdDown && !holdingDown){
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_DOWN;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingDown = true;
			printf("---------------D-ARROW-DOWN\n");
		}
		else if(!holdDown && holdingDown){
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_DOWN;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingDown = false;
			printf("---------------D-ARROW-UP\n");
		}


		//arrow left
		if(holdLeft && !holdingLeft){
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_LEFT;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingLeft = true;
			printf("---------------L-ARROW-DOWN\n");
		}
		else if(!holdLeft && holdingLeft){
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_LEFT;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingLeft = false;
			printf("---------------L-ARROW-UP\n");
		}


		//arrow right
		if(holdRight && !holdingRight){
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_RIGHT;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingRight = true;
			printf("---------------R-ARROW-DOWN\n");
		}
		else if(!holdRight && holdingRight){
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_RIGHT;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingRight = false;
			printf("---------------R-ARROW-UP\n");
		}




		//Windows
		if(holdWindows && !holdingWindows){
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_LWIN;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT));

			holdingWindows = true;
			printf("---------------WINDOWS-DOWN\n");
		}
		else if(!holdWindows && holdingWindows){
			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.wScan = 0;
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = VK_LWIN;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));

			holdingWindows = false;
			printf("---------------WINDOWS-UP\n");
		}


		//Enter
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
			printf("---------------ENTER-DOWN\n");
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
			printf("---------------ENTER-UP\n");
		}

		//Speed Cycler
		if(holdSpeed && !holdingSpeed){
			holdingSpeed = true;
			printf("---------------SPEED-DOWN\n");
		}
		else if(!holdSpeed && holdingSpeed){

			holdingSpeed = false;
			printf("---------------SPEED-UP\n");
			if(speed == speed_low) {Beep(240,250); speed = speed_med;}
			else if(speed == speed_med) {Beep(260,250); speed = speed_high;}
			else if(speed == speed_high){Beep(200,250); speed = speed_low;}
			
		}


		//Volume Cycler
		if(holdVolume && !holdingVolume){
	

			holdVolume = true;
			printf("---------------VOLUME-DOWN\n");
		}
		else if (!holdVolume && holdingVolume){

			holdingVolume = false;
			printf("---------------VOLUME-UP\n");
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

void setKeyBind(LPCWSTR key, int &keyBind)
{
	TCHAR fileValue[15];
	GetPrivateProfileString(L"keybinds", key, NULL, fileValue, 15, L".\\keyBinds.ini");

	std::wstring keyValue = fileValue;

	if (keyValue == L"a")
	{
		keyBind = XINPUT_GAMEPAD_A;
	}
	else if (keyValue == L"x")
	{
		keyBind = XINPUT_GAMEPAD_X;
	}
	else if (keyValue == L"back")
	{
		keyBind = XINPUT_GAMEPAD_BACK;
	}
	else if (keyValue == L"start")
	{
		keyBind = XINPUT_GAMEPAD_START;
	}
	else if (keyValue == L"b")
	{
		keyBind = XINPUT_GAMEPAD_B;
	}
	else if (keyValue == L"dLeft")
	{
		keyBind = XINPUT_GAMEPAD_DPAD_LEFT;
	}
	else if (keyValue == L"dRight")
	{
		keyBind = XINPUT_GAMEPAD_DPAD_RIGHT;
	}
	else if (keyValue == L"dUp")
	{
		keyBind = XINPUT_GAMEPAD_DPAD_UP;
	}
	else if (keyValue == L"dDown")
	{
		keyBind = XINPUT_GAMEPAD_DPAD_DOWN;
	}
	else if (keyValue == L"lBumper")
	{
		keyBind = XINPUT_GAMEPAD_LEFT_SHOULDER;
	}
	else if (keyValue == L"rBumper")
	{
		keyBind = XINPUT_GAMEPAD_RIGHT_SHOULDER;
	}
}