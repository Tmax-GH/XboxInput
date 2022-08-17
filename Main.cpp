//(NOTE) RAM Usage should be okay, but check for Allocations if Controller is reconnected
//(NOTE) Get Windows Version as a variable?
//(TODO) Set Admin mode permanently for windows 8+ 
//(NOTE) Map key bindings with cout/cin?



//#define WINVER 0x0500    //WIN 2000
//#define WINVER 0x0A00    //WIN 10
#define WINVER 0x0601      //WIN 7
#pragma comment (lib, "XInput9_1_0.lib")    // statt Xinput.lib, um error mit 1_4.dll zu vermeiden


#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <wtypes.h>
#include <string>

using namespace std;




#define LMB               0x01
#define RMB               0x02
#define MMB               0x04   
#define ENTER             0x0D
#define BACKSPACE         0x08   
#define TAB               0x09
#define CONTROL           0x11
#define ALT				  0x12
#define SPACE             0x20
#define PAGE_UP           0x21
#define PAGE_DOWN         0x22
#define LEFT_ARROW        0x25
#define RIGHT_ARROW       0x27
#define UP_ARROW          0x26
#define DOWN_ARROW        0x28
#define VOLUME_UP         0xAF
#define VOLUME_DOWN       0xAE
#define VOLUME_MUTE       0xAD
#define SHIFT             0x10
#define A_KEY             0x41
#define B_KEY             0x42
#define C_KEY			  0x43
#define D_KEY             0x44
#define E_KEY             0x45
#define F_KEY             0x46
#define G_KEY             0x47
#define H_KEY             0x48
#define I_KEY             0x49
#define J_KEY             0x4A
#define K_KEY             0x4B
#define L_KEY             0x4C
#define M_KEY             0x4D
#define N_KEY             0x4E
#define O_KEY             0x4F
#define P_KEY             0x50
#define Q_KEY             0x51
#define R_KEY             0x52
#define S_KEY             0x53
#define T_KEY             0x54
#define U_KEY             0x55
#define V_KEY             0x56
#define W_KEY             0x57
#define X_KEY             0x58
#define Y_KEY             0x59
#define Z_KEY             0x5A
#define PERIOD            0xBE
#define NUM_7             0x37


#define BROWSER_REFRESH   0xA8
#define BROWSER_HOME      0xAC
#define BROWSER_SEARCH    0xAA
#define BROWSER_BACK      0xA6
#define BROWSER_FORWARD   0xA7

#define GAMEPAD_D_UP      0x001
#define GAMEPAD_D_DOWN    0x002
#define	GAMEPAD_D_LEFT    0x004
#define GAMEPAD_D_RIGHT   0x008
#define GAMEPAD_START     0x0010
#define GAMEPAD_SELECT    0x0020
#define GAMEPAD_LTHUMB    0x0040
#define GAMEPAD_RTHUMB    0x0080
#define GAMEPAD_LSHOULDER 0x0100
#define GAMEPAD_RSHOULDER 0x0200
#define GAMEPAD_A         0x1000
#define GAMEPAD_B         0x2000
#define GAMEPAD_X         0x4000
#define GAMEPAD_Y         0x8000



WORD Alphabet[27] = 
{
	A_KEY, B_KEY, C_KEY, D_KEY,
	E_KEY, F_KEY, G_KEY, H_KEY,
	I_KEY, J_KEY, K_KEY, L_KEY, 
	M_KEY, N_KEY, O_KEY, P_KEY,
	Q_KEY, R_KEY, S_KEY, T_KEY,
	U_KEY, V_KEY, W_KEY,X_KEY,
	Y_KEY, Z_KEY, PERIOD
};



class Gamepad
{
private:
	int controllerId;
	XINPUT_STATE state;

	float deadzoneX;
	float deadzoneY;

public:
	Gamepad() : deadzoneX(0.21f), deadzoneY(0.21f) {}   
	Gamepad(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY) {}

	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;
	float leftTrigger;
	float rightTrigger;

	int  GetPort();
	XINPUT_GAMEPAD *GetState();
	bool CheckIfConnected();
	bool Refresh();
	bool IsPressed(WORD);
	WORD GetButtonPressed();
};

int Gamepad::GetPort()
{
	return controllerId + 1;
}

XINPUT_GAMEPAD *Gamepad::GetState()
{
	return &state.Gamepad;
}

bool Gamepad::CheckIfConnected()
{
	int cId = -1;

	for (DWORD i = 0; i < XUSER_MAX_COUNT && cId == -1; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == ERROR_SUCCESS)
			cId = i;
	}

	controllerId = cId;

	return controllerId != -1;
}

