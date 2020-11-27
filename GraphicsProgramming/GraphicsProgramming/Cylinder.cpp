#include "Cylinder.h"
Cylinder::Cylinder()
{
	radius = 1;
	stackResolution = 2;
	resolution = 3;
}

Cylinder::~Cylinder()
{

}

void Cylinder::render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texCoordinates[0]);

	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_BYTE, &indices[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Cylinder::generateShape()
{
	//We need a minimum of 2 stacks, so we set this to 1 so we make 1 more disc
	if (stackResolution < 1) stackResolution = 1;
	//We need a minimum of 3 vertices to make a triangle
	if (resolution < 3) resolution = 3;

	//Calculate dtheta
	float deltaTheta = 2.f * PI / resolution;
	//Declare the starting theta
	float theta = 0;
	//Sample the points for a disc for as many times as we have stacks
	for (float height = 0.f; height <= stackResolution; height += 1.f)
	{
		for (unsigned j = 0; j <= resolution; ++j)
		{
			//Vertices and normals need 3 coordinates
			vertices.push_back(radius * cosf(theta));
			vertices.push_back(0.f + height);
			vertices.push_back(radius * sinf(theta));
			normals.push_back(cosf(theta));
			normals.push_back(0.f);
			normals.push_back(sinf(theta));
			//TexCoords need 2 coordinates
			texCoordinates.push_back(1 - ((float)j / resolution));
			texCoordinates.push_back(1 - (height / stackResolution));
			//Increment theta by dtheta for the next position
			if(j == resolution - 1) theta = 0;
			else theta += deltaTheta;
		}
		//We will start a new disc, reset theta
		theta = 0;
	}
	//Set up the indices to make up quads
	//In the vertices vector we have [disc 1 vertices][disc 2 vertices][etc.]
	//So we need to do disc2[0]disc1[0]disc1[1]disc2[1] for a quad to render the interior quad,
	//and disc2[1]disc1[1]disc1[0]disc2[0] to render the exterior quads
	//Note to self: wouldn't work because for a disc resolution of 3, we're actually creating 4 vertices in order for the texture to be matched properly.
	//So here we needed to consider the discResolution with + 1 since we dont have 3 but 4 vertices for each disc!
	for (unsigned i = 0; i < stackResolution; ++i)
	{
		for (unsigned j = 0; j < resolution; ++j)
		{
			indices.push_back((i + 1) * (resolution + 1) + (j + 1));
			indices.push_back(i * (resolution + 1) + (j + 1));
			indices.push_back(i * (resolution + 1) + j);
			indices.push_back((i + 1) * (resolution + 1) + j);
		}
	}
}

void Cylinder::setStackResolution(unsigned r)
{
	//Minus one so that we get as many discs as the resolution
	stackResolution = r - 1;
}

unsigned Cylinder::getStackResolution()
{
	return stackResolution;
}