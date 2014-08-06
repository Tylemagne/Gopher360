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

#include <Windows.h>
#include <Xinput.h>
#include <stdio.h>
#pragma comment(lib, "XInput.lib")

void gopherLoop();

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

	bool holdLeft; //instructed to hold
	bool holdingLeft; //is actually holding

	bool holdRight;
	bool holdingRight;

	bool holdEnter;
	bool holdingEnter;

	bool holdBack;
	bool holdingBack;

	bool holdStart;
	bool holdingStart;

	bool holdLThumb;
	bool holdingLThumb;

	bool holdDUp;
	bool holdingDUp;

	bool holdDDown;
	bool holdingDDown;

	bool holdDLeft;
	bool holdingDLeft;

	bool holdDRight;
	bool holdingDRight;

	bool disabled = false; //use for Select sleep mode

int main()
{
	SetConsoleTitle( TEXT( "Gopher v0.8" ) );
	Controller = new CXBOXController(1);

	system("Color 1D");
	printf("Welcome to Gopher - a tremendously lightweight controller-to-Mouse tool.\nSee the GitHub repository for Gopher for more information. Copyleft 2014.\n\n-------------------------\n\n");
	printf("Gopher is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n");
	printf("\nYou should have received a copy of the GNU General Public License\nalong with this program. If not, see http://www.gnu.org/licenses/.\n\n-------------------------\n\n");
	printf("Verify controller and press ENTER to begin reading and displaying input...\n\n\n");

	getchar(); //press enter

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nBeginning read sequence at ~62.5 loops per second. Prepare for main loop!\n");
	Sleep(1500);

	while(true){
		gopherLoop();
	}
}



