#include "Debug.h"
#include "GlobalProperties.h"
#include "ObjectCamera.h"
#include "PuzzleVisual.h"
#include "Renderer.h"
#include "Scene.h"
#include "Test.h"

#include "GL/freeglut.h"

ObjectCamera* objectCamera;
Scene* scene;

void create()
{
	Renderer::createInstance(gp::SCREEN_WIDTH, gp::SCREEN_HEIGHT);
	Renderer* renderer = Renderer::getInstance();
	renderer->defaultSettings3D();

	objectCamera = new ObjectCamera(100);
	renderer->setWorldCamera(objectCamera);
	scene = new Scene();
	scene->select();
}

void destroy()
{
	delete scene;
	delete objectCamera;

	Renderer::destroyInstance();
}

void update()
{
	objectCamera->update(0);
	scene->update(0);
}

void draw()
{
	update();

	Renderer* renderer = Renderer::getInstance();
	renderer->beginFrame();

	renderer->renderScene();
	
	renderer->endFrame();
}

void keyboard(unsigned char key, int /*x*/, int /*y*/)
{
	switch (key)
	{
		case 0x1B: // Escape
			glutLeaveMainLoop();
			break;
	}
}

void specialKeys(int /*key*/, int /*x*/, int /*y*/)
{

}

void mouse(int /*btn*/, int state, int /*x*/, int /*y*/)
{
	if (state == GLUT_DOWN)
	{
		objectCamera->startMouseMotion();
	}
	if (state == GLUT_UP)
	{
		objectCamera->stopMouseMotion();
	}
}

void motion(int x, int y)
{
	objectCamera->feedMousePosition(x, y);
}

void passiveMotion(int /*x*/, int /*y*/)
{
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(gp::SCREEN_WIDTH, gp::SCREEN_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowPosition(200,100);
	glutCreateWindow("Puzzle");
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutDisplayFunc(draw);
	glutIdleFunc(draw);

	create();

	Test::runDifficultTest();
	glutMainLoop();

	destroy();
}
