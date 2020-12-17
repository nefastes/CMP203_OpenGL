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

void Cylinder::shapeSpecificDrawingMode()
{
	if (renderInside)
	{
		glCullFace(GL_FRONT);
		glNormalPointer(GL_FLOAT, 0, invertedNormals.data());
		glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, indices.data());
		glCullFace(GL_BACK);
		glNormalPointer(GL_FLOAT, 0, normals.data());
	}
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, indices.data());
}

void Cylinder::generateShape()
{
	//Clear vectors in case of regeneration
	vertices.clear();
	normals.clear();
	texCoordinates.clear();

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
			vertices.push_back(radius * cosf(theta) * scale.x + origin.x);
			vertices.push_back((height - (float)stackResolution / 2.f) * scale.y + origin.y);	//We want the origin the be the center of the shape
			vertices.push_back(radius * sinf(theta) * scale.z + origin.z);
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

	//Assign the inverted normals
	for (unsigned i = 0; i < normals.size(); ++i) invertedNormals.push_back(normals[i] * -1.f);
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