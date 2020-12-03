#pragma once
#include <glut.h>
#include "Vector3.h"
#include <array>
class Skybox
{
private:
	GLuint* texture;
	Vector3 position;

	std::array<GLfloat, 4> Material_Ambient{ 1.f, 1.f, 1.f, 1.f };
	std::array<GLfloat, 4> Material_Diffuse{ 0.f, 0.f, 0.f, 0.f };

public:
	Skybox();
	~Skybox();

	void setPos(Vector3);
	Vector3& getPos();
	void setTexture(GLuint&);
	void draw();

	//Declare the vertices, normals and texture coordinates to make a unit cube skybox
	float verts[72] = {
		//Front
		1.0,  1.0, 1.0,
		1.0,  -1.0, 1.0,
		-1.0, -1.0, 1.0,
		-1.0, 1.0, 1.0,

		//Right
		1.0, 1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,

		//Back
		-1.0, 1.0, -1.0,
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, 1.0, -1.0,

		//Left
		-1.0, 1.0, 1.0,
		-1.0, -1.0, 1.0,
		-1.0, -1.0, -1.0,
		-1.0, 1.0, -1.0,

		//Top
		1.0, 1.0, -1.0,
		1.0, 1.0, 1.0,
		-1.0, 1.0, 1.0,
		-1.0, 1.0, -1.0,

		//Bottom
		1.0, -1.0, 1.0,
		1.0, -1.0, -1.0,
		-1.0, -1.0, -1.0,
		-1.0, -1.0, 1.0
	};

	float norms[72] = {
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,

		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,

		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,

		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,

		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,

		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0
	};


	float texcoords[48] = {
		.5f, .25f,
		.5f, .5f,
		.25f, .5f,
		.25f, .25f,

		.75f, .25f,
		.75f, .5f,
		.5f, .5f,
		.5f, .25f,

		1.f, .25f,
		1.f, .5f,
		.75f, .5f,
		.75f, .25f,

		.25f, .25f,
		.25f, .5f,
		0.0, .5f,
		0.0, .25f,

		.5f, 0.f,
		.5f, .25f,
		.25f, .25f,
		.25f, 0.f,

		.5f, .5f,
		.5f, .75f,
		.25f, .75f,
		.25f, .5f
	};

	//Should not require anything else, as we just need to give it a texture and update its position
	//relative to the camera position.
};

