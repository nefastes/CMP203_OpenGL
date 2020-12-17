#pragma once
#include "Vector3.h"
#include "Input.h"
#include "glut.h"
#define PI 3.1415

//Camera type enum, to keep track of our current cameratype
enum class CameraTypes { FREE, THIRDPERSON, TRACKING };

class Camera
{
public:
	Camera();
	~Camera();

	void handleInput(float dt);
	void update();
	void updateSide();

	void setSpeed(float s);
	void setSensitivity(float s);
	void setPosition(Vector3);
	void setLookAt(Vector3);
	void setUp(Vector3);

	float getSpeed();
	float getSensitivity();
	float getYaw();
	float getPitch();
	float getRoll();
	Vector3 getPosition();
	Vector3 getLookAt();
	Vector3 getUp();

	void setInput(Input* in) { input = in; }
	CameraTypes* getTypePointer() { return &currentCameraType; }

	//Set the center to a point x,y and make sure the mouse is also placed at said center
	//It's important to also set it to the input class since glutWarpPointer does not update it itself, would end up having some sort of random displacement of mouse
	//each time the application starts / the window being resized.
	void setCenterWindow(int x, int y) { center.centerX = x; center.centerY = y; glutWarpPointer(center.centerX, center.centerY); input->setMousePos(center.centerX, center.centerY); }

private:
	//Camera settings
	float speed;
	float sensitivity;

	//Camera essentials
	float yaw;
	float pitch;
	float roll;
	Vector3 position;
	Vector3 forward;
	Vector3 up;
	Vector3 side;

	//Input pointer
	Input* input;

	//Track the center of the window for the mouse
	struct centerWindow {
		int centerX = 640, centerY = 360; //Default values from main()
	};
	centerWindow center;

	//Camera type tracker
	CameraTypes currentCameraType = CameraTypes::FREE;
};