// Returns false if the controller has been disconnected
bool Gamepad::Refresh()
{
	if (controllerId == -1)
		CheckIfConnected();

	if (controllerId != -1)
	{
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(controllerId, &state) != ERROR_SUCCESS)
		{
			controllerId = -1;
			return false;
		}

		//LEFT
		float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

		
		if((abs(normLX) < deadzoneX)) leftStickX = 0;
		  else leftStickX = (abs(normLX) - deadzoneX) * (normLX / abs(normLX));
		if ((abs(normLY) < deadzoneY)) leftStickY = 0;
		else leftStickY = (abs(normLY) - deadzoneY) * (normLY / abs(normLY));

		if (deadzoneX > 0) leftStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) leftStickY *= 1 / (1 - deadzoneY);

		//RIGHT
		float normRX = fmaxf(-1, (float)state.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)state.Gamepad.sThumbRY / 32767);


		if ((abs(normRX) < deadzoneX)) rightStickX = 0;
		 else rightStickX = (abs(normRX) - deadzoneX) * (normRX / abs(normRX));
		if ((abs(normRY) < deadzoneY)) rightStickY = 0;
		else rightStickY = (abs(normRY) - deadzoneY) * (normRY / abs(normRY));



		if (deadzoneX > 0) rightStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) rightStickY *= 1 / (1 - deadzoneY);

		leftTrigger = (float)state.Gamepad.bLeftTrigger / 255;
		rightTrigger = (float)state.Gamepad.bRightTrigger / 255;

		return true;
	}
	return false;
}

bool Gamepad::IsPressed(WORD button)
{
	return (state.Gamepad.wButtons & button) != 0;

}

WORD Gamepad::GetButtonPressed()
{
	WORD inputKey = state.Gamepad.wButtons;
	return inputKey;
	

}


void KeyboardInput(WORD Key)
{
	INPUT Input = { 0 };
	Input.type = INPUT_KEYBOARD;
	Input.ki.wScan = 0; // hardware scan code for key
	Input.ki.time = 0;
	Input.ki.dwExtraInfo = 0;

	// DOWN
	Input.ki.wVk = Key; // 
	Input.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &Input, sizeof(INPUT));

	// UP
	Input.ki.dwFlags = KEYEVENTF_KEYUP; 
	SendInput(1, &Input, sizeof(INPUT));


}

void KeyboardMultipleInput(WORD Key1, WORD Key2)
{
	INPUT Input1 = { 0 };
	Input1.type = INPUT_KEYBOARD;
	Input1.ki.wScan = 0; // hardware scan code for key
	Input1.ki.time = 0;
	Input1.ki.dwExtraInfo = 0;

	INPUT Input2 = { 0 };
	Input2.type = INPUT_KEYBOARD;
	Input2.ki.wScan = 0; // hardware scan code for key
	Input2.ki.time = 0;
	Input2.ki.dwExtraInfo = 0;

	// Key 1 DOWN
	Input1.ki.wVk = Key1; 
	Input1.ki.dwFlags = 0; 
	SendInput(1, &Input1, sizeof(INPUT));

	// Key 2 DOWN
	Input2.ki.wVk = Key2;
	Input2.ki.dwFlags = 0;
	SendInput(1, &Input2, sizeof(INPUT));

	//Key 2 UP
	Input2.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &Input2, sizeof(INPUT));


	// Key 1 UP
	Input1.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &Input1, sizeof(INPUT));



}

