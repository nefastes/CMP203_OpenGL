#include "Sphere.h"
Sphere::Sphere()
{
	radius = 1;
	resolution = 3;
}

Sphere::~Sphere()
{

}

void Sphere::render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texCoordinates[0]);

	glDrawArrays(GL_QUADS, 0, 4 * resolution * resolution);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Sphere::generateShape()
{
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

				theta = latitude * deltaTheta;
				gamma = longitude * deltaGamma;

				//Calaculate normals coordinates (need to do it here since we're also using the values with vertices, avoid calculating it again)
				float x = cosf(theta) * sinf(gamma);
				float y = cosf(gamma);
				float z = sinf(theta) * sinf(gamma);
				//Vertices and normals need 3 coordinates
				vertices.push_back(radius * x);
				vertices.push_back(radius * y);
				vertices.push_back(radius * z);
				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);
				//TexCoords need 2 coordinates
				texCoordinates.push_back((float)longitude / resolution);
				texCoordinates.push_back((float)latitude / resolution);
			}
			//Since it has been incremented, set the longitude back to its original value
			--longitude;
		}
	}
}