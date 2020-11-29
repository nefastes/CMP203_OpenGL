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
	//Color the shape with its color
	glColor3f(red, green, blue);
	//Render using ordered arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordinates.data());
	shapeSpecificDrawingMode();	//Call the shape's specific render, will differ for every sort of shape.
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//If a texture has been applied, remove it
	if (texture != nullptr) glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//Reset color
	glColor4f(1.f, 1.f, 1.f, 1.f);
}

void BasicShape::shapeSpecificDrawingMode()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (transparent) glColor4f(red, green, blue, alpha);
	if (renderInside)
	{
		glPushMatrix();
		glCullFace(GL_FRONT);
		glDrawArrays(GL_QUADS, 0, 24);
		glCullFace(GL_BACK);
		glPopMatrix();
	}
	glDrawArrays(GL_QUADS, 0, 24);
}

void BasicShape::generateShape()
{
	//Clear vectors in case of regeneration
	vertices.clear();
	normals.clear();
	texCoordinates.clear();

	//TODO: Make things smarter here
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
	//Generate normals for a unit cube
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
	//Generate texture coordinates if we want a filling texture
	if (fillTexture)
	{
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
	//Generate texture coordinates if we want a repeating texture
	else
	{
		float texcoords[48] = {
		verts[0] / textureScale.x, verts[1] / textureScale.y,
		verts[3] / textureScale.x, verts[4] / textureScale.y,
		verts[6] / textureScale.x, verts[7] / textureScale.y,
		verts[9] / textureScale.x, verts[10] / textureScale.y,

		verts[13] / textureScale.y, verts[14] / textureScale.z,
		verts[16] / textureScale.y, verts[17] / textureScale.z,
		verts[19] / textureScale.y, verts[20] / textureScale.z,
		verts[22] / textureScale.y, verts[23] / textureScale.z,

		verts[24] / textureScale.x, verts[25] / textureScale.y,
		verts[27] / textureScale.x, verts[28] / textureScale.y,
		verts[30] / textureScale.x, verts[31] / textureScale.y,
		verts[33] / textureScale.x, verts[34] / textureScale.y,

		verts[37] / textureScale.y, verts[38] / textureScale.z,
		verts[40] / textureScale.y, verts[41] / textureScale.z,
		verts[43] / textureScale.y, verts[44] / textureScale.z,
		verts[46] / textureScale.y, verts[47] / textureScale.z,

		verts[48] / textureScale.x, verts[50] / textureScale.z,
		verts[51] / textureScale.x, verts[53] / textureScale.z,
		verts[54] / textureScale.x, verts[56] / textureScale.z,
		verts[57] / textureScale.x, verts[59] / textureScale.z,

		verts[60] / textureScale.x, verts[62] / textureScale.z,
		verts[63] / textureScale.x, verts[65] / textureScale.z,
		verts[66] / textureScale.x, verts[68] / textureScale.z,
		verts[69] / textureScale.x, verts[71] / textureScale.z,
		};
		for (unsigned i = 0; i < 48; ++i) texCoordinates.push_back(texcoords[i]);
	}
}