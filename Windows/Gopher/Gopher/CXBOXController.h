#pragma once

#include <Windows.h>
#include <Xinput.h>

class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum;
public:
	CXBOXController(int playerNumber);
	XINPUT_STATE GetState();
	bool IsConnected();
	void CXBOXController::Vibrate(int leftVal, int rightVal);
};
