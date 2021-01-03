#include "Scene.h"

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in)
{
	//Seed the rng
	srand(time(NULL));

	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// Other OpenGL / render setting should be applied here.
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Init texture filtering as trilinear
	currentFilter = Filters::TRILINEAR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Init rotations
	rotation = 0.f;
	rotationMultiplier = 1.f;
	rotationSpeed = 10.f;

	//Init lights
	{
		ambientLight->makeAmbient(std::array<GLfloat, 4>{.1f, .1f, .1f, 1.f}.data());
		/*ambientLight->makeDiffuse(
			std::array<GLfloat, 4>{.75f, .75f, .75f, 1.f}.data(),
			std::array<GLfloat, 4>{-1.f, 1.f, 1.f, 0.f}.data(),
			.5f, .25f, .125f);*/
		Scene::pointLightPosition = { 0.f, -2.f, -8.5f, 1.f };
		pointLight->makeDiffuse(
			std::array<GLfloat, 4>{0.f, .25f, .25f, 1.f}.data(),
			pointLightPosition.data(),
			.125f, .25f, .5f);
		spotLight->makeSpot(
			std::array<GLfloat, 4>{1.f, 1.f, 1.f, 1.f}.data(),
			std::array<GLfloat, 4>{0.f, 2.125f, -5.f, 1.f}.data(),
			std::array<GLfloat, 4>{0.f, -1.f, 0.f, 0.f}.data(),
			50.f, 5.f, .5f, .125f, .0675f);
		pointLightSphere.setRadius(.125f);
		pointLightSphere.setResolution(10);
		pointLightSphere.setColor3f(0.f, 0.f, 1.f);
		pointLightSphere.generateShape();
		timeToFlicker = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.f));	//Generate time to the next flicker between 0 and 10 seconds
		flickerTimer = 0.f;
	}

	//Init Camera
	{
		camera.setInput(in);
		camera.setSpeed(10);
		camera.setSensitivity(10);
		currentCameraType = camera.getTypePointer();
	}

	// Initialise textures
	{
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
		glass2 = SOIL_load_OGL_texture("gfx/transparent-glass.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		earthTexture = SOIL_load_OGL_texture("gfx/earth-diffuse.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		moonTexture = SOIL_load_OGL_texture("gfx/moon-diffuse.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		marsTexture = SOIL_load_OGL_texture("gfx/mars-diffuse.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		transparentBox = SOIL_load_OGL_texture("gfx/transparentCrate.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		grassBillboardTexture = SOIL_load_OGL_texture("gfx/bush-billboard.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		treeBillboardTexture = SOIL_load_OGL_texture("gfx/tree-billboard.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		steveTexture = SOIL_load_OGL_texture("gfx/Steve.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	}

	//Init skybox
	skybox.setTexture(sky);

	//Init models
	{
		chair.load("models/13494_Folding_Chairs_v1_L3_lowpoly.obj", "gfx/13494_Folding_Chairs_diff.jpg");
		table.load("models/10233_Kitchen_Table_v2_max2011_it2.obj", "gfx/10233_Kitchen_Table_v1_Diffuse.jpg");
		lamp_struct.load("models/HangingLight_triangles_struct.obj", "gfx/OldFlorecentLight.jpg");
		lamp_neons.load("models/HangingLight_triangles_neons.obj", "gfx/OldFlorecentLight.jpg");
		lamp_neons.setAsLight(true);
		trump.load("models/trump_centerfixed.obj", "gfx/trump.png");
	}

	//Init Shapes
	//Init mirror
	{
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
	}
	//Init window
	{
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
	}
	//Init spheres for solar system (i'm not initialising a position except for the sun, as it would then be difficult to rotate, translate and scale the shapes, so their center is at 0,0,0)
	{
		sun.setRadius(.5f);
		sun.setResolution(50);
		sun.setColor4f(1.f, .65f, 0.f, .875f);		//Orange
		sun.setPosition(0.f, -.5f, -5.f);
		sun.generateShape();
		planet1.setRadius(.125f);
		planet1.setResolution(50);
		planet1.setTexture(earthTexture);
		planet1.generateShape();
		planet2.setRadius(.25f);
		planet2.setResolution(50);
		planet2.setTexture(marsTexture);
		planet2.generateShape();
		moon1.setRadius(.0675f);
		moon1.setResolution(25);
		moon1.setTexture(moonTexture);
		moon1.generateShape();
		moonsMoon1.setRadius(.05f);
		moonsMoon1.setResolution(25);
		moonsMoon1.setColor3f(1.f, 1.f, .2f);	//Yellow
		moonsMoon1.generateShape();
	}
	//Init transparent shapes
	{
		transparentCube1.setPosition(-1.125f, -1.f, -7.5f);
		transparentCube1.setColor4f(.6f, .2f, .2f, .875f);
		transparentCube1.setScale(.125f, .125f, .125f);
		transparentCube1.generateShape();
		transparentCube2.setPosition(1.f, -.875f, -2.675f);
		transparentCube2.setTexture(glass2);
		transparentCube2.setScale(.25f, .25f, .25f);
		transparentCube2.setAmbientMaterial(std::array<GLfloat, 4> { .2f, .2f, .2f, 1.f});	//I'd like this cube to be darker so we can better see the lighting taking place
		transparentCube2.generateShape();	//I did not set the transparency as the png file already contains this information
		transparentCube3.setPosition(.375f, -.875f, -6.875f);
		transparentCube3.setTexture(transparentBox);
		transparentCube3.setScale(.25f, .25f, .25f);
		transparentCube3.renderInsideShape(true);
		transparentCube3.setAmbientMaterial(std::array<GLfloat, 4> { .2f, .2f, .2f, 1.f});	//I'd like this cube to be darker so we can better see the lighting taking place
		transparentCube3.generateShape();	//I did not set the transparency as the png file already contains this information
		transparentCylinder1.setRadius(.125f);
		transparentCylinder1.setStackResolution(2);
		transparentCylinder1.setResolution(20);
		transparentCylinder1.setColor4f(0.125f, .875f, .5f, .375f);
		transparentCylinder1.setPosition(-1.f, -.625f, -4.f);
		transparentCylinder1.generateShape();
		transparentCylinderCap.setRadius(.125f);
		transparentCylinderCap.setResolution(20);
		transparentCylinderCap.setColor4f(.125f, .875f, .5f, .375f);
		transparentCylinderCap.setPosition(-1.f, -.125f, -4.f);
		transparentCylinderCap.renderInsideShape(true);
		transparentCylinderCap.generateShape();
	}
	//Push references of all transparent shapes into the according vector
	{
		transparentShapes.push_back(&sun);
		transparentShapes.push_back(&transparentCube1);
		transparentShapes.push_back(&transparentCube2);
		transparentShapes.push_back(&transparentCube3);
		transparentShapes.push_back(&transparentCylinder1);
		transparentShapes.push_back(&transparentCylinderCap);
	}

	//Init room vertex arrays
	{
		//Billboards init
		Scene::billboardVerts = {
			-2.5f, 0.f, -0.f,
			-2.5f, -3.f, 0.f,
			2.5f, -3.f, 0.f,
			2.5f, 0.f, 0.f,
			-2.f, 4.f, -0.f,
			-2.f, -3.f, 0.f,
			2.f, -3.f, 0.f,
			2.f, 4.f, 0.f
		};
		Scene::billboardTexcoords = {
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0
		};
		Scene::billboardNormals = {
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f
		};

		//Room init
		Scene::seriousRoomVerts = {
			//Bottom walls
			-5.f, -2.f, 0.f,
			-5.f, -3.f, 0.f,
			-5.f, -3.f, -10.f,
			-5.f, -2.f, -10.f,
			5.f, -2.f, -10.f,
			5.f, -3.f, -10.f,
			5.f, -3.f, -2.f,
			5.f, -2.f, -2.f,
			-5.f, -2.f, -10.f,
			-5.f, -3.f, -10.f,
			5.f, -3.f, -10.f,
			5.f, -2.f, -10.f,
			5.f, -2.f, 0.f,
			5.f, -3.f, 0.f,
			-5.f, -3.f, 0.f,
			-5.f, -2.f, 0.f,
			//Top walls
			-5.f, 3.f, 0.f,
			-5.f, -2.f, 0.f,
			-5.f, -2.f, -2.5f,
			-5.f, 3.f, -2.5f,
			-5.f, 3.f, -2.5f,
			-5.f, 2.f, -2.5f,
			-5.f, 2.f, -7.5f,
			-5.f, 3.f, -7.5f,
			-5.f, 3.f, -7.5f,
			-5.f, -2.f, -7.5f,
			-5.f, -2.f, -10.f,
			-5.f, 3.f, -10.f,
			5.f, 3.f, -10.f,
			5.f, -2.f, -10.f,
			5.f, -2.f, -2.f,
			5.f, 3.f, -2.f,
			5.f, 3.f, -2.f,
			5.f, 1.f, -2.f,
			5.f, 1.f, 0.f,
			5.f, 3.f, 0.f,
			-5.f, 3.f, -10.f,
			-5.f, -2.f, -10.f,
			-1.5f, -2.f, -10.f,
			-1.5f, 3.f, -10.f,
			-1.5f, 3.f, -10.f,
			-1.5f, 2.f, -10.f,
			1.5f, 2.f, -10.f,
			1.5f, 3.f, -10.f,
			-1.5f, -1.f, -10.f,
			-1.5f, -2.f, -10.f,
			1.5f, -2.f, -10.f,
			1.5f, -1.f, -10.f,
			1.5f, 3.f, -10.f,
			1.5f, -2.f, -10.f,
			5.f, -2.f, -10.f,
			5.f, 3.f, -10.f,
			5.f, 3.f, 0.f,
			5.f, -2.f, 0.f,
			-5.f, -2.f, 0.f,
			-5.f, 3.f, 0.f,
			//Ceiling
			5.f, 3.f, -10.f,
			5.f, 3.f, 0.f,
			-5.f, 3.f, 0.f,
			-5.f, 3.f, -10.f,
			//Glass
			-1.25f, 1.75f, -10.f,
			-1.25f, -0.75f, -10.f,
			1.25f, -0.75f, -10.f,
			1.25f, 1.75f, -10.f,
			//Door
			5.f, 1.f, -2.f,
			5.f, -3.f, -2.f,
			5.f, -3.f, 0.f,
			5.f, 1.f, 0.f
		};
		Scene::seriousRoomTexcoords = {
			//Bottom walls
			0, 0,
			0, 1,
			10, 1,
			10, 0,
			8, 0,
			8, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 1,
			10, 1,
			10, 0,
			10, 0,
			10, 1,
			0, 1,
			0, 0,
			//Top walls
			0, 0,
			0, 1,
			2.5f, 1,
			2.5f, 0,
			2.5f, 0,
			2.5f, .2f,
			7.5f, .2f,
			7.5f, 0,
			7.5f, 0,
			7.5f, 1,
			10, 1,
			10, 0,
			8, 0,
			8, 1,
			0, 1,
			0, 0,
			2, 0,
			2, .4f,
			0, .4f,
			0, 0,
			0, 0,
			0, 1,
			3.5f, 1,
			3.5f, 0,
			3.5f, 0,
			3.5f, .2f,
			6.5f, .2f,
			6.5f, 0,
			3.5f, .8f,
			3.5f, 1,
			6.5f, 1,
			6.5f, .8f,
			6.5f, 0,
			6.5f, 1,
			10, 1,
			10, 0,
			10, 0,
			10, 1,
			0, 1,
			0, 0,
			//Ceiling
			0, 0,
			0, 10,
			10, 10,
			10, 0,
			//Glass
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			//Door
			0, 0,
			0, 1,
			1, 1,
			1, 0
		};
		Scene::seriousRoomNormals = {
			//Bottom walls
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
			//Top walls
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
			//Ceiling
			0.f, -1.f, 0.f,
			0.f, -1.f, 0.f,
			0.f, -1.f, 0.f,
			0.f, -1.f, 0.f,
			//Glass
			0.f, 0.f, 1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, 1.f,
			//Door
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f
		};

		//Floor init
		for (float z = -10.f; z < 0.f; z += .1f)
		{
			for (float x = -5.f; x < 5.f - .1f; x += .1f)
			{
				for (unsigned char i = 0; i < 4; ++i)
				{
					seriousFloorNormals.push_back(0.f);
					seriousFloorNormals.push_back(1.f);
					seriousFloorNormals.push_back(0.f);
				}

				seriousFloorTexcoords.push_back(x + 5.f);
				seriousFloorTexcoords.push_back(z + 10.f);
				seriousFloorTexcoords.push_back(x + 5.f);
				seriousFloorTexcoords.push_back(z + 10.1f);
				seriousFloorTexcoords.push_back(x + 5.1f);
				seriousFloorTexcoords.push_back(z + 10.1f);
				seriousFloorTexcoords.push_back(x + 5.1f);
				seriousFloorTexcoords.push_back(z + 10.f);

				seriousFloorVerts.push_back(x);
				seriousFloorVerts.push_back(-3.f);
				seriousFloorVerts.push_back(z);
				seriousFloorVerts.push_back(x);
				seriousFloorVerts.push_back(-3.f);
				seriousFloorVerts.push_back(z + .1f);
				seriousFloorVerts.push_back(x + .1f);
				seriousFloorVerts.push_back(-3.f);
				seriousFloorVerts.push_back(z + .1f);
				seriousFloorVerts.push_back(x + .1f);
				seriousFloorVerts.push_back(-3.f);
				seriousFloorVerts.push_back(z);
			}
		}
	}
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
	
	//Change camera mode
	if (input->isKeyDownOnce(GLUT_KEY_F4))
	{
		if (*currentCameraType == CameraTypes::TRACKING) *currentCameraType = CameraTypes::FREE;
		else *currentCameraType = static_cast<CameraTypes>((int)*currentCameraType + 1);
		camera.update();
	}

	//Toggle steve mode
	if (!stevemode && input->isKeyDownOnce(GLUT_KEY_F5))
	{
		stevemode = true;
		updateTextures(stevemode);
	}
	else if (stevemode && input->isKeyDownOnce(GLUT_KEY_F5))
	{
		stevemode = false;
		updateTextures(stevemode);
	}

	//Other user interactions
	if (input->isKeyDown('t') && rotationMultiplier > .75f) rotationMultiplier -= .1f * dt;
	if (input->isKeyDown('y') && rotationMultiplier < 1.5f) rotationMultiplier += .1f * dt;
	if (input->isKeyDown('g') && rotationSpeed > 10.f) rotationSpeed -= 50.f * dt;
	if (input->isKeyDown('h') && rotationSpeed < 200.f) rotationSpeed += 50.f * dt;

	//Handle camera input, but not for tracking since we dont want to
	if(*currentCameraType != CameraTypes::TRACKING) camera.handleInput(dt);
}

void Scene::update(float dt)
{
	//Update the rotation angle by 10 degrees per second
	rotation += rotationSpeed * dt;

	//Update the position of the point light (rotate around the Y axis)
	//In the form: radius * cosf(angle in radians) + translation.x;
	pointLightPosition[0] = 3.5f * rotationMultiplier * cosf(rotation * PI / 180.f);
	pointLightPosition[2] = 3.5f * rotationMultiplier * sinf(rotation * PI / 180.f) - 5.f;
	pointLight->setPosition(pointLightPosition.data());
	//Update the camera position in the case of Tracking mode by reusing the light's rotation coordinates
	if (*currentCameraType == CameraTypes::TRACKING)
	{
		camera.setPosition(Vector3(-pointLightPosition[0], -.5f, -pointLightPosition[2] - 10.f));
		//We need to define the angle so that it looks at the target
		camera.setAngles(rotation + 90, 0.f, 0.f);
	}
	//Update camera with the new inputs
	camera.update();

	// update scene related variables.
	skybox.setPos(camera.getPosition(true));

	//Update the transparent shapes order of rendering relative to the camera position (furthest to be rendered first, closest to be rendered last)
	std::sort(transparentShapes.begin(), transparentShapes.end(), [=](BasicShape* s1, BasicShape* s2)
		{
			//We want to return true if the first shape is further to the camera than the second shape
			return s1->getPosition() - camera.getPosition() > s2->getPosition() - camera.getPosition();
		});

	//Flicker the spot light
	flickerTimer += dt;
	if (flickerTimer >= timeToFlicker && spotLight->isEnabled() && !fullbright)
	{
		flickerTimer = 0;
		spotLight->disable();
		lamp_neons.setAsLight(false);
		timeToFlicker = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.f));	//Generate time to the next flicker between 0 and 10 seconds
	}
	else if (flickerTimer >= .1f && !spotLight->isEnabled() && !fullbright)
	{
		flickerTimer = 0;
		spotLight->enable();
		lamp_neons.setAsLight(true);
	}

	// Calculate FPS for output
	calculateFPS();
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(
		camera.getPosition(true).x, camera.getPosition(true).y, camera.getPosition(true).z,
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

	//Draw floor reflections inside the stencil quad or mirror surface
	if(!fullbright) drawReflections();
	//Draw the real world
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
	frametime = glutGet(GLUT_ELAPSED_TIME);

	if (frametime - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (frametime - timebase));
		timebase = frametime;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second, and every other UI elements
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	sprintf_s(positionText, "Position: %g, %g, %g", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	sprintf_s(viewText, "View: %g, %g, %g", camera.getYaw(), camera.getPitch(), camera.getRoll());
	sprintf_s(rotationMultiplierText, "Rotation radius multiplier: %g", rotationMultiplier);
	sprintf_s(rotationSpeedText, "Rotation speed: %g degrees/s", rotationSpeed);
	switch (currentFilter)
	{
	case Filters::POINT:		sprintf_s(filterMode, "Filter: POINT SAMPLE");	break;
	case Filters::BILINEAR:		sprintf_s(filterMode, "Filter: BILINEAR");		break;
	case Filters::MIPMAPING:	sprintf_s(filterMode, "Filter: MIPMAPPING");	break;
	case Filters::TRILINEAR:	sprintf_s(filterMode, "Filter: TRILINEAR");		break;
	default:					sprintf_s(filterMode, "Filter: Error");			break;
	}
	switch (*currentCameraType)
	{
	case CameraTypes::FREE:			sprintf_s(cameraMode, "Camera: First Person");	break;
	case CameraTypes::THIRDPERSON:	sprintf_s(cameraMode, "Camera: Third Person");	break;
	case CameraTypes::TRACKING:		sprintf_s(cameraMode, "Camera: Tracking");		break;
	default:						sprintf_s(cameraMode, "Camera: Error");			break;
	}
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
	displayText(-1.f, 0.84f, 1.f, 0.f, 0.f, positionText);
	displayText(-1.f, 0.78f, 1.f, 0.f, 0.f, viewText);
	displayText(-1.f, 0.72f, 1.f, 0.f, 0.f, filterMode);
	displayText(-1.f, 0.66f, 1.f, 0.f, 0.f, cameraMode);
	displayText(-1.f, 0.60f, 1.f, 0.f, 0.f, rotationMultiplierText);
	displayText(-1.f, 0.54f, 1.f, 0.f, 0.f, rotationSpeedText);
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

void Scene::renderSeriousRoom(bool renderingReflection)
{
	//RENDER LIGHTS
	if (!fullbright)
	{
		ambientLight->render();
		pointLight->render();
		glPushMatrix();
		{
			glTranslatef(pointLight->getPosition()[0], pointLight->getPosition()[1], pointLight->getPosition()[2]);
			pointLightSphere.render();
		}
		glPopMatrix();
		spotLight->render();
	}

	//Render billboards
	//Render in order since this will never change
	glPushMatrix();
	{
		glTranslatef(0.f, 0.f, -15.f);
		drawTreeBillboard();
		glPushMatrix();
		{
			glTranslatef(-3.5f, 0.f, 2.f);
			glRotatef(22.5f, 0, 1, 0);
			drawTreeBillboard();
		}
		glPopMatrix();
		glTranslatef(3.5f, 0.f, 2.f);
		glRotatef(-22.5f, 0, 1, 0);
		drawTreeBillboard();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.f, 0.f, -12.5f);
		glPushMatrix();
		{
			glPushMatrix();
			{
				glTranslatef(-3.f, 0.f, .5f);
				glRotatef(22.5f, 0, 1, 0);
				drawGrassBillboard();
			}
			glPopMatrix();
			glTranslatef(3.f, 0.f, .5f);
			glRotatef(-22.5f, 0, 1, 0);
			drawGrassBillboard();
		}
		glPopMatrix();
		drawGrassBillboard();
	}
	glPopMatrix();

	//Create the room's walls
	makeSeriousWalls();

	//Render mirror
	if (!renderingReflection)	//Dont render twice, or it will overlap and appear glitchy
	{
		mirrorLeftEdge.render((short unsigned)currentFilter);
		mirrorRightEdge.render((short unsigned)currentFilter);
		mirrorTopEdge.render((short unsigned)currentFilter);
		mirrorBottomEdge.render((short unsigned)currentFilter);
	}

	//Render window
	windowLeftEdge.render((short unsigned)currentFilter);
	windowRightEdge.render((short unsigned)currentFilter);
	windowTopEdge.render((short unsigned)currentFilter);
	windowBottomEdge.render((short unsigned)currentFilter);

	//Render models
	glEnable(GL_NORMALIZE);	//Normalise the models, so that the lighting happens properly
	glPushMatrix();
	{
		glPushMatrix();
		{
			glTranslatef(4.f, -3.f, -8.f);
			glRotatef(-90, 1, 0, 0);
			glRotatef(-45, 0, 0, 1);
			glScalef(.075f, .075f, .075f);
			chair.render((short unsigned)currentFilter);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(3.f, -3.f, -9.f);
			glRotatef(-90, 1, 0, 0);
			glRotatef(-90, 0, 0, 1);
			glScalef(.075f, .075f, .075f);
			chair.render((short unsigned)currentFilter);
		}
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.f, -3.f, -5.f);
		glRotatef(-90, 1, 0, 0);
		glScalef(.025f, .025f, .025f);
		table.render((short unsigned)currentFilter);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.f, 2.95f, -5.f);
		glRotatef(-90, 0, 1, 0);
		glScalef(20.f, 20.f, 20.f);
		lamp_struct.render((short unsigned)currentFilter);
		lamp_neons.render((short unsigned)currentFilter);
	}
	glPopMatrix();
	//Render trump based on the camera position
	glPushMatrix();
	//That way we only see the model in the mirror while in first person, so it is not annoying our view
	if(*currentCameraType == CameraTypes::THIRDPERSON || renderingReflection && *currentCameraType != CameraTypes::TRACKING)
	{
		glTranslatef(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		glRotatef(180 - camera.getYaw(), 0, 1, 0);
		glScalef(2.f, 2.f, 2.f);
		trump.render((short unsigned)currentFilter);
	}
	glPopMatrix();
	glDisable(GL_NORMALIZE);	//Finihed rendering models, undo normalize

	//Render solar system hierchical animations
	drawSolarSystem();

	//Draw shadows
	if (spotLight->isEnabled() && !fullbright) renderShadows(renderingReflection);

	//Render all transparent shapes once everything else is rendered
	if(!fullbright) glEnable(GL_BLEND);
	glPushMatrix();
	{
		//When rendering the reflection, apply the sorted render order would make no sense and all our transparent shapes would appear mad glitchy
		//So instead, since the mirror is on one fixed place, it is safe to say that the render order will never change, as we wont be able
		//to go around the table in the virtual world. Therefore, it is best to render them manually as follows.
		if (renderingReflection)
		{
			glScalef(-1.f, 1.f, 1.f);	//Reapply the original scale, otherwise objects are rendered insideout and glitchy
			glCullFace(GL_BACK);		//Because we re-applied the original scale, need to make sure the backface is culled and not the front face
			glPushMatrix();
			{
				glTranslatef(-2.f * transparentCube2.getPosition().x, 0.f, 0.f);
				transparentCube2.render((short unsigned)currentFilter);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(-2.f * transparentCube3.getPosition().x, 0.f, 0.f);
				transparentCube3.render((short unsigned)currentFilter);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(-2.f * transparentCylinder1.getPosition().x, 0.f, 0.f);
				transparentCylinder1.render();
				transparentCylinderCap.render();
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(-2.f * transparentCube1.getPosition().x, 0.f, 0.f);
				transparentCube1.render();
			}
			glPopMatrix();
			sun.render();
			glCullFace(GL_FRONT);	//Re-enable front face culling in case any other geometry comes after
		}
		//In the real world, draw objects in a particular sorted by their position relative to the camera 
		else
		{
			for (unsigned i = 0; i < transparentShapes.size(); ++i)
				if (transparentShapes[i]->getTexture() != nullptr) transparentShapes[i]->render((short unsigned)currentFilter);
				else transparentShapes[i]->render();
		}
	}
	glPopMatrix();
	glDisable(GL_BLEND);
}

void Scene::makeSeriousWalls()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, seriousRoomVerts.data());
	glNormalPointer(GL_FLOAT, 0, seriousRoomNormals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, seriousRoomTexcoords.data());

	glBindTexture(GL_TEXTURE_2D, seriousWallBase);
	applyFilter();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDrawArrays(GL_QUADS, 0, 16);

	glBindTexture(GL_TEXTURE_2D, seriousWallTop);
	applyFilter();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDrawArrays(GL_QUADS, 16, 40);

	glBindTexture(GL_TEXTURE_2D, seriousCeiling);
	applyFilter();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDrawArrays(GL_QUADS, 56, 4);

	glBindTexture(GL_TEXTURE_2D, glass);
	if (!fullbright) glEnable(GL_BLEND);
	glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4>{ 1.f, 1.f, 1.f, .4f }.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, std::array<GLfloat, 4>{ 1.f, 1.f, 1.f, .4f }.data());
	glDrawArrays(GL_QUADS, 60, 4);
	glDisable(GL_BLEND);
	glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4>{ 1.f, 1.f, 1.f, 1.f }.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, std::array<GLfloat, 4>{ 1.f, 1.f, 1.f, 1.f }.data());

	glBindTexture(GL_TEXTURE_2D, seriousDoor);
	applyFilter();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDrawArrays(GL_QUADS, 64, 4);

	glVertexPointer(3, GL_FLOAT, 0, seriousFloorVerts.data());
	glNormalPointer(GL_FLOAT, 0, seriousFloorNormals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, seriousFloorTexcoords.data());
	glBindTexture(GL_TEXTURE_2D, seriousFloor);
	applyFilter();
	glDrawArrays(GL_QUADS, 0, seriousFloorVerts.size() / 3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Scene::drawGrassBillboard()
{
	if (!fullbright) glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, grassBillboardTexture);
	applyFilter();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, billboardVerts.data());
	glNormalPointer(GL_FLOAT, 0, billboardNormals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, billboardTexcoords.data());
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glDisable(GL_BLEND);
}

