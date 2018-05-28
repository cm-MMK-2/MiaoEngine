#pragma once
// GLFW
#include <GLFW/glfw3.h>

#include "MInputState.hpp"
#include <iostream>
#define MINPUT_KEYBOARD 0x01
#define MINPUT_MOUSE 0x02
#define MINPUT_JOYSTICK 0x04
#define MINPUT_ALL 0x07
typedef std::function<void(uint32_t)> KeyEvent;


class MMouseInput
{
public:
	double x;
	double y;

};

/*
 * Input Module
 * keyboard input
 * mouse input
 * joystick
 */
class MInput
{
public:
	static MInput* Init(GLFWwindow* window, int modules) {
		if (!instance) {
			instance = new MInput(window, modules);
		}
		return instance;
	}

	static void Dispose() {
		if (instance) {
			delete instance;
		}
	}

	static MInput* Get() {
		return instance;
	}

	static KeyEvent OnKeyDown;
	static KeyEvent OnKeyUp;

	static void SetKeyEventListener(KeyEvent onKeyDown, KeyEvent onKeyUp)
	{
		OnKeyDown = onKeyDown;
		OnKeyUp = onKeyUp;
	}

private:

	static MInput* instance;

	MInput(GLFWwindow* window, int modules)
	{
		using namespace std;
		if ((modules & MINPUT_KEYBOARD) == MINPUT_KEYBOARD)
		{
			cout << "DEBUG::MInput:Init keyboard input" << endl;
			glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
				if (action == GLFW_PRESS)//key == GLFW_KEY_E  //GLFW_RELEASE
				{
					for (auto& keyPair : MInputState::keySet)
					{
						if (keyPair.second == key)
						{
							MInputState::keyStates[keyPair.first] = true;
							if (nullptr != OnKeyDown)
							{
								OnKeyDown(keyPair.first);
							}

							cout << "DEBUG::MInput:Defined Key[" << keyPair.first << "] pressed" << endl;
							break;
						}
					}
					cout << "DEBUG::MInput:" << key << " pressed" << endl;
				}
				else if (action == GLFW_RELEASE)
				{
					for (auto& keyPair : MInputState::keySet)
					{
						if (keyPair.second == key)
						{
							MInputState::keyStates[keyPair.first] = false;
							if (nullptr != OnKeyUp)
							{
								OnKeyUp(keyPair.first);
							}
							cout << "DEBUG::MInput:Defined Key[" << keyPair.first << "] released" << endl;
							break;
						}
					}
					cout << "DEBUG::MInput:" << key << " released" << endl;
				}
			});
		}

		if ((modules & MINPUT_MOUSE) == MINPUT_MOUSE)
		{
			cout << "DEBUG::MInput:Init mouse input" << endl;
			mouseInput = new MMouseInput();
			//cursor move
			glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
			{
				cout << "DEBUG::MInput:Mouse move to " << xpos << ", " << ypos << endl;
			});
			//mouse button
			glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)//GLFW_RELEASE
				{
					cout << "DEBUG::MInput:Left button pressed" << endl;
				}

			});
			//mouse scroll
			glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				cout << "DEBUG::MInput:Scroll:" << xoffset << "," << yoffset << endl;
			});
		}
		else
		{
			cout << "DEBUG::MInput:Hide mouse cursor" << endl;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		if ((modules & MINPUT_JOYSTICK) == MINPUT_JOYSTICK)
		{
			cout << "DEBUG::MInput:Init joystick input" << endl;
			//init 
			int axesCount;
			int buttonCount;
			if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE)
			{
				axesPos = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
				buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
				cout << "DEBUG::MInput:Joystick has " << axesCount << " axes and " << buttonCount << " buttons in total" << endl;
			}
			else
			{
				//new joystick
				glfwSetJoystickCallback([](int joy, int event)
				{
					if (event == GLFW_CONNECTED)
					{
						cout << "DEBUG::MInput:New joystick connected" << endl;
					}
				});
			}
		}
	}

	~MInput()
	{
		if (mouseInput)
		{
			delete mouseInput;
		}
	}

	MMouseInput* mouseInput = nullptr;
	const float* axesPos;
	const unsigned char* buttons;
};

MInput* MInput::instance = nullptr;
KeyEvent MInput::OnKeyDown = nullptr;
KeyEvent MInput::OnKeyUp = nullptr;