#pragma once
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <array>
class Light
{
private:
	bool debug = false;
	//We only want 1 light per object, use this tracker to prevent creating more lights on the same object
	bool lightCreated = false;
	//Tracker for the amount of lights
	GLenum currentLight = GL_LIGHT0;
	GLenum lights[8] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };
	//Function to determine if there is an available (disabled light)
	bool findAvailableLight();

public:
	Light();
	~Light();

	void makeAmbient(std::array<GLfloat, 4> rgba);
	void makeDiffuse(std::array<GLfloat, 4> rgba, std::array<GLfloat, 4> position);
	void makeSpot(std::array<GLfloat, 4> rgba, std::array<GLfloat, 4> position, std::array<GLfloat, 3> direction, float cutoff, float exponent);

	void enable();
	void disable();

	void setDebugging(bool d) { debug = d; }
	bool isDebugging() { return debug; }

};

