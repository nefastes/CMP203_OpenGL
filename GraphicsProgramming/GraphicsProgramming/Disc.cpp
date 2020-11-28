#include "Disc.h"

Disc::Disc()
{
	//Default variables
	radius = 1;
	resolution = 3;
}

Disc::~Disc()
{

}

void Disc::render()
{
	//Texture the shape if a texture exists
	if (texture != nullptr) glBindTexture(GL_TEXTURE_2D, *texture);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texCoordinates[0]);

	if (transparent)
	{
		glPushMatrix();
		glColor4f(red, green, blue, alpha);
		glScalef(-1.f, -1.f, -1.f);
		glDrawArrays(GL_TRIANGLES, 0, 3 * resolution);
		glPopMatrix();
	}
	glDrawArrays(GL_TRIANGLES, 0, 3 * resolution);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//If a texture has been applied, remove it
	if (texture != nullptr) glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//Reset color
	glColor4f(1.f, 1.f, 1.f, 1.f);
}

void Disc::generateShape()
{
	//The least resolution contains 3 vertices to make 1 triangle
	if (resolution < 3) resolution = 3;
	//Calculate dtheta
	float deltaTheta = 2.f * PI / resolution;
	//Declare the starting theta
	float theta = 0;
	for (unsigned j = 0; j < resolution; ++j)
	{
		//Center of disc
		for (unsigned i = 0; i < 3; ++i) i == 2 ? vertices.push_back(1.f) : vertices.push_back(0.f);
		for (unsigned i = 0; i < 3; ++i) i == 2 ? normals.push_back(1.f) : normals.push_back(0.f);
		for (unsigned i = 0; i < 2; ++i) texCoordinates.push_back(.5f);

		//Calculate 2 vertices, normals and tex coords to form a triangle
		for (unsigned i = 0; i < 2; ++i)
		{
			//Vertices and normals need 3 coordinates
			vertices.push_back(radius * cosf(theta));
			vertices.push_back(radius * sinf(theta));
			vertices.push_back(1.f);
			normals.push_back(0.f);
			normals.push_back(0.f);
			normals.push_back(1.f);
			//TexCoords need 2 coordinates
			texCoordinates.push_back(cosf(theta) / (2 * radius) + .5f);
			texCoordinates.push_back(-sinf(theta) / (2 * radius) + .5f);
			//Increment theta by dtheta for the next position
			if (i == 0)
			{
				if (j == resolution - 1) theta = 0;
				else theta += deltaTheta;
			}
		}
	}
}

void Disc::setRadius(float r)
{
	radius = r;
}

void Disc::setResolution(unsigned rez)
{
	resolution = rez;
}

float Disc::getRadius()
{
	return radius;
}

unsigned Disc::getResolution()
{
	return resolution;
}