#pragma once
#include <glut.h>
#include <array>
class Light
{
private:
	unsigned currentLight;
	enum class lightType { AMBIENT, DIFFUSE, POINT, SPOT };
	lightType type;
	std::array<GLfloat, 4> Ambient;
	std::array<GLfloat, 4> Diffuse;
	std::array<GLfloat, 4> Position;
	std::array<GLfloat, 4> Direction;
	float cutoff, exponent;
	float constant_attenuation, linear_attenutation, quadratic_attenuation;

public:
	Light(unsigned type);
	~Light();
	
	void render();

	void makeAmbient(GLfloat* ambient, GLfloat* diffuse, float constant_attenuation, float linear_attenutation, float quadratic_attenuation);
	void makeDiffuse(GLfloat* diffuse, GLfloat* position, float constant_attenuation, float linear_attenutation, float quadratic_attenuation);
	void makeSpot(GLfloat* diffuse, GLfloat* position, GLfloat* direction, float cutoff, float exponent, float constant_attenuation, float linear_attenutation, float quadratic_attenuation);

	void setCutoff(float cutoff);
	void setExponent(float exponent);
	void setConstantAttenuation(float constant_attenuation);
	void setLinearAttenuation(float linear_attenuation);
	void setQuadraticAttenuation(float quadratic_attenuation);
	void setAttenuations(float constant_attenuation, float linear_attenutation, float quadratic_attenuation);

	void enable();
	void disable();
	bool isEnabled();

	void setPosition(GLfloat* position);
	GLfloat* getPosition();
};

