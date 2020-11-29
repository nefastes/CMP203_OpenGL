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
	//Texture the shape if a texture exists
	if (texture != nullptr) glBindTexture(GL_TEXTURE_2D, *texture);
	//Color the shape with its color
	glColor3f(red, green, blue);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texCoordinates[0]); 

	if (transparent) glColor4f(red, green, blue, alpha);
	if (renderInside)
	{
		glPushMatrix();
		glScalef(-1.f, -1.f, -1.f);
		glTranslatef(-2.f * origin.x, -2.f * origin.y, -2.f * origin.z);
		glDrawArrays(GL_QUADS, 0, 4 * resolution * resolution);
		glPopMatrix();
	}
	glDrawArrays(GL_QUADS, 0, 4 * resolution * resolution);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//If a texture has been applied, remove it
	if (texture != nullptr) glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//Reset color
	glColor4f(1.f, 1.f, 1.f, 1.f);
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
				texCoordinates.push_back((float)longitude / resolution);
				texCoordinates.push_back((float)latitude / resolution);
			}
			//Since it has been incremented, set the longitude back to its original value
			--longitude;
		}
	}
}