#include "Skybox.h"

Skybox::Skybox()
{
	texture = nullptr;
}

Skybox::~Skybox()
{

}

void Skybox::setPos(Vector3 pos)
{
	position = pos;
}

Vector3& Skybox::getPos()
{
	return position;
}

void Skybox::setTexture(GLuint* tex)
{
	texture = tex;
}

void Skybox::draw(short unsigned textureFilteringMode)
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glMaterialfv(GL_FRONT, GL_AMBIENT, Material_Ambient.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Material_Diffuse.data());
	glColor4f(1.f, 1.f, 1.f, 1.f);
	
	//Render using ordered arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	glNormalPointer(GL_FLOAT, 0, norms);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	for (unsigned i = 0; i < 6; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		switch (textureFilteringMode)
		{
		case 0:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);					break;
		case 1:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);					break;
		case 2:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	break;
		case 3:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		break;
		default:	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);					break;
		}
		glDrawArrays(GL_QUADS, i * 4, 4 + i * 4);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4 > {1.f, 1.f, 1.f, 1.f}.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, std::array<GLfloat, 4 > {1.f, 1.f, 1.f, 1.f}.data());
	glPopMatrix();
}