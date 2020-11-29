#pragma once
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#include "Disc.h"
#define PI 3.1415f
class Cylinder : public Disc
{
private:
	unsigned stackResolution;

public:
	Cylinder();
	~Cylinder();

	void shapeSpecificDrawingMode() override;
	void generateShape() override;
	void setStackResolution(unsigned);
	unsigned getStackResolution();
};

