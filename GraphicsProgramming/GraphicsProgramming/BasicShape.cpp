#include "BasicShape.h"
BasicShape::BasicShape()
{
	//Color of the shape (default white)
	red = 1.f, green = 1.f, blue = 1.f;
	//A trasparency tracker, to be true if we want a transparent shape
	transparent = false;
	alpha = 1.f;
	//A simple bool to track if we want to render the inside of the shape (true)
	renderInside = false;
	//The shape's texture, to stay NULL if none is wanted
	texture = nullptr;
	//Scale of the shape
	scale = Vector3(1.f, 1.f, 1.f);
	//A simple bool to track whether we want the texture to fill or to repeat on our shape. By default we will fill
	fillTexture = true;
	textureScale = Vector3(1.f, 1.f, 1.f);;
	//Track the material properties of our shape, init with default values
	shininess = 1;
	BasicShape::Material_Ambient = { red, green, blue, 1.f };
	BasicShape::Material_Diffuse = { red, green, blue, 1.f };	//Initial the material color with the color of the shape
	Material_Specular.fill(0.f);	//No specular by default
}

BasicShape::~BasicShape()
{

}

void BasicShape::render(short unsigned textureFilteringMode)
{
	//Color the shape with its color
	glColor3f(red, green, blue);
	//Setup material
	glMaterialfv(GL_FRONT, GL_AMBIENT, Material_Ambient.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Material_Diffuse.data());
	glMaterialfv(GL_FRONT, GL_SPECULAR, Material_Specular.data());
	glMateriali(GL_FRONT, GL_SHININESS, shininess);
	//Texture the shape if a texture exists
	if (texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, *texture);
		switch (textureFilteringMode)
		{
		case 0:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);					break;
		case 1:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);					break;
		case 2:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	break;
		case 3:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		break;
		default:	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);					break;
		}
	}
	//Render using ordered arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordinates.data());
	glPushMatrix();
	shapeSpecificDrawingMode();	//Call the shape's specific render, will differ for every sort of shape.
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//If a texture has been applied, remove it
	if (texture != nullptr) glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//Reset color
	glColor4f(1.f, 1.f, 1.f, 1.f);
	//Reset materials
	glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4 > {1.f, 1.f, 1.f, 1.f}.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, std::array<GLfloat,4 > {1.f, 1.f, 1.f, 1.f}.data());
	glMaterialfv(GL_FRONT, GL_SPECULAR, std::array<GLfloat, 4 > {0.f, 0.f, 0.f, 0.f}.data());
	glMateriali(GL_FRONT, GL_SHININESS, 1);
}

void BasicShape::shapeSpecificDrawingMode()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (transparent) glColor4f(red, green, blue, alpha);
	if (renderInside)
	{
		glCullFace(GL_FRONT);
		glNormalPointer(GL_FLOAT, 0, invertedNormals.data());
		glDrawArrays(GL_QUADS, 0, 24);
		glCullFace(GL_BACK);
		glNormalPointer(GL_FLOAT, 0, normals.data());
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
		-1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		1.f * scale.z + origin.z,
		-1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		1.f * scale.z + origin.z,
		1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		1.f * scale.z + origin.z,
		1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		1.f * scale.z + origin.z,

		//Right
		1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		1.f * scale.z + origin.z,
		1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		1.f * scale.z + origin.z,
		1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,
		1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,

		//Back
		1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,
		1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,
		-1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,
		-1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,

		//Left
		-1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,
		-1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,
		-1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		1.f * scale.z + origin.z,
		-1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		1.f * scale.z + origin.z,

		//Top
		-1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,
		-1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		1.f * scale.z + origin.z,
		1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		1.f * scale.z + origin.z,
		1.f * scale.x + origin.x,		1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,

		//Bottom
		-1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		1.f * scale.z + origin.z,
		-1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,
		1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		-1.f * scale.z + origin.z,
		1.f * scale.x + origin.x,		-1.f * scale.y + origin.y,		1.f * scale.z + origin.z
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

	//Assign the inverted normals
	for (unsigned i = 0; i < normals.size(); ++i) invertedNormals.push_back(normals[i] * -1.f);
}

void BasicShape::setColor3f(float r, float g, float b)
{
	red = r, green = g, blue = b;
	BasicShape::Material_Diffuse = { red, green, blue, 1.f };
	BasicShape::Material_Ambient = { red / 5.f, green / 5.f, blue / 5.f, 1.f };
}

void BasicShape::setColor4f(float r, float g, float b, float a)
{
	red = r, green = g, blue = b, alpha = a;
	transparent = true;
	BasicShape::Material_Diffuse = { red, green, blue, alpha };
	BasicShape::Material_Ambient = { red / 5.f, green / 5.f, blue / 5.f, alpha / 5.f };
}

void BasicShape::setTextureRepeating(float textureScaleX, float textureScaleY, float textureScaleZ)
{
	textureScale.x = textureScaleX, textureScale.y = textureScaleY, textureScale.z = textureScaleZ;
	fillTexture = false;
}
