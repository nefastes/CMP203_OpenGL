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

void Disc::shapeSpecificDrawingMode()
{
	if (transparent) glColor4f(red, green, blue, alpha);
	glDrawArrays(GL_TRIANGLES, 0, 3 * resolution);
}

void Disc::generateShape()
{
	//Clear vectors in case of regeneration
	vertices.clear();
	normals.clear();
	texCoordinates.clear();

	//The least resolution contains 3 vertices to make 1 triangle
	if (resolution < 3) resolution = 3;
	//Calculate dtheta
	float deltaTheta = 2.f * PI / resolution;
	//Declare the starting theta
	float theta = 0;
	for (unsigned j = 0; j < resolution; ++j)
	{
		//Center of disc
		vertices.push_back(origin.x);
		vertices.push_back(origin.y);
		vertices.push_back(origin.z);
		for (unsigned i = 0; i < 3; ++i) i == 2 ? normals.push_back(1.f) : normals.push_back(0.f);
		for (unsigned i = 0; i < 2; ++i) texCoordinates.push_back(.5f);

		//Calculate 2 vertices, normals and tex coords to form a triangle
		for (unsigned i = 0; i < 2; ++i)
		{
			//Vertices and normals need 3 coordinates
			vertices.push_back(radius * cosf(theta) * scale.x + origin.x);
			vertices.push_back(radius * sinf(theta) * scale.y + origin.y);
			vertices.push_back(origin.z);
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