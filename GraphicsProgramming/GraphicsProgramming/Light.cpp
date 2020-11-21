#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

void Light::makeAmbient(std::array<GLfloat, 4> rgba)
{
	//Creates an ambient light
	//Find an available light spot
	if (findAvailableLight() && !lightCreated)
	{
		glLightfv(currentLight, GL_AMBIENT, rgba.data());
		glEnable(currentLight);
		lightCreated = true;
	}
	else; // TODO: add error message here to say it failed to create a light cause all spots are occupied
}

void Light::makeDiffuse(std::array<GLfloat, 4> rgba, std::array<GLfloat, 4> position)
{
	//Creates a diffuse or point light
	//Find an available light spot
	if (findAvailableLight() && !lightCreated)
	{
		glLightfv(currentLight, GL_DIFFUSE, rgba.data());
		glLightfv(currentLight, GL_POSITION, position.data());
		glEnable(currentLight);
		lightCreated = true;
	}
	else; // TODO: add error message here to say it failed to create a light cause all spots are occupied
}

void Light::makeSpot(std::array<GLfloat, 4> rgba, std::array<GLfloat, 4> position, std::array<GLfloat, 3> direction, float cutoff, float exponent)
{
	//Creates a spotlight
	//Find an available light spot
	if (findAvailableLight() && !lightCreated)
	{
		glLightfv(currentLight, GL_DIFFUSE, rgba.data());
		glLightfv(currentLight, GL_POSITION, position.data());
		glLightf(currentLight, GL_SPOT_CUTOFF, cutoff);
		glLightfv(currentLight, GL_SPOT_DIRECTION, direction.data());
		glLightf(currentLight, GL_SPOT_EXPONENT, exponent);
		glEnable(currentLight);
		lightCreated = true;
	}
	else; // TODO: add error message here to say it failed to create a light cause all spots are occupied
}

bool Light::findAvailableLight()
{
	for (short unsigned i = 0; i < 8; ++i)
		if (!glIsEnabled(lights[i]))
		{
			currentLight = lights[i];
			return true;
		}
	return false;	//All lights are activated
}

void Light::enable()
{
	glEnable(currentLight);
}

void Light::disable()
{
	glDisable(currentLight);
}
