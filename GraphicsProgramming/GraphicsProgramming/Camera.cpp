#include "Camera.h"
Camera::Camera()
{
	//Init default camera
	speed = 1.f;
	sensitivity = 1.f;
	yaw = 0.f;
	pitch = 0.f;
	roll = 0.f;
	update();
}

Camera::~Camera()
{

}

void Camera::handleInput(float dt)
{
	//Keyboard inputs
	if (input->isKeyDown('w'))	position += forward * speed * dt;
	if (input->isKeyDown('a'))	position -= side * speed * dt;
	if (input->isKeyDown('s'))	position -= forward * speed * dt;
	if (input->isKeyDown('d'))	position += side * speed * dt;
	if (input->isKeyDown('1'))	position -= up * speed * dt;
	if (input->isKeyDown('2'))	position += up * speed * dt;
	if (input->isKeyDown('q'))
	{
		yaw -= sensitivity * dt;
		update();
	}
	if (input->isKeyDown('e'))
	{
		yaw += sensitivity * dt;
		update();
	}
	if (input->isKeyDown('r'))
	{
		pitch += sensitivity * dt;
		if (pitch > 95.f) pitch = 95.f;	//Avoid Gimbal lock
		update();
	}
	if (input->isKeyDown('f'))
	{
		pitch -= sensitivity * dt;
		if (pitch < -95.f) pitch = -95.f; //Avoid Gimbal lock
		update();
	}

	//Mouse inputs
	//Get the values once
	int mouseX = input->getMouseX();
	int mouseY = input->getMouseY();
	//Determine whether a mouvement of mouse has been made
	if (mouseX != center.centerX)
	{
		//We should rotate the camera in yaw since the mouse travelled in X
		yaw += (mouseX - center.centerX) * sensitivity * dt;
		update();
	}
	if (mouseY != center.centerY)
	{
		//We should rotate the camera in pitch since the mouse travelled in Y
		//Only move vertically in the range -0.95 to 0.95 to avoid Gimbal lock
		pitch -= (mouseY - center.centerY) * sensitivity * dt;
		if (pitch < -95.f) pitch = -95.f;
		if (pitch > 95.f) pitch = 95.f;
		update();
	}
	//Leave the yaw in-between 0 - 360 degrees
	if (yaw < 0) yaw += 360;
	if (yaw > 360) yaw -= 360;

	//Set the mouse to the center of the screen every frame
	glutWarpPointer(center.centerX, center.centerY);
}

void Camera::update()
{
	float sinY = sinf(yaw * PI / 180);
	float sinP = sinf(pitch * PI / 180);
	float sinR = sinf(roll * PI / 180);
	float cosY = cosf(yaw * PI / 180);
	float cosP = cosf(pitch * PI / 180);
	float cosR = cosf(roll * PI / 180);

	//Update the forward vector
	forward.x = sinY * cosP;
	forward.y = sinP;
	forward.z = cosP * -cosY;

	//Update the up vector
	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY * sinR - sinP * cosR * -cosY;

	//Update the side vector
	updateSide();
}

void Camera::updateSide()
{
	//Update the side vector
	side = forward.cross(up);
}

void Camera::setSpeed(float s)
{
	speed = s;
}

void Camera::setSensitivity(float s)
{
	sensitivity = s;
}

void Camera::setPosition(Vector3 pos)
{
	position = pos;
}

void Camera::setLookAt(Vector3 dir)
{
	forward = dir;
	updateSide();
}

void Camera::setUp(Vector3 u)
{
	up = u;
	updateSide();
}

float Camera::getSpeed()
{
	return speed;
}

float Camera::getSensitivity()
{
	return sensitivity;
}

float Camera::getYaw()
{
	return yaw;
}

float Camera::getPitch()
{
	return pitch;
}

float Camera::getRoll()
{
	return roll;
}

Vector3 Camera::getPosition()
{
	return position;
}

Vector3 Camera::getLookAt()
{
	return position + forward;
}

Vector3 Camera::getUp()
{
	return up;
}