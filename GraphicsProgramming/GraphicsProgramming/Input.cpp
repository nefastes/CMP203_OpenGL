// Input class
// Stores current keyboard and mouse state include, pressed keys, mouse button pressed and mouse position.
// @author Paul Robertson

#include "Input.h"

Input::Input()
{
	// set default values
	mouse.left = false;
	mouse.right = false;
	mouse.x = 0;
	mouse.y = 0;
}

void Input::setKeyDown(unsigned char key)
{
	if (key >= 0)
	{
		keys[key] = true;
	}
}

void Input::setKeyUp(unsigned char key)
{
	if (key >= 0)
	{
		keys[key] = false;
	}
}

bool Input::isKeyDown(int key)
{
	if (key >= 0)
	{
		return keys[key];
	}
	return false;
}

bool Input::isKeyDownOnce(int key)
{
	//Returns true if a key is pressed and was released the frame before
	if (released[key])
		return keys[key];
	return false;
}

void Input::updatePreviousFrameKeys()
{
	//Store the inverse of the keys array
	for (unsigned i = 0; i < 256; ++i) released[i] = !keys[i];
}

void Input::setMouseX(int pos)
{
	mouse.x = pos;
}

void Input::setMouseY(int pos)
{
	mouse.y = pos;
}

void Input::setMousePos(int ix, int iy)
{
	mouse.x = ix;
	mouse.y = iy;
}

int Input::getMouseX()
{
	return mouse.x;
}

int Input:: getMouseY()
{
	return mouse.y;
}

void Input::setMouseLDown(bool b)
{
	mouse.left = b;
}

bool Input::isMouseLDown()
{
	return mouse.left;
}

void Input::setMouseRDown(bool down)
{
	mouse.right = down;
}
bool Input::isMouseRDown()
{
	return mouse.right;
}