#include "Debug.h"
#include "GlobalProperties.h"
#include "Input.h"
#include "PuzzleLayout.h"
#include "PuzzleVisual.h"
#include "Renderer.h"
#include "Scene.h"
#include "Test.h"
#include "Timer.h"

Scene* scene;
bool programClosed = false;

void create()
{
	Renderer::createInstance("Jigsaw 3D", gp::SCREEN_WIDTH, gp::SCREEN_HEIGHT);
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
	Renderer* renderer = Renderer::getInstance();
	renderer->beginFrame();
	renderer->renderScene();	
	renderer->endFrame();
}

int main(int , char **)
{
	create();

	const PuzzleLayout* solution = Test::runDifficultTest();

	PuzzleVisual* visual = new PuzzleVisual(*solution, scene->getCamera());
	visual->setPosition(Vector3(0,0,0));
	visual->setScale(Vector3(10,10,10));
	scene->add(visual);

	Input* input = Input::getInstance();
	
	Timer frameTimer;
	Timer gameTimer;

	while (input->userQuit() == false && 
		   input->isPressed(Key::Escape) == false)
	{
		gameTimer.update();
		frameTimer.update();

		input->update();
		update();
		draw();

		frameTimer.update();

		// Limit the framerate.
		double frameTime = 1.0 / 60.0;
		double timeLeft = frameTime - frameTimer.getSeconds();
		if (timeLeft > 0)
		{
			Sleep(static_cast<int>(timeLeft * 1000.0));
		}
	}

	delete solution;
}
