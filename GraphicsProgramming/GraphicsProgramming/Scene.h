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
#include <stdlib.h>
#include <time.h>
#include "Camera.h"
#include "Skybox.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Model.h"
#include "Light.h"
#include "Shadow.h"

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
	int frame = 0, frametime, timebase = 0;
	char fps[40];
	char mouseText[40];
	char positionText[255];
	char viewText[255];
	char filterMode[40];
	char cameraMode[40];
	char rotationMultiplierText[40];
	char rotationSpeedText[40];

	//Wireframe mode toggle
	bool wireframeMode = false;
	//A fullbright mode toggle
	bool fullbright = false;
	//Steve mode toogle
	bool stevemode = false;
	void updateTextures(bool sm);

	//Filtering
	enum class Filters { POINT, BILINEAR, MIPMAPING, TRILINEAR } currentFilter;
	void applyFilter();

	//Light stuff
	Light* ambientLight = new Light(0);
	Light* pointLight = new Light(2);
	std::array<GLfloat, 4> pointLightPosition;
	Sphere pointLightSphere;
	Light* spotLight = new Light(3);
	float flickerTimer;	//how much time has passed since the last flicker
	float timeToFlicker;//how much time needs to pass until the next flicker

	//Camera stuff
	CameraTypes* currentCameraType;
	Camera camera;

	//Skybox stuff
	Skybox skybox;

	//Models
	Model chair;
	Model table;
	Model lamp_struct;
	Model lamp_neons;
	Model trump;

	//Geometry for serious room
	//Mirror stuff
	Cylinder mirrorLeftEdge;
	Cylinder mirrorRightEdge;
	BasicShape mirrorBottomEdge;
	BasicShape mirrorTopEdge;
	void drawMirrorQuad();
	void setupStencil();
	void drawReflections();
	//Window stuff
	BasicShape windowLeftEdge;
	BasicShape windowRightEdge;
	BasicShape windowTopEdge;
	BasicShape windowBottomEdge;
	//Solar system stuff
	Sphere sun;
	Sphere planet1;
	Sphere planet2;
	Sphere moon1;
	Sphere moonsMoon1;
	void drawSolarSystem(bool renderAsShadow = false);
	float rotation;
	float rotationMultiplier;	//For user interaction
	float rotationSpeed;		//For user interaction
	//Transparent cubes & cylinders
	BasicShape transparentCube1;
	BasicShape transparentCube2;
	BasicShape transparentCube3;
	Cylinder transparentCylinder1;
	Disc transparentCylinderCap;
	//Vertex/Normals/Tex arrays to generate the seriousRoom
	std::array<GLfloat, 16> lowerWallsVertices;
	std::array<GLfloat, 40> upperWallsVertices;
	std::array<GLfloat, 4> ceilingVertices;
	std::vector<GLfloat> floorVertices;
	std::array<GLfloat, 16> lowerWallsNormals;
	std::array<GLfloat, 40> upperWallsNormals;
	std::array<GLfloat, 4> ceilingNormals;
	std::vector<GLfloat> floorNormals;
	std::array<GLfloat, 16> lowerWallsTexcoords;
	std::array<GLfloat, 40> upperWallsTexcoords;
	std::array<GLfloat, 4> ceilingTexcoords;
	std::vector<GLfloat> floorTexcoords;

	//Functions that will render their according room
	void renderSeriousRoom(bool renderingReflection = false);
	void makeSeriousWalls();

	//Billboards
	void drawGrassBillboard();
	void drawTreeBillboard();

	//Shadow
	std::array<GLfloat, 12> tableShadowQuad{
		-1.26875f, -1.1125f, -7.625f,
		-1.26875f, -1.1125f, -2.375f,
		1.26875f, -1.1125f, -2.375f,
		1.26875f, -1.1125f, -7.625f
	};
	float shadowMatrix[16];
	void drawShadowPlane();
	void renderShadows(bool drawingReflection);

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
	GLuint glass2;
	GLuint earthTexture;
	GLuint moonTexture;
	GLuint marsTexture;
	GLuint transparentBox;
	GLuint grassBillboardTexture;
	GLuint treeBillboardTexture;
	GLuint steveTexture;

	//Vertex array for the room
	std::array<float, 204> seriousRoomVerts;
	std::array<float, 136> seriousRoomTexcoords;
	std::array<float, 204> seriousRoomNormals;
	std::vector<float> seriousFloorVerts;
	std::vector<float> seriousFloorTexcoords;
	std::vector<float> seriousFloorNormals;
	std::array<float, 24> billboardVerts;
	std::array<float, 16> billboardTexcoords;
	std::array<float, 24> billboardNormals;
};

#endif