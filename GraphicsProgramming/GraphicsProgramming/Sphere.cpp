#include "Sphere.h"
Sphere::Sphere()
{
	radius = 1;
	resolution = 3;
}

Sphere::~Sphere()
{

}

void Sphere::shapeSpecificDrawingMode()
{
	if (renderInside)
	{
		glCullFace(GL_FRONT);
		glNormalPointer(GL_FLOAT, 0, invertedNormals.data());
		glDrawArrays(GL_QUADS, 0, 4 * resolution * resolution);
		glNormalPointer(GL_FLOAT, 0, normals.data());
		glCullFace(GL_BACK);
	}
	glDrawArrays(GL_QUADS, 0, 4 * resolution * resolution);
}

void Sphere::generateShape()
{
	//Clear vectors in case of regeneration
	vertices.clear();
	normals.clear();
	texCoordinates.clear();

	//The least resolution contains 3 layers of both lats and longs to make a sphere
	if (resolution < 3) resolution = 3;
	//Calculate deltaAngles
	float deltaTheta = 2.f * PI / resolution;
	float deltaGamma = PI / resolution;
	//Declare the starting theta
	float theta = 0;
	float gamma = 0;
	for (unsigned longitude = 0; longitude < resolution;++longitude)
	{
		for (unsigned latitude = 0; latitude < resolution; ++latitude)
		{
			//We need to calculate 4 vertices to make a quad, as well as its normals and uv coords
			//We want to have: vertex[lat][long], vertex[lat+1][long], vertex[lat+1][long+1], vertex[lat][long+1]

			for (unsigned i = 0; i < 4; ++i)
			{
				switch (i)
				{
				case 1:		++latitude;		break;
				case 2:		++longitude;	break;
				case 3:		--latitude;		break;
				}

				//Need this to make sure the last vertices match with the first ones, avoiding a little transparent artefact 
				latitude == resolution ? theta = 0 : theta = latitude * deltaTheta;
				gamma = longitude * deltaGamma;

				//Calaculate normals coordinates (need to do it here since we're also using the values with vertices, avoid calculating it again)
				float x = cosf(theta) * sinf(gamma);
				float y = cosf(gamma);
				float z = sinf(theta) * sinf(gamma);
				//Vertices and normals need 3 coordinates, also consider the origin  so we can build the shape in the correct position
				vertices.push_back(radius * x * scale.x + origin.x);
				vertices.push_back(radius * y * scale.y + origin.y);
				vertices.push_back(radius * z * scale.z + origin.z);
				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);
				//TexCoords need 2 coordinates
				latitude == resolution ? texCoordinates.push_back(1.f) : texCoordinates.push_back((float)latitude / resolution);
				longitude == resolution ? texCoordinates.push_back(1.f) : texCoordinates.push_back((float)longitude / resolution);
			}
			//Since it has been incremented, set the longitude back to its original value
			--longitude;
		}
	}
	//Assign the inverted normals
	for (unsigned i = 0; i < normals.size(); ++i) invertedNormals.push_back(normals[i] * -1.f);
}