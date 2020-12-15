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
#include "Light.h"

class Scene{

public:
	Scene(Input *in);
	~Scene();
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
	char filterMode[40];

	//Wireframe mode toggle
	bool wireframeMode = false;
	//A fullbright mode toggle
	bool fullbright = false;

	//Filtering
	enum class Filters { POINT, BILINEAR, MIPMAPING, TRILINEAR };
	Filters currentFilter;
	void applyFilter();

	//Light stuff
	Light* ambientLight = new Light(0);
	Light* pointLight = new Light(2);
	Light* spotLight = new Light(3);
	float lightRotation = 0.f;

	//Camera stuff
	Camera camera;

	//Skybox stuff
	Skybox skybox;

	//Models
	Model chair;
	Model table;
	Model lamp;

	//Geometry for serious room
	Cylinder mirrorLeftEdge;
	Cylinder mirrorRightEdge;
	BasicShape mirrorBottomEdge;
	BasicShape mirrorTopEdge;
	void drawMirrorQuad();
	BasicShape windowLeftEdge;
	BasicShape windowRightEdge;
	BasicShape windowTopEdge;
	BasicShape windowBottomEdge;
	Sphere sun;
	Sphere planet1;
	Sphere planet2;
	Sphere moon1;
	Sphere moonsMoon1;
	void drawSolarSystem();

	//Functions that will render their according room
	void renderSeriousRoom();
	void makeSeriousWalls();

	//Vector to hold any transparent shape
	std::vector<BasicShape*> transparentShapes;

	//Textures
	GLuint sky[6];
	GLuint seriousWallBase;
	GLuint seriousWallTop;
	GLuint seriousFloor;
	GLuint seriousCeiling;
	GLuint seriousDoor;
	GLuint wood;
	GLuint glass;
};

#endif