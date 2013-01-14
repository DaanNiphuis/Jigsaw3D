#include "Debug.h"
#include "GlobalProperties.h"
#include "PuzzleLayout.h"
#include "PuzzleVisual.h"
#include "Renderer.h"
#include "Scene.h"
#include "Test.h"

#include "GL/freeglut.h"

Scene* scene;
bool programClosed = false;

void create()
{
	Renderer::createInstance(gp::SCREEN_WIDTH, gp::SCREEN_HEIGHT);
	Renderer* renderer = Renderer::getInstance();
	renderer->defaultSettings3D();

	scene = new Scene();
	scene->select();
}

void destroy()
{
	delete scene;

	Renderer::destroyInstance();
}

void update()
{
	scene->update(0);
}

void draw()
{
	if (programClosed)
		return;

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
			programClosed = true;
			destroy();
			glutLeaveMainLoop();
			break;
	}

	scene->feedKey(key);
}

void specialKeys(int /*key*/, int /*x*/, int /*y*/)
{

}

void mouse(int /*btn*/, int state, int /*x*/, int /*y*/)
{
	if (state == GLUT_DOWN)
	{
		scene->getCamera().startMouseMotion();
	}
	if (state == GLUT_UP)
	{
		scene->getCamera().stopMouseMotion();
	}
}

void motion(int x, int y)
{
	scene->getCamera().feedMousePosition(x, y);
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
	glutInitContextVersion(3, 2);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutCreateWindow("Puzzle");
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutDisplayFunc(draw);
	glutIdleFunc(draw);

	create();
	
	const PuzzleLayout* solution = Test::runDifficultTest();

	PuzzleVisual* visual = new PuzzleVisual(*solution, scene->getCamera());
	visual->setPosition(Vector3(0,0,0));
	visual->setScale(Vector3(10,10,10));
	scene->add(visual);

	delete solution;

	glutMainLoop();
}
