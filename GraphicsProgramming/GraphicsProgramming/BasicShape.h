#pragma once
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#include <array>
#include "Vector3.h"
#ifndef PI
#define PI 3.1415f
#endif // !PI
class BasicShape
{
protected:
	//Color of the shape
	float red, green, blue;
	//A trasparency tracker, to be true if we want a transparent shape
	bool transparent;
	float alpha;
	//A simple bool to track if we want to render the inside of the shape (true)
	bool renderInside;
	//The shape's texture, to stay NULL if none is wanted
	GLuint* texture;
	//Vectors used for procedural generation
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoordinates;
	std::vector<GLuint> indices;
	//Origin of the shape (the origin is allways in the middle/center of the shape)
	Vector3 origin;
	//Scale of the shape
	Vector3 scale;
	//A simple bool to track whether we want the texture to fill or to repeat on our shape. By default we will fill
	bool fillTexture;
	Vector3 textureScale;
	//Track the material properties of our shape, init with default values
	GLint shininess;
	std::array< GLfloat, 4> Material_Ambient;
	std::array< GLfloat, 4> Material_Diffuse;
	std::array< GLfloat, 4> Material_Specular;

public:
	BasicShape();
	~BasicShape();

	void render();
	virtual void shapeSpecificDrawingMode();	//This function will be overwritten by other shapes
	virtual void generateShape();

	void setTransparent(bool t) { transparent = t; }
	void setTransparency(float a) { alpha = a; transparent = true; Material_Diffuse[3] = alpha; }
	bool isTranspearent() { return transparent; }

	void renderInsideShape(bool r) { renderInside = r; }
	bool isInsideRendered() { return renderInside; }

	void setColor3f(float r, float g, float b);					//Function too big to be declared in header file only
	void setColor4f(float r, float g, float b, float a);		//Function too big to be declared in header file only

	void setTexture(GLuint& tex) { texture = &tex; }
	GLuint* getTexture() { return texture; }

	void setPosition(Vector3 pos) { origin = pos; }	//Sets the position of the shape, must be called BEFORE generating it.
	void setPosition(float x, float y, float z) { origin.x = x; origin.y = y; origin.z = z; }	//Sets the position of the shape, must be called BEFORE generating it.
	Vector3 getPosition() { return origin; }

	void setScale(Vector3 s) { scale = s; }
	void setScale(float sx, float sy, float sz) { scale.x = sx; scale.y = sy; scale.z = sz; }
	Vector3 getScale() { return scale; }
	void rescale(Vector3 s) { scale = s; generateShape(); }	//Need to regenerate the shape when rescaling it
	void rescale(float sx, float sy, float sz) { scale.x = sx; scale.y = sy; scale.z = sz; generateShape(); }	//Need to regenerate the shape when rescaling it

	void setTextureRepeating() { fillTexture = false; }
	void setTextureRepeating(float textureScaleX, float textureScaleY, float textureScaleZ);		//Function too big to be declared in header file only

	void setDiffuseMaterial(std::array<GLfloat, 4> diffuseMat) { Material_Diffuse = diffuseMat; };
	void setSpecularMaterial(std::array<GLfloat, 4> specularMat) { Material_Specular = specularMat; };
	void setShininess(GLint s) { shininess = s; };
};

