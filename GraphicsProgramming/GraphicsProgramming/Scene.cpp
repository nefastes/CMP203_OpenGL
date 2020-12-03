#include "Scene.h"

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// Other OpenGL / render setting should be applied here.
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Init lights

	//Init Camera
	camera.setInput(in);
	camera.setSpeed(10);
	camera.setSensitivity(10);

	// Initialise textures
	checkedTex = SOIL_load_OGL_texture("gfx/checked.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	boxTexSolid = SOIL_load_OGL_texture("gfx/crate.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	boxTexTransparent = SOIL_load_OGL_texture("gfx/transparentCrate.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	marble = SOIL_load_OGL_texture("gfx/marble1.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	sky = SOIL_load_OGL_texture("gfx/skybox2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	//Init skybox
	skybox.setTexture(sky);

	//Init models
	spaceship.load("models/spaceship.obj", "gfx/spaceship.jpg");

	//Init Shapes
	//Genrate cubes
	cube.setTexture(boxTexSolid);
	cube.setTransparency(.4f);
	cube.setPosition(-5.f, 2.f, -20.f);
	cube.setScale(2.f, 2.f, 2.f);
	cube.renderInsideShape(true);
	cube.generateShape();
	cube2.setColor4f(0.f, 1.f, 1.f, .55f);
	cube2.setScale(3.f, 3.f, 3.f);
	cube2.setPosition(-5.f, 2.f, -30.f);
	cube2.generateShape();
	cube3.setPosition(4.f, 0.f, 0.f);
	cube3.setTexture(boxTexTransparent);
	cube3.renderInsideShape(true);
	cube3.generateShape();
	cube4.setPosition(8.f, 0.f, 0.f);
	cube4.setTexture(boxTexTransparent);
	cube4.renderInsideShape(true);
	cube4.generateShape();
	cube5.setPosition(8.f, 0.f, 4.f);
	cube5.setTexture(boxTexTransparent);
	cube5.renderInsideShape(true);
	cube5.generateShape();
	cube6.setPosition(4.f, 0.f, 4.f);
	cube6.setTexture(boxTexTransparent);
	cube6.renderInsideShape(true);
	cube6.generateShape();
	//Generate platform
	platform.setPosition(-10.f, -5.f, 0.f);
	platform.setTexture(marble);
	platform.setScale(5.f, .25f, 5.f);
	platform.setTextureRepeating(2.f, 2.f, 2.f);
	platform.generateShape();
	//Generate room
	room.setPosition(20.f, 0.f, -20.f);
	room.setColor3f(0.f, 0.f, 1.f);
	room.setScale(5.f, 10.f, 10.f);
	room.renderInsideShape(true);
	room.generateShape();
	//Genrate disc
	disc.setResolution(20);
	disc.setPosition(-5.f, 2.f, -10.f);
	disc.setColor3f(1.f, 0.f, 1.f);
	disc.generateShape();
	disc2.setResolution(100);
	disc2.setTransparency(.75f);
	disc2.setTexture(checkedTex);
	disc2.setPosition(-10.f, 2.f, -10.f);
	disc2.generateShape();
	//Generate sphere
	sphere.setResolution(10);
	sphere.setRadius(1.f);
	sphere.setPosition(-5.f, 2.f, -5.f);
	sphere.setColor3f(0.f, 1.f, 1.f);
	sphere.generateShape();
	sphere2.setResolution(100);
	sphere2.setRadius(1.f);
	sphere2.setTransparency(.5f);
	sphere2.setTexture(checkedTex);
	sphere2.setPosition(-10.f, 2.f, -5.f);
	sphere2.renderInsideShape(true);
	sphere2.generateShape();
	//Genrate cylinders
	cylinder.setResolution(20);
	cylinder.setStackResolution(13);
	cylinder.setPosition(-5.f, 2.f, -15.f);
	cylinder.setColor3f(1.f, 1.f, 0.f);
	cylinder.generateShape();
	cylinder2.setResolution(21);
	cylinder2.setStackResolution(11);
	cylinder2.setRadius(2.f);
	cylinder2.setTexture(checkedTex);
	cylinder2.setTransparency(.75f);
	cylinder2.setPosition(-5.f, 2.f, -40.f);
	cylinder2.renderInsideShape(true);
	cylinder2.generateShape();

	//Push references of all transparent shapes into the according vector
	transparentShapes.push_back(&cube);
	transparentShapes.push_back(&cube2);
	transparentShapes.push_back(&cube3);
	transparentShapes.push_back(&cube4);
	transparentShapes.push_back(&cube5);
	transparentShapes.push_back(&cube6);
	transparentShapes.push_back(&disc2);
	transparentShapes.push_back(&sphere2);
	transparentShapes.push_back(&cylinder2);
}

void Scene::handleInput(float dt)
{
	//Toggle wireframe mode
	if(!wireframeMode && input->isKeyDownOnce(GLUT_KEY_F1))
	{
		wireframeMode = true;
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	else if(wireframeMode && input->isKeyDownOnce(GLUT_KEY_F1))
	{
		wireframeMode = false;
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
	}
	if (!fullbright && input->isKeyDownOnce(GLUT_KEY_F2)) fullbright = true;
	else if (fullbright && input->isKeyDownOnce(GLUT_KEY_F2)) fullbright = false;

	// Handle user input
	camera.handleInput(dt);
}

void Scene::update(float dt)
{
	// update scene related variables.
	skybox.setPos(camera.getPosition());

	//Update the transparent shapes order of rendering relative to the camera position (furthest to be rendered first, closest to be rendered last)
	std::sort(transparentShapes.begin(), transparentShapes.end(), [&](BasicShape* s1, BasicShape* s2)
		{
			//We want to return true if the first shape is further to the camera than the second shape
			return s1->getPosition() - camera.getPosition() > s2->getPosition() - camera.getPosition();
		});

	// Calculate FPS for output
	calculateFPS();

	lightRotation += 10 * dt;
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(
		camera.getPosition().x, camera.getPosition().y, camera.getPosition().z,
		camera.getLookAt().x, camera.getLookAt().y, camera.getLookAt().z,
		camera.getUp().x, camera.getUp().y, camera.getUp().z
	);
	
	// Render geometry/scene here -------------------------------------
	//ENABLE LIGHTING FOR GEOMETRY
	if (!fullbright) glEnable(GL_LIGHTING);
	//Render sky first, disable depth sorting, the skybox is only lit by the ambient lighting
	//TODO: make the skybox to interact with ambient light only
	glDisable(GL_DEPTH_TEST);
	skybox.draw();
	glEnable(GL_DEPTH_TEST);

	//RENDER LIGHTS
	testLights();
	//RENDER GEOMETRY
	glPushMatrix();
		glTranslatef(-10.f, 0.f, -15.f);
		glScalef(5.f, 5.f, 5.f);
		spaceship.render();
	glPopMatrix();

	drawPlane();

	room.render();
	platform.render();
	sphere.render();
	disc.render();
	cylinder.render();

	glEnable(GL_BLEND);
	for (unsigned i = 0; i < transparentShapes.size(); ++i) transparentShapes[i]->render();
	glDisable(GL_BLEND);

	// End render geometry --------------------------------------

	// Render text, should be last object rendered. Lighting should not be applied to HUD elements.
	glDisable(GL_LIGHTING);
	renderTextOutput();
	if (!fullbright) glEnable(GL_LIGHTING);
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

void Scene::initialiseOpenGL()
{
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);		// TODO: Find out what this does
	glEnable(GL_CULL_FACE);								// Enable face culling
	glCullFace(GL_BACK);								// Face culling mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blend Function for transparent shapes
	glutSetCursor(GLUT_CURSOR_NONE);					// Hide cursor
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	
	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	//Update the camera center
	camera.setCenterWindow((int)(w / 2), (int)(h / 2));
}

// Calculates FPS
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	sprintf_s(positionText, "Position: %g, %g, %g", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	sprintf_s(viewText, "View: %g, %g, %g", camera.getYaw(), camera.getPitch(), camera.getRoll());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
	displayText(-1.f, 0.84f, 1.f, 0.f, 0.f, positionText);
	displayText(-1.f, 0.78f, 1.f, 0.f, 0.f, viewText);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}

void Scene::drawSimpleQuad(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
	glBegin(GL_QUADS);

	//FRONT
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);

	glEnd();
}

void Scene::drawPlane()
{
	glPushMatrix();
	//Translate
	glTranslatef(-5.f, 0.f, -5.f);
	//Scale down
	glScalef(.2f, 1.f, .2f);

	//Color
	glColor3f(1.f, 1.f, 1.f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4 > {.2f, .2f, .2f, 1.f}.data());

	//Make a plane of 50x50 quads
	glBegin(GL_QUADS);
	for (unsigned z = 0; z < 50; ++z)
		for (unsigned x = 0; x < 50; ++x)
		{
			glNormal3f(0.f, 1.f, 0.f);
			glVertex3f(0.f + (float)x, -2.f, 0.f + (float)z);
			glNormal3f(0.f, 1.f, 0.f);
			glVertex3f(1.f + (float)x, -2.f, 0.f + (float)z);
			glNormal3f(0.f, 1.f, 0.f);
			glVertex3f(1.f + (float)x, -2.f, -1.f + (float)z);
			glNormal3f(0.f, 1.f, 0.f);
			glVertex3f(0.f + (float)x, -2.f, -1.f + (float)z);
		}
	glEnd();

	glPopMatrix();
}

void Scene::testLights()
{
	//Material for debug spheres
	GLfloat Material_No_Emission[4] = { 0.f, 0.f, 0.f, 0.f };

	//TEST AMBIENT LIGHT
	//GLfloat Ambient[4] = { .6f, 0.f, 0.f, 1.f };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	//glEnable(GL_LIGHT0);

	//TEST DIFFUSE LIGHT
	GLfloat Ambient[4] = { .2f, .2f, .2f, 1.f };
	GLfloat Diffuse1[4] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat Position1[4] = { -1.f, 1.f, 1.f, 0.f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse1);
	glLightfv(GL_LIGHT1, GL_POSITION, Position1);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.222f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.234f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, .444f);
	glEnable(GL_LIGHT1);

	//TEST POINT LIGHT
	GLfloat Diffuse2[4] = { 0.f, 0.f, 1.f, 1.f };
	GLfloat Position2[4] = { -3.f, 0.f, -3.f, 1.f };
	glPushMatrix();
	glRotatef(lightRotation, 0, 1, 0);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Diffuse2);
	glLightfv(GL_LIGHT2, GL_POSITION, Position2);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.f);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, .5f);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, .25f);
	glEnable(GL_LIGHT2);
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, Diffuse2);
		glTranslatef(Position2[0], Position2[1], Position2[2]);
		gluSphere(gluNewQuadric(), .1f, 20, 20);
		glMaterialfv(GL_FRONT, GL_EMISSION, Material_No_Emission);
	glPopMatrix();
	glPopMatrix();

	//TEST SPOTLIGHT
	GLfloat Diffuse3[4] = { 0.f, 1.f, 0.f, 1.f };
	GLfloat Position3[4] = { 4.f, -1.75f, 3.f, 1.f };
	GLfloat Direction3[4] = { -1.f, 0.f, 0.f, 0.f };
	float cutoff = 90.f;
	float exponent = 15.f;
	glPushMatrix();
	glRotatef(lightRotation, 0, 0, 1);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, Diffuse3);
	glLightfv(GL_LIGHT3, GL_POSITION, Position3);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, cutoff);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, Direction3);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, exponent);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, .5f);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, .125f);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, .0675f);
	glEnable(GL_LIGHT3);
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, Diffuse3);
		glTranslatef(Position3[0], Position3[1], Position3[2]);
		gluSphere(gluNewQuadric(), .1f, 20, 20);
		glMaterialfv(GL_FRONT, GL_EMISSION, Material_No_Emission);
	glPopMatrix();
	glPopMatrix();
}