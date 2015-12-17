#include "CXBOXController.h"

CXBOXController::CXBOXController(int playerNumber)
{
	_controllerNum = playerNumber - 1; //set number
}

XINPUT_STATE CXBOXController::GetState()
{
	ZeroMemory(&this->_controllerState, sizeof(XINPUT_STATE));
	XInputGetState(_controllerNum, &this->_controllerState);
	return _controllerState;
}

bool CXBOXController::IsConnected()
{
	ZeroMemory(&this->_controllerState, sizeof(XINPUT_STATE));
	DWORD Result = XInputGetState(_controllerNum, &this->_controllerState);

	return (Result == ERROR_SUCCESS);
}
