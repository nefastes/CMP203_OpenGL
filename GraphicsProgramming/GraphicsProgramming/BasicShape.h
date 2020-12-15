#pragma once
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#include <array>
#include <functional>
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
	//A tracker to track if this object is scriptable/dynamic
	bool scriptable;
	//When the object is scriptable, will need to store the current transformation matrix in here
	std::array<GLfloat, 16> transformationMatrix;


public:
	BasicShape();
	~BasicShape();

	void render(short unsigned textureFilteringMode = 3);
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

	void makeScriptObject() { scriptable = true; }
	void assignCurrentTransformationMatrix() { 
		//Get the transformation matrix for this object
		glGetFloatv(GL_MODELVIEW_MATRIX, &transformationMatrix[0]);
		//Update the position accordingly with a 1x4 by 4x4 matrix multiplication
		std::array<GLfloat, 4> temppos;
		for(unsigned char i = 0; i < 4; ++i)
			temppos[i] = origin.x * transformationMatrix[i] + origin.y * transformationMatrix[4 + i] + origin.z * transformationMatrix[8 + i] + transformationMatrix[12 + i];
		origin.x = temppos[0] / temppos[3];
		origin.y = temppos[1] / temppos[3];
		origin.z = temppos[2] / temppos[3];
	}
};

