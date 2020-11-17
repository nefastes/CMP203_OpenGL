#pragma once
#include <glut.h>
#include "Vector3.h"
class Skybox
{
private:
	GLuint* texture;
	Vector3 position;

public:
	Skybox();
	~Skybox();

	void setPos(Vector3);
	Vector3& getPos();
	void setTexture(GLuint&);
	void draw();

	//Should not require anything else, as we just need to give it a texture and update its position
	//relative to the camera position.
};