void Scene::drawTreeBillboard()
{
	if(!fullbright) glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, treeBillboardTexture);
	applyFilter();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, billboardVerts.data());
	glNormalPointer(GL_FLOAT, 0, billboardNormals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, billboardTexcoords.data());
	glDrawArrays(GL_QUADS, 4, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glDisable(GL_BLEND);
}

void Scene::drawMirrorQuad()
{
	glBegin(GL_QUADS);
	{
		glNormal3f(1.f, 0.f, 0.f);
		glVertex3f(-5.f, 2.f, -2.5f);
		glNormal3f(1.f, 0.f, 0.f);
		glVertex3f(-5.f, -2.f, -2.5f);
		glNormal3f(1.f, 0.f, 0.f);
		glVertex3f(-5.f, -2.f, -7.5f);
		glNormal3f(1.f, 0.f, 0.f);
		glVertex3f(-5.f, 2.f, -7.5f);
	}
	glEnd();
}

void Scene::drawSolarSystem(bool renderAsShadow)
{
	glPushMatrix();
	{
		glTranslatef(0.f, -.5f, -5.f);	//Sun position
		glRotatef(rotation, 0, 1, 0);
		glPushMatrix();
		{
			glTranslatef(-1.f * rotationMultiplier, 0.f, 0.f);
			glPushMatrix();
			{
				glRotatef(4 * rotation, 0, 1, 0);
				planet2.render((short unsigned)currentFilter, renderAsShadow);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glRotatef(-2.f * rotation, 0, 1, 0);
				glTranslatef(-.5f * rotationMultiplier, 0.f, 0.f);
				glRotatef(-2.f * rotation, 1, 1, 0);
				glScalef(.1f, .1f, .1f);
				glEnable(GL_NORMALIZE);
				trump.render((short unsigned)currentFilter, renderAsShadow);
				glDisable(GL_NORMALIZE);
			}
			glPopMatrix();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(1.f * rotationMultiplier, 0.f, 0.f);
			glPushMatrix();
			{
				glRotatef(-4.f * rotation, 0, 1, 0);
				planet1.render((short unsigned)currentFilter, renderAsShadow);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glRotatef(2 * rotation, 0, 0, 1);
				glTranslatef(-0.25f * rotationMultiplier, 0.f, 0.f);
				moon1.render((short unsigned)currentFilter, renderAsShadow);
				glPushMatrix();
				{
					glRotatef(4 * rotation, 1, 1, 1);
					glTranslatef(-.125f * rotationMultiplier, 0.f, 0.f);
					moonsMoon1.render(false, renderAsShadow);
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0.f, 0.f, -1.f * rotationMultiplier);
			glEnable(GL_NORMALIZE);	//Normalise models for proper lighting
			glPushMatrix();
			{
				glRotatef(-4 * rotation, 0, 1, 1);
				glScalef(0.0025f, 0.0025f, 0.0025f);
				table.render((short unsigned)currentFilter, renderAsShadow);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glRotatef(rotation, 0, 1, 0);
				glTranslatef(-.5f * rotationMultiplier, 0.f, 0.f);
				glRotatef(2.f * rotation, 1, 1, 1);
				glScalef(0.0075f, 0.0075f, 0.0075f);
				chair.render((short unsigned)currentFilter, renderAsShadow);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glRotatef(rotation, 0, 1, 0);
				glTranslatef(.5f * rotationMultiplier, 0.f, 0.f);
				glRotatef(2.f * rotation, 0, 1, 1);
				glScalef(0.0075f, 0.0075f, 0.0075f);
				chair.render((short unsigned)currentFilter, renderAsShadow);
			}
			glPopMatrix();
			glDisable(GL_NORMALIZE);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Scene::setupStencil()
{
	//Clear stencil
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	//Turn off writing to the frame buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//Turn on stencil test
	glEnable(GL_STENCIL_TEST);
	//Set the stencil function to always pass
	glStencilFunc(GL_ALWAYS, 1, 1);
	//Set the stencil operation to replace values when the test passes (basically always, we don't have any depth values initalised yet and the stencil test is set to always)
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//Disable depth test (we don't want to write values to it)
	glDisable(GL_DEPTH_TEST);
	//Draw our mirror quad to the stencil buffer
	drawMirrorQuad();
	//Enable depth testing back
	glEnable(GL_DEPTH_TEST);
	//Turn on rendering to the frame buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//The stencil buffer is not initialised. The reflections to be produced by it are declared inside drawReflections().
}

void Scene::drawReflections()
{
	//Uncomment to debug the reflection iteslf, so it shows up even if out of the stencil
	//glDisable(GL_STENCIL_TEST);
	//Initialise and setup the stencil
	setupStencil();
	//Set stencil function to test if the value is 1 (if we can draw basically)
	glStencilFunc(GL_EQUAL, 1, 1);
	//Set the stencil operation to keep all values (we don't want to write to the stencil now, just use it)
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//The models will be rendered inside out because of the negative scaling, therefore we need to cull the front face and render the backface
	glCullFace(GL_FRONT);
	//Draw the room
	glPushMatrix();
	if(!fullbright)
	{
		glScalef(-1.f, 1.f, 1.f);
		glTranslatef(10.f, 0.f, 0.f);
		renderSeriousRoom(true);
	}
	glPopMatrix();
	glCullFace(GL_BACK);
	//Disable the stencil test (no longer needed, reflections have been rendered)
	glDisable(GL_STENCIL_TEST);
	//Enable alpha blending (to combine the calculated reflection with our stencil quad (mirror quad))
	if(!fullbright) glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Setup material and texture for mirror quad
	/*glBindTexture(GL_TEXTURE_2D, glass);
	glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4>{ .2f, .2f, .2f, .4f }.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, std::array<GLfloat, 4>{ 1.f, 1.f, 1.f, .4f }.data());*/
	glDisable(GL_LIGHTING);
	//Set the colour of the stencil quad (mirror)(note that we put an alpha value, since it's sort of transparent)
	glColor4f(0.8f, 0.8f, 1.f, 0.25f);
	//Draw the stencil quad (mirror)
	drawMirrorQuad();
	//Reset material and texture
	/*glMaterialfv(GL_FRONT, GL_AMBIENT, std::array<GLfloat, 4>{ 1.f, 1.f, 1.f, 1.f }.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, std::array<GLfloat, 4>{ 1.f, 1.f, 1.f, 1.f }.data());*/
	if (!fullbright)  glEnable(GL_LIGHTING);
	//Disable alpha blending (we rendered the mirror)
	glDisable(GL_BLEND);
	//Reflections have now been drawn correctly, next step is to draw the model inside drawModel()
}

void Scene::updateTextures(bool sm)
{
	if (sm)
	{
		sky[0] = steveTexture;
		sky[1] = steveTexture;
		sky[2] = steveTexture;
		sky[3] = steveTexture;
		sky[4] = steveTexture;
		sky[5] = steveTexture;
		seriousWallBase = steveTexture;
		seriousWallTop = steveTexture;
		seriousFloor = steveTexture;
		seriousCeiling = steveTexture;
		seriousDoor = steveTexture;
		wood = steveTexture;
		glass = steveTexture;
		glass2 = steveTexture;
		earthTexture = steveTexture;
		moonTexture = steveTexture;
		marsTexture = steveTexture;
		transparentBox = steveTexture;
		grassBillboardTexture = steveTexture;
		treeBillboardTexture = steveTexture;
		chair.reloadTexture(&steveTexture);
		table.reloadTexture(&steveTexture);
		lamp_neons.reloadTexture(&steveTexture);
		lamp_struct.reloadTexture(&steveTexture);
		trump.reloadTexture(&steveTexture);
	}
	else
	{
		sky[0] = 1;
		sky[1] = 2;
		sky[2] = 3;
		sky[3] = 4;
		sky[4] = 5;
		sky[5] = 6;
		seriousWallBase = 7;
		seriousWallTop = 8;
		seriousFloor = 9;
		seriousCeiling = 10;
		seriousDoor = 11;
		wood = 12;
		glass = 13;
		glass2 = 14;
		earthTexture = 15;
		moonTexture = 16;
		marsTexture = 17;
		transparentBox = 18;
		grassBillboardTexture = 19;
		treeBillboardTexture = 20;
		chair.reloadTexture();
		table.reloadTexture();
		lamp_neons.reloadTexture();
		lamp_struct.reloadTexture();
		trump.reloadTexture();
	}
}

void Scene::drawShadowPlane()
{
	glBegin(GL_QUADS);
	for (unsigned char i = 0; i < 12; i += 3) glVertex3f(tableShadowQuad[i], tableShadowQuad[i + 1], tableShadowQuad[i + 2]);
	glEnd();
}

void Scene::renderShadows(bool drawingReflection)
{
	//Generate the shadow matrix
	Shadow::generateShadowMatrix(shadowMatrix, spotLight->getPosition(), tableShadowQuad.data());

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_STENCIL_TEST);

	if (drawingReflection)
	{
		glStencilFunc(GL_EQUAL, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

		drawShadowPlane();

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 2, 2);
	}
	else
	{
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		drawShadowPlane();

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 1, 1);
	}

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	{
		glMultMatrixf((GLfloat*)shadowMatrix);
		//Render the shadows of every desired objects from here
		//model.render()...
		sun.render(false, true);
		drawSolarSystem(true);
	}
	glPopMatrix();
	//Disable the stencil test if we are rendering the real world
	if(!drawingReflection) glDisable(GL_STENCIL_TEST);
	else
	{
		glStencilFunc(GL_LESS, 0, 1 | 2);
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glColor3f(1.f, 1.f, 1.f);
}