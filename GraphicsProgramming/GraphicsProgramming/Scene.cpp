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

	//Init texture filtering as trilinear
	currentFilter = Filters::TRILINEAR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Init lights
	ambientLight->makeAmbient(std::array<GLfloat, 4>{.1f, .1f, .1f, 1.f}.data());
	/*ambientLight->makeDiffuse(
		std::array<GLfloat, 4>{.75f, .75f, .75f, 1.f}.data(),
		std::array<GLfloat, 4>{-1.f, 1.f, 1.f, 0.f}.data(),
		.5f, .25f, .125f);*/
	pointLight->makeDiffuse(
		std::array<GLfloat, 4>{0.f, 0.f, 1.f, 1.f}.data(),
		std::array<GLfloat, 4>{0.f, 2.f, -5.f, 1.f}.data(),
		.5f, .25f, .125f);
	spotLight->makeSpot(
		std::array<GLfloat, 4>{1.f, 1.f, 1.f, 1.f}.data(),
		std::array<GLfloat, 4>{0.f, 2.125f, -5.f, 1.f}.data(),
		std::array<GLfloat, 4>{0.f, -1.f, 0.f, 0.f}.data(),
		50.f, 5.f, .5f, .125f, .0675f);

	//Init Camera
	camera.setInput(in);
	camera.setSpeed(10);
	camera.setSensitivity(10);

	// Initialise textures
	sky[0] = SOIL_load_OGL_texture("gfx/skybox2/skybox_fartsky_ft.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	sky[1] = SOIL_load_OGL_texture("gfx/skybox2/skybox_fartsky_rt.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	sky[2] = SOIL_load_OGL_texture("gfx/skybox2/skybox_fartsky_bk.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	sky[3] = SOIL_load_OGL_texture("gfx/skybox2/skybox_fartsky_lf.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	sky[4] = SOIL_load_OGL_texture("gfx/skybox2/skybox_fartsky_up.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	sky[5] = SOIL_load_OGL_texture("gfx/skybox2/skybox_fartsky_dn.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	seriousWallBase = SOIL_load_OGL_texture("gfx/halflife/-0OUT_WALL3.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	seriousWallTop = SOIL_load_OGL_texture("gfx/halflife/-1LAB1_W4GAD.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	seriousFloor = SOIL_load_OGL_texture("gfx/halflife/-1LAB3_FLR1B.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	seriousCeiling = SOIL_load_OGL_texture("gfx/halflife/-2FREEZER_PAN2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	seriousDoor = SOIL_load_OGL_texture("gfx/halflife/GENERIC_113C.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	wood = SOIL_load_OGL_texture("gfx/wood.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glass = SOIL_load_OGL_texture("gfx/halflife/GLASS_BRIGHT.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	//Init skybox
	skybox.setTexture(sky);

	//Init models
	chair.load("models/13494_Folding_Chairs_v1_L3.obj", "gfx/13494_Folding_Chairs_diff.jpg");
	table.load("models/10233_Kitchen_Table_v2_max2011_it2.obj", "gfx/10233_Kitchen_Table_v1_Diffuse.jpg");
	lamp.load("models/HangingLight_triangles.obj", "gfx/OldFlorecentLight.jpg");

	//Init Shapes
	//Init mirror
	mirrorLeftEdge.setPosition(-5.f, 0.f, -2.5f);
	mirrorLeftEdge.setRadius(.25f);
	mirrorLeftEdge.setResolution(10);
	mirrorLeftEdge.setStackResolution(5);
	mirrorLeftEdge.setTexture(wood);
	mirrorLeftEdge.generateShape();
	mirrorRightEdge.setPosition(-5.f, 0.f, -7.5f);
	mirrorRightEdge.setRadius(.25f);
	mirrorRightEdge.setResolution(10);
	mirrorRightEdge.setStackResolution(5);
	mirrorRightEdge.setTexture(wood);
	mirrorRightEdge.generateShape();
	mirrorTopEdge.setPosition(-5.f, 2.125f, -5.f);
	mirrorTopEdge.setScale(.25f, .125f, 2.75f);
	mirrorTopEdge.setTextureRepeating();
	mirrorTopEdge.setTexture(wood);
	mirrorTopEdge.generateShape();
	mirrorBottomEdge.setPosition(-5.f, -2.125f, -5.f);
	mirrorBottomEdge.setScale(.25f, .125f, 2.75f);
	mirrorBottomEdge.setTextureRepeating();
	mirrorBottomEdge.setTexture(wood);
	mirrorBottomEdge.generateShape();
	//Init window
	windowLeftEdge.setPosition(-1.375f, 0.5f, -10.f);
	windowLeftEdge.setScale(.125f, 1.5f, .125f);
	windowLeftEdge.setTextureRepeating();
	windowLeftEdge.setTexture(wood);
	windowLeftEdge.generateShape();
	windowRightEdge.setPosition(1.375f, 0.5f, -10.f);
	windowRightEdge.setScale(.125f, 1.5f, .125f);
	windowRightEdge.setTextureRepeating();
	windowRightEdge.setTexture(wood);
	windowRightEdge.generateShape();
	windowTopEdge.setPosition(0.f, 1.875f, -10.f);
	windowTopEdge.setScale(1.25f, .125f, .125f);
	windowTopEdge.setTextureRepeating();
	windowTopEdge.setTexture(wood);
	windowTopEdge.generateShape();
	windowBottomEdge.setPosition(0.f, -0.875f, -10.f);
	windowBottomEdge.setScale(1.25f, .125f, .125f);
	windowBottomEdge.setTextureRepeating();
	windowBottomEdge.setTexture(wood);
	windowBottomEdge.generateShape();
	//Init spheres for solar system (i'm not initialising a position, as it would then be difficult to rotate, translate and scale the shapes, so their center is at 0,0,0)
	sun.setRadius(.5f);
	sun.setResolution(50);
	sun.setColor4f(1.f, .65f, 0.f, .875f);		//Orange
	sun.makeScriptObject();
	sun.generateShape();
	planet1.setRadius(.125f);
	planet1.setResolution(25);
	planet1.setColor4f(.2f, .73f, 1.f, .25f);	//Cyan
	planet1.makeScriptObject();
	planet1.generateShape();
	planet2.setRadius(.25f);
	planet2.setResolution(20);
	planet2.setColor4f(.4f, 0.f, .6f, .75f);	//Purple
	planet2.makeScriptObject();
	planet2.generateShape();
	moon1.setRadius(.0675f);
	moon1.setResolution(10);
	moon1.setColor4f(0.f, 1.f, 0.f, .625f);	//Green
	moon1.makeScriptObject();
	moon1.generateShape();
	moonsMoon1.setRadius(.05f);
	moonsMoon1.setResolution(10);
	moonsMoon1.setColor4f(1.f, 1.f, .2f, .375f);	//Yellow
	moonsMoon1.makeScriptObject();
	moonsMoon1.generateShape();

	//Push references of all transparent shapes into the according vector
	transparentShapes.push_back(&sun);
	transparentShapes.push_back(&planet1);
	transparentShapes.push_back(&planet2);
	transparentShapes.push_back(&moon1);
	transparentShapes.push_back(&moonsMoon1);
}

Scene::~Scene()
{
	//Free everything on the heap
	delete ambientLight;
	delete pointLight;
	delete spotLight;
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
	//Toggle fullbright mode
	if (!fullbright && input->isKeyDownOnce(GLUT_KEY_F2)) fullbright = true;
	else if (fullbright && input->isKeyDownOnce(GLUT_KEY_F2)) fullbright = false;
	//Change texture filtering mode
	if (input->isKeyDownOnce(GLUT_KEY_F3))
	{
		if (currentFilter == Filters::TRILINEAR) currentFilter = Filters::POINT;
		else currentFilter = static_cast<Filters>((int)currentFilter + 1);
		applyFilter();
	}

	// Handle user input
	camera.handleInput(dt);
}

void Scene::update(float dt)
{
	// update scene related variables.
	skybox.setPos(camera.getPosition());

	//Update the transparent shapes order of rendering relative to the camera position (furthest to be rendered first, closest to be rendered last)
	std::sort(transparentShapes.begin(), transparentShapes.end(), [=](BasicShape* s1, BasicShape* s2)
		{
			//We want to return true if the first shape is further to the camera than the second shape
			return s1->getPosition() - camera.getPosition() > s2->getPosition() - camera.getPosition();
		});

	// Calculate FPS for output
	calculateFPS();

	lightRotation += 10.f * dt;
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
	glDisable(GL_DEPTH_TEST);
	skybox.draw((short unsigned)currentFilter);
	glEnable(GL_DEPTH_TEST);

	//RENDER GEOMETRY
	renderSeriousRoom();

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
	switch (currentFilter)
	{
	case Filters::POINT:		sprintf_s(filterMode, "Filter: POINT SAMPLE");	break;
	case Filters::BILINEAR:		sprintf_s(filterMode, "Filter: BILINEAR");		break;
	case Filters::MIPMAPING:	sprintf_s(filterMode, "Filter: MIPMAPPING");	break;
	case Filters::TRILINEAR:	sprintf_s(filterMode, "Filter: TRILINEAR");		break;
	default:					sprintf_s(filterMode, "Filter: Error");			break;
	}
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
	displayText(-1.f, 0.84f, 1.f, 0.f, 0.f, positionText);
	displayText(-1.f, 0.78f, 1.f, 0.f, 0.f, viewText);
	displayText(-1.f, 0.72f, 1.f, 0.f, 0.f, filterMode);
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

void Scene::applyFilter()
{
	switch (currentFilter)
	{
	case Filters::POINT:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);					break;
	case Filters::BILINEAR:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);					break;
	case Filters::MIPMAPING:	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	break;
	case Filters::TRILINEAR:	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		break;
	default:					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);					break;
	}
}

void Scene::renderSeriousRoom()
{
	//RENDER LIGHTS
	ambientLight->render();
	spotLight->render();

	//Create the room's walls
	makeSeriousWalls();

	//Render mirror
	mirrorLeftEdge.render((short unsigned)currentFilter);
	mirrorRightEdge.render((short unsigned)currentFilter);
	mirrorTopEdge.render((short unsigned)currentFilter);
	mirrorBottomEdge.render((short unsigned)currentFilter);
	drawMirrorQuad();

	//Render window
	windowLeftEdge.render((short unsigned)currentFilter);
	windowRightEdge.render((short unsigned)currentFilter);
	windowTopEdge.render((short unsigned)currentFilter);
	windowBottomEdge.render((short unsigned)currentFilter);

	//Render models
	glPushMatrix();
	{
		glPushMatrix();
		{
			glTranslatef(4.f, -3.f, -8.f);
			glRotatef(-90, 1, 0, 0);
			glRotatef(-45, 0, 0, 1);
			glScalef(.075f, .075f, .075f);
			chair.render();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(3.f, -3.f, -9.f);
			glRotatef(-90, 1, 0, 0);
			glRotatef(-90, 0, 0, 1);
			glScalef(.075f, .075f, .075f);
			chair.render();
		}
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.f, -3.f, -5.f);
		glRotatef(-90, 1, 0, 0);
		glScalef(.025f, .025f, .025f);
		table.render();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.f, 2.95f, -5.f);
		glRotatef(-90, 0, 1, 0);
		glScalef(20.f, 20.f, 20.f);
		lamp.render();
	}
	glPopMatrix();

	//Render solar system
	drawSolarSystem();
}

void Scene::makeSeriousWalls()
{
	//Left wall
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, seriousWallBase);
		applyFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		{
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(0, 0);
			glVertex3f(-5.f, -2.f, 0.f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(0, 1);
			glVertex3f(-5.f, -3.f, 0.f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(10, 1);
			glVertex3f(-5.f, -3.f, -10.f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(10, 0);
			glVertex3f(-5.f, -2.f, -10.f);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, seriousWallTop);
		applyFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		{
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(0, 0);
			glVertex3f(-5.f, 3.f, 0.f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(0, 1);
			glVertex3f(-5.f, -2.f, 0.f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(2.5f, 1);
			glVertex3f(-5.f, -2.f, -2.5f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(2.5f, 0);
			glVertex3f(-5.f, 3.f, -2.5f);

			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(2.5f, 0);
			glVertex3f(-5.f, 3.f, -2.5f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(2.5f, .2f);
			glVertex3f(-5.f, 2.f, -2.5f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(7.5f, .2f);
			glVertex3f(-5.f, 2.f, -7.5f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(7.5f, 0);
			glVertex3f(-5.f, 3.f, -7.5f);

			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(7.5f, 0);
			glVertex3f(-5.f, 3.f, -7.5f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(7.5f, 1);
			glVertex3f(-5.f, -2.f, -7.5f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(10, 1);
			glVertex3f(-5.f, -2.f, -10.f);
			glNormal3f(1.f, 0.f, 0.f);
			glTexCoord2f(10, 0);
			glVertex3f(-5.f, 3.f, -10.f);
		}
		glEnd();
	}
	glPopMatrix();

	//Right wall
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, seriousWallBase);
		applyFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		{
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(10, 0);
			glVertex3f(5.f, -2.f, -10.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(10, 1);
			glVertex3f(5.f, -3.f, -10.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(0, 1);
			glVertex3f(5.f, -3.f, -2.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(0, 0);
			glVertex3f(5.f, -2.f, -2.f);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, seriousWallTop);
		applyFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		{
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(8, 0);
			glVertex3f(5.f, 3.f, -10.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(8, 1);
			glVertex3f(5.f, -2.f, -10.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(0, 1);
			glVertex3f(5.f, -2.f, -2.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(0, 0);
			glVertex3f(5.f, 3.f, -2.f);

			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(2, 0);
			glVertex3f(5.f, 3.f, -2.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(2, .4f);
			glVertex3f(5.f, 1.f, -2.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(0, .4f);
			glVertex3f(5.f, 1.f, 0.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(0, 0);
			glVertex3f(5.f, 3.f, 0.f);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, seriousDoor);
		applyFilter();
		glBegin(GL_QUADS);
		{
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(0, 0);
			glVertex3f(5.f, 1.f, -2.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(0, 1);
			glVertex3f(5.f, -3.f, -2.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(1, 1);
			glVertex3f(5.f, -3.f, 0.f);
			glNormal3f(-1.f, 0.f, 0.f);
			glTexCoord2f(1, 0);
			glVertex3f(5.f, 1.f, 0.f);
		}
		glEnd();
	}
	glPopMatrix();

	//Front wall
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, seriousWallBase);
		applyFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(0, 0);
			glVertex3f(-5.f, -2.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(0, 1);
			glVertex3f(-5.f, -3.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(10, 1);
			glVertex3f(5.f, -3.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(10, 0);
			glVertex3f(5.f, -2.f, -10.f);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, seriousWallTop);
		applyFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(0, 0);
			glVertex3f(-5.f, 3.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(0, 1);
			glVertex3f(-5.f, -2.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(3.5f, 1);
			glVertex3f(-1.5f, -2.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(3.5f, 0);
			glVertex3f(-1.5f, 3.f, -10.f);

			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(3.5f, 0);
			glVertex3f(-1.5f, 3.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(3.5f, .2f);
			glVertex3f(-1.5f, 2.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(6.5f, .2f);
			glVertex3f(1.5f, 2.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(6.5f, 0);
			glVertex3f(1.5f, 3.f, -10.f);

			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(3.5f, .8f);
			glVertex3f(-1.5f, -1.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(3.5f, 1);
			glVertex3f(-1.5f, -2.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(6.5f, 1);
			glVertex3f(1.5f, -2.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(6.5f, .8f);
			glVertex3f(1.5f, -1.f, -10.f);

			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(6.5f, 0);
			glVertex3f(1.5f, 3.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(6.5f, 1);
			glVertex3f(1.5f, -2.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(10, 1);
			glVertex3f(5.f, -2.f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(10, 0);
			glVertex3f(5.f, 3.f, -10.f);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, glass);
		glEnable(GL_BLEND);
		glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4>{ .2f, .2f, .2f, .4f }.data());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, std::array<GLfloat, 4>{ 1.f, 1.f, 1.f, .4f }.data());
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(0, 0);
			glVertex3f(-1.25f, 1.75f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(0, 1);
			glVertex3f(-1.25f, -0.75f, -10.f);
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(1, 1);
			glVertex3f(1.25f, -0.75f, -10.f);
			glNormal3f(0.f, 0.f, 1.f);
			glTexCoord2f(1, 0);
			glVertex3f(1.25f, 1.75f, -10.f);
		}
		glEnd();
		glDisable(GL_BLEND);
		glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4>{ 1.f, 1.f, 1.f, 1.f }.data());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, std::array<GLfloat, 4>{ 1.f, 1.f, 1.f, 1.f }.data());
	}
	glPopMatrix();

	//Back wall
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, seriousWallBase);
		applyFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(10, 0);
			glVertex3f(5.f, -2.f, 0.f);
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(10, 1);
			glVertex3f(5.f, -3.f, 0.f);
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(0, 1);
			glVertex3f(-5.f, -3.f, 0.f);
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(0, 0);
			glVertex3f(-5.f, -2.f, 0.f);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, seriousWallTop);
		applyFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(10, 0);
			glVertex3f(5.f, 3.f, 0.f);
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(10, 1);
			glVertex3f(5.f, -2.f, 0.f);
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(0, 1);
			glVertex3f(-5.f, -2.f, 0.f);
			glNormal3f(0.f, 0.f, -1.f);
			glTexCoord2f(0, 0);
			glVertex3f(-5.f, 3.f, 0.f);
		}
		glEnd();
	}
	glPopMatrix();

	//Floor
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, seriousFloor);
		applyFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//drawPlane(Vector3(-5.f, -3.f, -10.f), Vector3(5.f, -3.f, 0.f), Vector3(0.f, 1.f, 0.f));
		glBegin(GL_QUADS);
		for (float z = -10.f; z < 0.f; z += .1f)
		{
			for (float x = -5.f; x < 5.f - .1f; x += .1f)
			{
				glNormal3f(0.f, 1.f, 0.f);
				glTexCoord2f(x + 5.f, z + 10.f);
				glVertex3f(x, -3.f, z);
				glNormal3f(0.f, 1.f, 0.f);
				glTexCoord2f(x + 5.f, z + 10.1f);
				glVertex3f(x, -3.f, z + .1f);
				glNormal3f(0.f, 1.f, 0.f);
				glTexCoord2f(x + 5.1f, z + 10.1f);
				glVertex3f(x + .1f, -3.f, z + .1f);
				glNormal3f(0.f, 1.f, 0.f);
				glTexCoord2f(x + 5.1f, z + 10.f);
				glVertex3f(x + .1f, -3.f, z);
			}
		}
		glEnd();
		/*glBegin(GL_QUADS);
		{
			glNormal3f(0.f, 1.f, 0.f);
			glTexCoord2f(0, 0);
			glVertex3f(-5.f, -3.f, -10.f);
			glNormal3f(0.f, 1.f, 0.f);
			glTexCoord2f(0, 10);
			glVertex3f(-5.f, -3.f, 0.f);
			glNormal3f(0.f, 1.f, 0.f);
			glTexCoord2f(10, 10);
			glVertex3f(5.f, -3.f, 0.f);
			glNormal3f(0.f, 1.f, 0.f);
			glTexCoord2f(10, 0);
			glVertex3f(5.f, -3.f, -10.f);
		}
		glEnd();*/
	}
	glPopMatrix();

	//Ceiling
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, seriousCeiling);
		applyFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		/*glBegin(GL_QUADS);
		for (float z = 0.f; z > -10.f; z -= .1f)
		{
			for (float x = 5.f; x > -5.f + .1f; x -= .1f)
			{
				glNormal3f(0.f, -1.f, 0.f);
				glTexCoord2f(x - 5.1f, z - 10.f);
				glVertex3f(x - .1f, 3.f, z);
				glNormal3f(0.f, -1.f, 0.f);
				glTexCoord2f(x - 5.1f, z - 10.1f);
				glVertex3f(x - .1f, 3.f, z - .1f);
				glNormal3f(0.f, -1.f, 0.f);
				glTexCoord2f(x - 5.f, z - 10.1f);
				glVertex3f(x, 3.f, z - .1f);
				glNormal3f(0.f, -1.f, 0.f);
				glTexCoord2f(x - 5.f, z - 10.f);
				glVertex3f(x, 3.f, z);
			}
		}
		glEnd();*/
		glBegin(GL_QUADS);
		{
			glNormal3f(0.f, -1.f, 0.f);
			glTexCoord2f(0, 0);
			glVertex3f(5.f, 3.f, -10.f);
			glNormal3f(0.f, -1.f, 0.f);
			glTexCoord2f(0, 10);
			glVertex3f(5.f, 3.f, 0.f);
			glNormal3f(0.f, -1.f, 0.f);
			glTexCoord2f(10, 10);
			glVertex3f(-5.f, 3.f, 0.f);
			glNormal3f(0.f, -1.f, 0.f);
			glTexCoord2f(10, 0);
			glVertex3f(-5.f, 3.f, -10.f);
		}
		glEnd();
	}
	glPopMatrix();
}

void Scene::drawMirrorQuad()
{

}

void Scene::drawSolarSystem()
{
	glPushMatrix();
	{
		glTranslatef(0.f, -.5f, -5.f);
		glPushMatrix();
		{
			glRotatef(4 * lightRotation, 0, 1, 0);
			sun.assignCurrentTransformationMatrix();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glRotatef(lightRotation, 0, 1, 0);
			glTranslatef(-1.f, 0.f, 0.f);
			planet2.assignCurrentTransformationMatrix();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glRotatef(lightRotation, 0, 1, 0);
			glTranslatef(1.f, 0.f, 0.f);
			planet1.assignCurrentTransformationMatrix();
			glPushMatrix();
			{
				glRotatef(2 * lightRotation, 0, 0, 1);
				glTranslatef(-0.25f, 0.f, 0.f);
				moon1.assignCurrentTransformationMatrix();
				glPushMatrix();
				{
					glRotatef(4 * lightRotation, 1, 1, 1);
					glTranslatef(-.125f, 0.f, 0.f);
					moonsMoon1.assignCurrentTransformationMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
	glEnable(GL_BLEND);
	for (unsigned i = 0; i < transparentShapes.size(); ++i)
		if (transparentShapes[i]->getTexture() != nullptr) transparentShapes[i]->render((short unsigned)currentFilter);
		else transparentShapes[i]->render();
	glDisable(GL_BLEND);
}