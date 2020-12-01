#pragma once
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#include "BasicShape.h"
class Disc : public BasicShape
{
protected:
	float radius;
	unsigned resolution;

public:
	Disc();
	~Disc();

	void shapeSpecificDrawingMode() override;
	void generateShape() override;
	void setRadius(float);
	void setResolution(unsigned);
	float getRadius();
	unsigned getResolution();
};

