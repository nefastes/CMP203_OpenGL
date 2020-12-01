#pragma once
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#include "Disc.h"
class Sphere : public Disc
{
public:
	Sphere();
	~Sphere();

	void shapeSpecificDrawingMode() override;
	void generateShape() override;
};

