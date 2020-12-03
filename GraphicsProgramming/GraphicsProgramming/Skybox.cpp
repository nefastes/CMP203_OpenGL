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

void Skybox::setTexture(GLuint& tex)
{
	texture = &tex;
}

void Skybox::draw()
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glBindTexture(GL_TEXTURE_2D, *texture);
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
	glDrawArrays(GL_QUADS, 0, 24);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, std::array<GLfloat, 4 > {1.f, 1.f, 1.f, 1.f}.data());
	glPopMatrix();
}