void LeftMouseClick()
{
	INPUT    Input = { 0 };
	//left down
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	//left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void RightMouseClick()
{
	INPUT Input = { 0 };
	//right down
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	//right up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void MiddleMouseClick()
{
	INPUT Input = { 0 };
	//right down
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	//right up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
	::SendInput(1, &Input, sizeof(INPUT));

}



void MouseWheel(float dy)
{
	INPUT Input = { 0 };
	
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	Input.mi.mouseData = dy * 120;
	Input.mi.time = 0;
	::SendInput(1, &Input, sizeof(INPUT));

	


}

void MoveMouse(float dx, float dy, int horizontal, int vertical)
{

	POINT p;
	GetCursorPos(&p);
	//p.x += dx*22;
	p.x += dx* (horizontal * 0.014);
	p.y -= dy* (vertical *   0.022);
	SetCursorPos(p.x, p.y);
	

}


void GetResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	//Get handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	//Get size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	//The top left corner will have coordinates (0,0)
	//and the bottom right corner will have (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;

}



int main()
{

	// to hide console Window use:   
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
  
	Gamepad gamepad;

	bool wasConnected = true;
	bool keyboardMode = false;
	int  letterCount = -1;
	int  horizontal;
	int  vertical;
    GetResolution(horizontal, vertical);


	//Begin Main Loop
	while (true)
	{
		Sleep(10);

	

		//No Controller found
		if(!gamepad.Refresh())
		{
			if (wasConnected)
				wasConnected = false;

			cout << "Please connect a Controller" << endl;
		
		}

		//Controller connected
		else
		{
			if (!wasConnected)
			{
				wasConnected = true;

				cout << "Controller connected on port " << gamepad.GetPort() << endl;

			}

			cout << "Left thumb stick:  (" << gamepad.leftStickX << ", " << gamepad.leftStickY  
				 << " ) Right thumb stick: (" << gamepad.rightStickX << "," << gamepad.rightStickY << ")" << endl;

			cout << "Left analog trigger: " << gamepad.leftTrigger 
				 << " Right analog trigger: " << gamepad.rightTrigger << endl;


			float LX = gamepad.leftStickX;
			float LY = gamepad.leftStickY;

			float RX = gamepad.rightStickX;
			float RY = gamepad.rightStickY;

			
		

			//Scrolling Up and Down
			if (!keyboardMode)
			{
				bool rightStickUsed = (RY > 0.3f || RY < -0.3f);
				if (rightStickUsed)
				{

					MouseWheel(RY);
				}
			}

			bool leftStickUp = (LY > 0.3f);
			bool leftStickDown = (LY < -0.3f);

			//Mouse Movement
			if(!keyboardMode) MoveMouse(LX, LY, horizontal, vertical);


			
			WORD input = gamepad.GetButtonPressed();

			
			if (keyboardMode)
			{
				
				if (leftStickUp)
				{
					KeyboardInput(BACKSPACE);
					Sleep(100);

					letterCount++;
					if (letterCount < 0)  letterCount = 0;
					if (letterCount > 27) letterCount = 0;
					for (int i = 0; i < 28; i++)
					{
						if (i == 27)
						{
							KeyboardMultipleInput(SHIFT, NUM_7);
							break;
						}
						if (letterCount == i)
						{
							KeyboardInput(Alphabet[i]);
							break;
						}
					}
					Sleep(100);
				}

				if (leftStickDown)
				{
					KeyboardInput(BACKSPACE);
					Sleep(100);



					letterCount--;
					if (letterCount < 0)  letterCount = 27;
					if (letterCount > 27) letterCount = 0;
					for (int i = 0; i < 28; i++)
					{

						if (letterCount == i)
						{
							if (i == 27) //Sonderfall Slash
							{
								KeyboardMultipleInput(SHIFT, NUM_7);
								break;
							}
							KeyboardInput(Alphabet[i]);
							break;
						}
					}
					Sleep(100);
				}
			}


			//React to Controller Input
			{
				switch (input)
				{
					
				case GAMEPAD_A:
					
					cout << "(A) Button pressed" << endl;
					Sleep(170);
					if (keyboardMode)KeyboardInput(ENTER);
					else LeftMouseClick();
					cout << "Resolution: " << horizontal << "x" << vertical << endl;
					break;
				case GAMEPAD_B:
					cout << "(B) Button pressed" << endl;
					if (keyboardMode) KeyboardInput(SPACE);
					else RightMouseClick();
					Sleep(170);
					break;
				case GAMEPAD_X:
					cout << "(X) Button pressed" << endl;
					if (keyboardMode) KeyboardInput(BACKSPACE);
					else KeyboardInput(SPACE);
					Sleep(170);
					break;
				case GAMEPAD_Y:
					if (keyboardMode) KeyboardMultipleInput(CONTROL, A_KEY);
					else return 0;
					break;
				case GAMEPAD_RSHOULDER:
					KeyboardInput(BROWSER_REFRESH);
					Sleep(170);
					break;
				case GAMEPAD_LSHOULDER:
					KeyboardInput(BROWSER_BACK);
					Sleep(170);
					break;
				case GAMEPAD_D_UP:
					if (keyboardMode) KeyboardInput(UP_ARROW);
					else
					{
						for (int i = 0; i < 4; i++)
						{
							KeyboardInput(VOLUME_UP);
						}
					}
					Sleep(170);
					break;
				case GAMEPAD_D_DOWN:
					if (keyboardMode) KeyboardInput(DOWN_ARROW);
					else
					{
						for (int i = 0; i < 4; i++)
						{
							KeyboardInput(VOLUME_DOWN);
						}
					}
					Sleep(170);
					break;
				case GAMEPAD_D_LEFT:
					if(keyboardMode) KeyboardInput(LEFT_ARROW);
					Sleep(170);
					break;
				case GAMEPAD_D_RIGHT:
					if (keyboardMode) KeyboardInput(RIGHT_ARROW);
					KeyboardMultipleInput(SHIFT, N_KEY);
					Sleep(170);
					break;
				
				case GAMEPAD_START:
					KeyboardMultipleInput(ALT, TAB);
					Sleep(170);
					break;
				case GAMEPAD_SELECT:
					KeyboardMultipleInput(CONTROL, T_KEY);
					Sleep(170);
					break;
				case GAMEPAD_LTHUMB: //starts Keyboard Mode
					keyboardMode = true;
					break;
				case GAMEPAD_RTHUMB:
					if(!keyboardMode) KeyboardInput(VOLUME_MUTE);
					keyboardMode = false;
					letterCount = -1;
					Sleep(170);
					break;
				default:
					break;

				//end Main Loop


				}
			}
			
		}
	}

  return 0;
}