void gopherLoop(){

	//initialize variables ------------------------------------------------------------------------------------------------------------------

	int x;
	int y;
	float addX;
	float addY;
	int deadZone = 3000; //X and Y minimum, below this is ignored since all controllers have some stick to them
	float speed = 0.000125f; //multiplied by integer value of analog X and Y (32,000).
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
	if(Controller->GetState().Gamepad.sThumbLX > deadZone)
	{

		addX = (speed * (Controller->GetState().Gamepad.sThumbLX*range));
	}
	else if(Controller->GetState().Gamepad.sThumbLX < -deadZone)
	{
		addX = (speed * (Controller->GetState().Gamepad.sThumbLX*range));
	}

	//zero check
	if(Controller->GetState().Gamepad.sThumbLX < deadZone && Controller->GetState().Gamepad.sThumbLX > -deadZone)
	{
		addX = 0.0f;
	}



	//get LY info
	if(Controller->GetState().Gamepad.sThumbLY > deadZone)
	{

		addY = -(speed * (Controller->GetState().Gamepad.sThumbLY*range));
	}
	else if(Controller->GetState().Gamepad.sThumbLY < -deadZone)
	{
		addY = -(speed * (Controller->GetState().Gamepad.sThumbLY*range));
	}

	//zero check
	if(Controller->GetState().Gamepad.sThumbLY < deadZone && Controller->GetState().Gamepad.sThumbLY > -deadZone)
	{
		addY = 0.0f;
	}



	//XINPUT_GAMEPAD_A
	if(Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_A)
	{
		//printf("HOLDING MOUSE1..........................\n");
		holdLeft = true;
	}
	else if (Controller->GetState().Gamepad.wButtons != XINPUT_GAMEPAD_A)
	{
		//printf("NOT HOLDING..........................\n");
		holdLeft = false;
	}



	//XINPUT_GAMEPAD_X
	if(Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_X)
	{
		//printf("HOLDING X..........................\n");
		holdRight = true;
	}
	else if (Controller->GetState().Gamepad.wButtons != XINPUT_GAMEPAD_X)
	{
		//printf("NOT HOLDING X..........................\n");
		holdRight = false;
	}


	//XINPUT_GAMEPAD_B
	if(Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_B)
	{
		//printf("HOLDING B..........................\n");
		holdEnter = true;
	}
	else if (Controller->GetState().Gamepad.wButtons != XINPUT_GAMEPAD_B)
	{
		//printf("NOT HOLDING B..........................\n");
		holdEnter = false;
	}




		//XINPUT_GAMEPAD_START
	if(Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_START)
	{
		holdStart = true;
	}
	else if (Controller->GetState().Gamepad.wButtons != XINPUT_GAMEPAD_START)
	{
		holdStart = false;
	}

	//XINPUT_GAMEPAD_LEFT_THUMB
	if(Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_THUMB)
	{
		holdLThumb = true;
	}
	else if (Controller->GetState().Gamepad.wButtons != XINPUT_GAMEPAD_LEFT_THUMB)
	{
		holdLThumb = false;
	}


	//XINPUT_GAMEPAD_DPAD_UP
	if(Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP)
	{
		holdDUp = true;
	}
	else if (Controller->GetState().Gamepad.wButtons != XINPUT_GAMEPAD_DPAD_UP)
	{
		holdDUp = false;
	}

		//XINPUT_GAMEPAD_DPAD_DOWN
	if(Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN)
	{
		holdDDown = true;
	}
	else if (Controller->GetState().Gamepad.wButtons != XINPUT_GAMEPAD_DPAD_DOWN)
	{
		holdDDown = false;
	}

		//XINPUT_GAMEPAD_DPAD_LEFT
	if(Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_LEFT)
	{
		holdDLeft = true;
	}
	else if (Controller->GetState().Gamepad.wButtons != XINPUT_GAMEPAD_DPAD_LEFT)
	{
		holdDLeft = false;
	}

		//XINPUT_GAMEPAD_DPAD_RIGHT
	if(Controller->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		holdDRight = true;
	}
	else if (Controller->GetState().Gamepad.wButtons != XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		holdDRight = false;
	}


//process input ---------------------------------------------------------------------------------------------------------------------------------
//process input ---------------------------------------------------------------------------------------------------------------------------------
//process input ---------------------------------------------------------------------------------------------------------------------------------

	GetCursorPos(&cursor);
	
		x = cursor.x;
		y = cursor.y;

		x += (int)addX;
		y += (int)addY;

		if (addY > -truncZone && addY < truncZone && addY != 0)
		{
					y = cursor.y; // truncate extremely low values
					//printf("Truncated Y\n");
		}

		if (addX > -truncZone && addX < truncZone && addX != 0)
		{
					x = cursor.x; // truncate extremely low values
					//printf("Truncated X\n");
		}


		//filter non-32768 and 32767, wireless ones can glitch sometimes and send it to the edge of the screen
		if(addY > 32767) addY = 0;
		if(addY < -32768) addY = 0;
		if(addX > 32767) addX = 0;
		if(addX < -32768) addX = 0;


		//lmouse
		if(holdLeft == true && holdingLeft == false){ // && holdingLeftMouseButton == false
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_LEFTDOWN;
			SendInput(1,&input,sizeof(input));
			holdingLeft = true;
			printf("---------------L-DOWN\n");
		}
		else if(holdLeft == false && holdingLeft == true){
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_LEFTUP;
			SendInput(1,&input,sizeof(input));
			holdingLeft = false;
			printf("---------------L-UP\n");
		}

		//rmouse
		if(holdRight == true && holdingRight == false){ // && holdingLeftMouseButton == false
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_RIGHTDOWN;
			SendInput(1,&input,sizeof(input));
			holdingRight = true;
			printf("---------------R-DOWN\n");
		}
		else if(holdRight == false && holdingRight == true){
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_RIGHTUP;
			SendInput(1,&input,sizeof(input));
			holdingRight = false;
			printf("---------------R-UP\n");
		}

		//middlemouse
		if(holdLThumb == true && holdingLThumb == false){
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_MIDDLEDOWN;
			SendInput(1,&input,sizeof(input));
			holdingLThumb = true;
			printf("---------------MM-DOWN\n");
		}
		else if(holdLThumb == false && holdingLThumb == true){
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi.mouseData=0;
			input.mi.dwFlags =  MOUSEEVENTF_MIDDLEUP;
			SendInput(1,&input,sizeof(input));
			holdingLThumb = false;
			printf("---------------MM-UP\n");
		}

		//arrow up
		if(holdDUp == true && holdingDUp == false){
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
		else if(holdDUp == false && holdingDUp == true){
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
		if(holdDDown == true && holdingDDown == false){
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
		else if(holdDDown == false && holdingDDown == true){
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





		//start
		if(holdStart == true && holdingStart == false){
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
		else if(holdStart == false && holdingStart == true){
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
		if(holdEnter == true && holdingEnter == false){
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
		else if(holdEnter == false && holdingEnter == true){
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


		
		SetCursorPos(x,y); //after all click input processing
		

		printf("Move X:%d, Y:%d\n", (int)addX, -(int)addY);

		}
		Sleep(sleepAmount);

} //..........................................................................................End GopherLoop


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

	if(Result == ERROR_SUCCESS){
		return true;
	}
	else{
		return false;
	}
}