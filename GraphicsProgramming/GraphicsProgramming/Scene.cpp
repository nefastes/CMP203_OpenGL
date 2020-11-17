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

	//Init Camera
	camera.setInput(in);
	camera.setSpeed(10);
	camera.setSensitivity(10);

	// Initialise scene variables
	boxTexSolid = SOIL_load_OGL_texture("gfx/crate.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	boxTexTransparent = SOIL_load_OGL_texture("gfx/transparentCrate.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	sky = SOIL_load_OGL_texture("gfx/skybox.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	//Init skybox
	skybox.setTexture(sky);
}

void Scene::handleInput(float dt)
{
	// Handle user input
	camera.handleInput(dt);
}

void Scene::update(float dt)
{
	// update scene related variables.
	skybox.setPos(camera.getPosition());

	// Calculate FPS for output
	calculateFPS();
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
	//Render sky first, disable depth sorting
	glDisable(GL_DEPTH_TEST);
	skybox.draw();
	glEnable(GL_DEPTH_TEST);

	
	glTranslatef(0.f, 0.f, 4.f);
	drawUnitCube(0.f, 1.f, 0.f, 1.f);

	glTranslatef(0.f, 0.f, 2.f);
	glScalef(.5f, .5f, .5f);
	drawUnitCube(0.f, 0.f, 1.f, 1.f);

	glTranslatef(2.f, 0.f, 0.f);
	glBindTexture(GL_TEXTURE_2D, boxTexSolid);
	drawUnitCube(1.f, 1.f, 1.f, 1.f);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTranslatef(0.f, 0.f, -8.f);
	drawUnitCube(1.f, 0.f, 0.f, .5f);

	glTranslatef(2.f, 0.f, 0.f);
	glScalef(2.f, 2.f, 2.f);
	glBindTexture(GL_TEXTURE_2D, boxTexTransparent);
	drawUnitCube(1.f, 1.f, 1.f, 1.f);

	glTranslatef(2.f, 0.f, 2.f);
	glScalef(5.f, 5.f, 5.f);
	drawUnitCube(1.f, 1.f, 1.f, 1.f);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glTranslatef(-2.f, -2.f, -2.f);
	glScalef(10.f, 10.f, 10.f);
	drawSimpleQuad(1.f, 0.f, 1.f, 0.5f);

	glDisable(GL_BLEND);

	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	renderTextOutput();
	
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
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
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

void Scene::drawUnitCube(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
	glBegin(GL_QUADS);

	//Front
	glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 0.f, 0.f);
	glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);

	//RIGHT
	glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 0.f, 0.f);
	glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 0.f, -1.f);
	glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, -1.f);
	glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);

	//BACK
	glNormal3f(0.f, 0.f, -1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 0.f, -1.f);
	glNormal3f(0.f, 0.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, -1.f);
	glNormal3f(0.f, 0.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 1.f, -1.f);
	glNormal3f(0.f, 0.f, -1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 1.f, -1.f);

	//LEFT
	glNormal3f(-1.f, 1.f, 0.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 0.f, -1.f);
	glNormal3f(-1.f, 1.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glNormal3f(-1.f, 1.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glNormal3f(-1.f, 1.f, 0.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 1.f, -1.f);

	//TOP
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, 0.f);
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, -1.f);
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 1.f, -1.f);

	//BOTTOM
	glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, -1.f);
	glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);
	glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);

	glEnd();
}

void Scene::drawSimpleQuad(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
	glBegin(GL_QUADS);

	//FRONT
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);

	glEnd();
}