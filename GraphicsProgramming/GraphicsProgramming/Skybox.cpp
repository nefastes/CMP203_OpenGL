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
	glTranslatef(position.x - .5f, position.y - .5f, position.z + .5f);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glBegin(GL_QUADS);

	//Front
	glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(0.f, .5f);
	glVertex3f(0.f, 0.f, 0.f);
	glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(.25f, .5f);
	glVertex3f(1.f, 0.f, 0.f);
	glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(.25f, .25f);
	glVertex3f(1.f, 1.f, 0.f);
	glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(0.f, .25f);
	glVertex3f(0.f, 1.f, 0.f);

	//RIGHT
	glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(.25f, .5f);
	glVertex3f(1.f, 0.f, 0.f);
	glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(.5f, .5f);
	glVertex3f(1.f, 0.f, -1.f);
	glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(.5f, .25f);
	glVertex3f(1.f, 1.f, -1.f);
	glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(.25f, .25f);
	glVertex3f(1.f, 1.f, 0.f);

	//BACK
	glNormal3f(0.f, 0.f, -1.f);
	glTexCoord2f(.5f, .5f);
	glVertex3f(1.f, 0.f, -1.f);
	glNormal3f(0.f, 0.f, -1.f);
	glTexCoord2f(.75f, .5f);
	glVertex3f(0.f, 0.f, -1.f);
	glNormal3f(0.f, 0.f, -1.f);
	glTexCoord2f(.75f, .25f);
	glVertex3f(0.f, 1.f, -1.f);
	glNormal3f(0.f, 0.f, -1.f);
	glTexCoord2f(.5f, .25f);
	glVertex3f(1.f, 1.f, -1.f);

	//LEFT
	glNormal3f(-1.f, 1.f, 0.f);
	glTexCoord2f(.75f, .5f);
	glVertex3f(0.f, 0.f, -1.f);
	glNormal3f(-1.f, 1.f, 0.f);
	glTexCoord2f(1.f, .5f);
	glVertex3f(0.f, 0.f, 0.f);
	glNormal3f(-1.f, 1.f, 0.f);
	glTexCoord2f(1.f, .25f);
	glVertex3f(0.f, 1.f, 0.f);
	glNormal3f(-1.f, 1.f, 0.f);
	glTexCoord2f(.75f, .25f);
	glVertex3f(0.f, 1.f, -1.f);

	//TOP
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(.25f, .0f);
	glVertex3f(0.f, 1.f, 0.f);
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(.25f, .25f);
	glVertex3f(1.f, 1.f, 0.f);
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(.5f, .25f);
	glVertex3f(1.f, 1.f, -1.f);
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(.5f, 0.f);
	glVertex3f(0.f, 1.f, -1.f);

	//BOTTOM
	glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(.25f, .75f);
	glVertex3f(0.f, 0.f, 0.f);
	glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(.5f, .75f);
	glVertex3f(0.f, 0.f, -1.f);
	glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(.5f, .5f);
	glVertex3f(1.f, 0.f, -1.f);
	glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(.25f, .5f);
	glVertex3f(1.f, 0.f, 0.f);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glPopMatrix();
}