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
//changes 0.97 -> 0.98: performance improvements, operational volume function, shorter beeps, no XY text
//changes 0.98 -> 0.985: 144Hz, Y to hide window(added float stillHoldingY), code cleanup, comments added
//changes 0.985 -> 0.986: Adding configuration file, changing from beeps to vibration.
//changes 0.986 -> 0.989: Improved speeds and speed reporting, created automatic config generator!
//TODO FOR FUTURE VERSIONS - offload speeds into config file

#include <windows.h> //for Beep()
#include <iostream>


#pragma comment(lib, "XInput9_1_0.lib")
#pragma comment(lib, "winmm") //for volume

#include "Gopher.h"

bool ChangeVolume(double nVolume, bool bScalar); //not used yet
BOOL isRunningAsAdministrator(); //check if administrator, makes on-screen keyboard clickable

/*To do:
* Enable/disable button
Key Codes:
http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx
xinput
http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad%28v=vs.85%29.aspx
*/

int main()
{
	CXBOXController controller(1);
	Gopher gopher(&controller);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle( TEXT( "Gopher v0.989" ) );

	system("Color 1D");

	//MessageBox(NULL,L"You'll need to run Gopher as an administrator if you intend use the on-screen keyboard. Otherwise, Windows will ignore attempted keystrokes. If not, carry on!",L"Gopher", MB_OK | MB_ICONINFORMATION);
	//Add admin rights checker. If none, display this?

	printf("Welcome to Gopher360 - a VERY fast and lightweight controller-to-keyboard & mouse input tool.\n");
	printf("All you need is an Xbox360/Xbone controller (wired or wireless adapter), or DualShock (with InputMapper 1.5+)\n");
	printf("Gopher will autofind the xinput device and begin reading input - if nothing happens, verify connectivity.\n");
	printf("See the GitHub repository at bit.ly/1syAhMT for more info. Twitter contact: TylerAt60FPS\n\n-------------------------\n\n");
	
	SetConsoleTextAttribute(hConsole, 23);
	printf("Gopher is free (as in freedom) software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n");
	printf("\nYou should have received a copy of the GNU General Public License\nalong with this program. If not, see http://www.gnu.org/licenses/.");
	SetConsoleTextAttribute(hConsole, 29);
	printf("\n\n-------------------------\n\n");

	SetConsoleTextAttribute(hConsole, 5); //set color to purple on black (windows only)
	//29 default

	//dump important tips
	printf("Tip - Press left and right bumpers simultaneously to toggle speeds!\n");

	if (!isRunningAsAdministrator())
	{
		printf("Tip - Not running as an admin! Windows on-screen keyboard and others won't work without admin rights.\n");
	}

	

	gopher.loadConfigFile();

	while (true) {
		gopher.loop();
	}
}

BOOL isRunningAsAdministrator()
{
	BOOL   fRet = FALSE;
	HANDLE hToken = NULL;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof( TOKEN_ELEVATION );

		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof( Elevation), &cbSize))
		{
			fRet = Elevation.TokenIsElevated;
		}
	}

	if (hToken)
	{
		CloseHandle(hToken);
	}

	return fRet;
}

//this works, but it's not enabled in the software since the best button for it is still undecided
bool ChangeVolume(double nVolume, bool bScalar) //o b
{

	HRESULT hr = NULL;
	bool decibels = false;
	bool scalar = false;
	double newVolume = nVolume;

	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
	                      __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
	                             CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	// -------------------------
	float currentVolume = 0;
	endpointVolume->GetMasterVolumeLevel(&currentVolume);
	//printf("Current volume in dB is: %f\n", currentVolume);

	hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
	//CString strCur=L"";
	//strCur.Format(L"%f",currentVolume);
	//AfxMessageBox(strCur);

	// printf("Current volume as a scalar is: %f\n", currentVolume);
	if (bScalar == false)
	{
		hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
	}
	else if (bScalar == true)
	{
		hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
	}
	endpointVolume->Release();

	CoUninitialize();

	return FALSE;
}
