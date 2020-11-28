#include "BasicShape.h"
BasicShape::BasicShape()
{

}

BasicShape::~BasicShape()
{

}

void BasicShape::render()
{
	//Texture the shape if a texture exists
	if (texture != nullptr) glBindTexture(GL_TEXTURE_2D, *texture);
	//Render using ordered arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordinates.data());
	//Handle transparency (render the object twice, once inside out and a second time normally)
	if (transparent)
	{
		glColor4f(red, green, blue, alpha);
		if (renderInside)
		{
			glPushMatrix();
			glScalef(-1.f, -1.f, -1.f);
			glTranslatef(-2.f * origin.x, -2.f * origin.y, -2.f * origin.z);
			glDrawArrays(GL_QUADS, 0, 24);
			glPopMatrix();
		}
	}
	glDrawArrays(GL_QUADS, 0, 24);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//If a texture has been applied, remove it
	if (texture != nullptr) glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//Reset color
	glColor4f(1.f, 1.f, 1.f, 1.f);
}

void BasicShape::generateShape()
{
	//Clear vectors in case of regeneration
	vertices.clear();
	normals.clear();
	texCoordinates.clear();

	//Generate vertices for a unit cube
	float verts[72] = {
		//Front
		-1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,
		-1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,
		1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,
		1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,

		//Right
		1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,
		1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,
		1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,
		1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,

		//Back
		1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,
		1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,
		-1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,
		-1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,

		//Left
		-1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,
		-1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,
		-1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,
		-1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,

		//Top
		-1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,
		-1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,
		1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,
		1.0 * scale.x + origin.x,		1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,

		//Bottom
		-1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z,
		-1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,
		1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		-1.0 * scale.z + origin.z,
		1.0 * scale.x + origin.x,		-1.0 * scale.y + origin.y,		1.0 * scale.z + origin.z
	};
	for (unsigned i = 0; i < 72; ++i) vertices.push_back(verts[i]);
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
	for (unsigned i = 0; i < 72; ++i) normals.push_back(norms[i]);
	float texcoords[48] = {
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f
	};
	for (unsigned i = 0; i < 48; ++i) texCoordinates.push_back(texcoords[i]);
}