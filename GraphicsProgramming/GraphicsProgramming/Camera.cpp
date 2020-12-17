#include "Camera.h"
Camera::Camera()
{
	//Init default camera
	speed = 1.f;
	sensitivity = 1.f;
	yaw = 0.f;
	pitch = 0.f;
	roll = 0.f;
	thirdPersonDistanceMultiplier = 5.f;
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
		yaw -= 5.f * sensitivity * dt;
	}
	if (input->isKeyDown('e'))
	{
		yaw += 5.f * sensitivity * dt;
	}
	if (input->isKeyDown('r'))
	{
		pitch += 5.f * sensitivity * dt;
		if (pitch > 95.f) pitch = 95.f;	//Avoid Gimbal lock
	}
	if (input->isKeyDown('f'))
	{
		pitch -= 5.f * sensitivity * dt;
		if (pitch < -95.f) pitch = -95.f; //Avoid Gimbal lock
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
	}
	if (mouseY != center.centerY)
	{
		//We should rotate the camera in pitch since the mouse travelled in Y
		//Only move vertically in the range -0.95 to 0.95 to avoid Gimbal lock
		pitch -= (mouseY - center.centerY) * sensitivity * dt;
		if (pitch < -95.f) pitch = -95.f;
		if (pitch > 95.f) pitch = 95.f;
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

void Camera::setAngles(float yaw, float pitch, float roll)
{
	if (yaw < 0) yaw += 360;
	if (yaw > 360) yaw -= 360;
	if (pitch < 0) pitch += 360;
	if (pitch > 360) pitch -= 360;
	if (roll < 0) roll += 360;
	if (roll > 360) roll -= 360;

	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;
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

Vector3 Camera::getPosition(bool gluLookAt)
{
	//The gluLookAt bool allows us to get a false position when 3rd person is enabled
	//Thanks to that, we can still get the real position when used by other objects (such as the trump model) as well as for the hud element,
	//but send the false third person camera position to the gluLookAt() function and to the skybox position
	if (currentCameraType == CameraTypes::THIRDPERSON &&  gluLookAt)
	{
		return position - forward * thirdPersonDistanceMultiplier;
	}
	else return position;
}

Vector3 Camera::getLookAt()
{
	if (currentCameraType == CameraTypes::THIRDPERSON)
	{
		return position;
	}
	else return position + forward;
}

Vector3 Camera::getUp()
{
	return up;
}