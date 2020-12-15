#include "Light.h"
Light::Light(unsigned type)
{
	this->type = (lightType)type;
	cutoff = 90.f, exponent = 15.f;
	Ambient.fill(0.f);
	Diffuse.fill(0.f);
	Position.fill(0.f);
	Direction.fill(0.f);
	constant_attenuation = 1.f, linear_attenutation = 0.f, quadratic_attenuation = 0.f;
	currentLight = 0;

	//Dectect the first available light and assign as current light
	for (unsigned i = GL_LIGHT0; i <= GL_LIGHT7; ++i)
	{
		if (!glIsEnabled(i))
		{
			glEnable(i);
			currentLight = i;
			return;
		}
	}
	MessageBox(NULL, "Attempted to create too many (more than 8) lights", "Error", MB_OK);
}

Light::~Light()
{

}

void Light::render()
{
	switch (type)
	{
	case lightType::AMBIENT:
		glLightfv(currentLight, GL_AMBIENT, Ambient.data());
	case lightType::DIFFUSE:
	case lightType::POINT:
		break;
	case lightType::SPOT:
		glLightf(currentLight, GL_SPOT_CUTOFF, cutoff);
		glLightfv(currentLight, GL_SPOT_DIRECTION, Direction.data());
		glLightf(currentLight, GL_SPOT_EXPONENT, exponent);
		break;
	}
	glLightfv(currentLight, GL_DIFFUSE, Diffuse.data());
	glLightfv(currentLight, GL_POSITION, Position.data());
	glLightf(currentLight, GL_CONSTANT_ATTENUATION, constant_attenuation);
	glLightf(currentLight, GL_LINEAR_ATTENUATION, linear_attenutation);
	glLightf(currentLight, GL_QUADRATIC_ATTENUATION, quadratic_attenuation);
	glEnable(currentLight);
}

void Light::makeAmbient(GLfloat* ambient)
{
	for (unsigned char i = 0; i < 4; ++i) Ambient[i] = ambient[i];
}

void Light::makeDiffuse(GLfloat* diffuse, GLfloat* position, float constant_attenuation, float linear_attenutation, float quadratic_attenuation)
{
	for (unsigned char i = 0; i < 4; ++i) Diffuse[i] = diffuse[i];
	for (unsigned char i = 0; i < 4; ++i) Position[i] = position[i];
	this->constant_attenuation = constant_attenuation;
	this->linear_attenutation = linear_attenutation;
	this->quadratic_attenuation = quadratic_attenuation;
}

void Light::makeSpot(GLfloat* diffuse, GLfloat* position, GLfloat* direction, float cutoff, float exponent, float constant_attenuation, float linear_attenutation, float quadratic_attenuation)
{
	for (unsigned char i = 0; i < 4; ++i) Diffuse[i] = diffuse[i];
	for (unsigned char i = 0; i < 4; ++i) Position[i] = position[i];
	for (unsigned char i = 0; i < 4; ++i) Direction	[i] = direction[i];
	this->cutoff = cutoff;
	this->exponent = exponent;
	this->constant_attenuation = constant_attenuation;
	this->linear_attenutation = linear_attenutation;
	this->quadratic_attenuation = quadratic_attenuation;
}

void Light::setCutoff(float cutoff)
{
	this->cutoff = cutoff;
}

void Light::setExponent(float exponent)
{
	this->exponent = exponent;
}

void Light::setConstantAttenuation(float constant_attenuation)
{
	this->constant_attenuation = constant_attenuation;
}

void Light::setLinearAttenuation(float linear_attenuation)
{
	this->linear_attenutation = linear_attenuation;
}

void Light::setQuadraticAttenuation(float quadratic_attenuation)
{
	this->quadratic_attenuation = quadratic_attenuation;
}

void Light::setAttenuations(float constant_attenuation, float linear_attenutation, float quadratic_attenuation)
{
	this->constant_attenuation = constant_attenuation;
	this->linear_attenutation = linear_attenutation;
	this->quadratic_attenuation = quadratic_attenuation;
}

void Light::enable()
{
	glEnable(currentLight);
}

void Light::disable()
{
	glDisable(currentLight);
}

bool Light::isEnabled()
{
	return glIsEnabled(currentLight);
}

void Light::setPosition(GLfloat* position)
{
	for (unsigned char i = 0; i < 4; ++i) Position[i] = position[i];
}

GLfloat* Light::getPosition()
{
	return Position.data();
}