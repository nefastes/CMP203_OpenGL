// Below ifdef required to remove warnings for unsafe version of fopen and fscanf.
// Secure version won't work cross-platform, forcing this small hack.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "model.h"

bool Model::load(char* modelFilename, char* textureFilename)
{
	bool result;

	// Load in the model data,
	result = loadModel(modelFilename);
	if (!result)
	{
		MessageBox(NULL, "Model failed to load", "Error", MB_OK);
		return false;
	}

	// Load the texture for this model.
	loadTexture(textureFilename);

	return true;
}

void Model::render(short unsigned textureFilter)
{
	//Render the model with previously sorted vectors during the unroll_data section
	//Also apply and set up how the model will be textured prior to rendering
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	switch (textureFilter)
	{
	case 0:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);					break;
	case 1:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);					break;
	case 2:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	break;
	case 3:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		break;
	default:	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);					break;
	}
	if (lightSource)
	{
		glMaterialfv(GL_FRONT, GL_EMISSION, std::array<GLfloat, 4 > {1.f, 1.f, 1.f, 1.f}.data());
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4 > {.2f, .2f, .2f, 1.f}.data());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, std::array<GLfloat, 4 > {1.f, 1.f, 1.f, 1.f}.data());
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertex.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());

	//Render the data as much times as needed, but avoid to call the subroutine if either is empty
	if(numberTrianglesToRender) glDrawArrays(GL_TRIANGLES, 0, numberTrianglesToRender * 3);
	if(numberQuadsToRender) glDrawArrays(GL_QUADS, numberTrianglesToRender * 3, numberQuadsToRender * 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	if (lightSource)
	{
		glMaterialfv(GL_FRONT, GL_EMISSION, std::array<GLfloat, 4 > {0.f, 0.f, 0.f, 0.f}.data());
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4 > {1.f, 1.f, 1.f, 1.f}.data());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, std::array<GLfloat, 4 > {1.f, 1.f, 1.f, 1.f}.data());
	}
}


// Modified from a mulit-threaded version by Mark Ropper.
bool Model::loadModel(char* filename)
{
	vector<Vector3> verts;
	vector<Vector3> norms;
	vector<Vector3> texCs;
	vector<unsigned> triFaces;
	vector<unsigned> quadFaces;
	
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		return false;
	}
	while (true)
	{
		char lineHeader[128];

		// Read first word of the line, %s indicates a string to be stored inside lineHeader
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; // exit loop
		}
		else // Parse
		{
			if (strcmp(lineHeader, "v") == 0) // Vertex
			{
				Vector3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				verts.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) // Tex Coord
			{
				Vector3 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				texCs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) // Normal
			{
				Vector3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				norms.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) // Face
			{
				unsigned int face[12];
				//%d indicates integers
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &face[0], &face[1], &face[2],
																					&face[3], &face[4], &face[5],
																					&face[6], &face[7], &face[8],
																					&face[9], &face[10], &face[11]);

				//We only support triangles and quads to be read
				if (matches == 9)
				{
					for (int i = 0; i < 9; i++) triFaces.push_back(face[i]);
					++numberTrianglesToRender;
				}
				else if (matches == 12)
				{
					for (int i = 0; i < 12; i++) quadFaces.push_back(face[i]);
					++numberQuadsToRender;
				}
				else return false;
			}
		}
	}

#pragma region unroll_data
	// "Unroll" the loaded obj information into a list of triangles and/or quads.
	for (unsigned i = 0; i < triFaces.size(); i += 3)
	{
		//Get the vertices
		vertex.push_back(verts[triFaces[i] - 1].x);
		vertex.push_back(verts[triFaces[i] - 1].y);
		vertex.push_back(verts[triFaces[i] - 1].z);

		//Get the texture coords
		texCoords.push_back(texCs[triFaces[i + 1] - 1].x);
		texCoords.push_back(texCs[triFaces[i + 1] - 1].y);

		//Get the normals
		normals.push_back(norms[triFaces[i + 2] - 1].x);
		normals.push_back(norms[triFaces[i + 2] - 1].y);
		normals.push_back(norms[triFaces[i + 2] - 1].z);
	}
	//Then push the quads in order
	for (unsigned i = 0; i < quadFaces.size(); i += 3)
	{
		//Get the vertices
		vertex.push_back(verts[quadFaces[i] - 1].x);
		vertex.push_back(verts[quadFaces[i] - 1].y);
		vertex.push_back(verts[quadFaces[i] - 1].z);

		//Get the texture coords
		texCoords.push_back(texCs[quadFaces[i + 1] - 1].x);
		texCoords.push_back(texCs[quadFaces[i + 1] - 1].y);

		//Get the normals
		normals.push_back(norms[quadFaces[i + 2] - 1].x);
		normals.push_back(norms[quadFaces[i + 2] - 1].y);
		normals.push_back(norms[quadFaces[i + 2] - 1].z);
	}

#pragma endregion unroll_data
		
	// Once data has been sorted clear read data (which has been copied and are not longer needed).
	verts.clear();
	norms.clear();
	texCs.clear();
	triFaces.clear();
	quadFaces.clear();

	return true;
}

void Model::loadTexture(char* filename)
{
	originalTexture = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need inverted others don't.
	);

	//check for an error during the load process
	if (originalTexture == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	else texture = &originalTexture;

}

void Model::reloadTexture(GLuint* tex)
{
	if (tex == nullptr) texture = &originalTexture;
	else texture = tex;
}

