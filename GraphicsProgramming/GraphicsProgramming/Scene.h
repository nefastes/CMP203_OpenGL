// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Input.h"
#include <stdio.h>
// Further includes should go here:
#include "SOIL.h"
#include <vector>
#include <array>
#include "Camera.h"
#include "Skybox.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Model.h"

class Scene{

public:
	Scene(Input *in);
	// Main render function
	void render();
	// Handle input function that receives delta time from parent.
	void handleInput(float dt);
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

protected:
	// configure opengl render pipeline
	void initialiseOpenGL();
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();

	// draw primitive functions
	void drawSimpleQuad(float, float, float, float);
	void drawPlane();

	// For access to user input.
	Input* input;
		
	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];
	char positionText[255];
	char viewText[255];

	//Wireframe mode toggle
	bool wireframeMode = false;
	//A fullbright mode toggle
	bool fullbright = false;

	//Light stuff
	void testLights();
	float lightRotation = 0.f;

	//Camera stuff
	Camera camera;

	//Skybox stuff
	Skybox skybox;

	//Models
	Model spaceship;

	//Shapes
	BasicShape cube;
	BasicShape cube2;
	BasicShape cube3;
	BasicShape cube4;
	BasicShape cube5;
	BasicShape cube6;
	BasicShape platform;
	Disc disc;
	Disc disc2;
	Cylinder cylinder;
	Cylinder cylinder2;
	Sphere sphere;
	Sphere sphere2;
	//Vector to hold any transparent shape
	std::vector<BasicShape*> transparentShapes;

	//Textures
	GLuint checkedTex;
	GLuint boxTexSolid;
	GLuint boxTexTransparent;
	GLuint marble;
	GLuint sky;
};

#endif