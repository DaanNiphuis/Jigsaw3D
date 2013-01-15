#include "RenderWindow.h"

#include "Debug.h"

#include <SDL/SDL.h>

#include "OpenGL.h"

struct HiddenMembers
{
	SDL_Window* window;
	SDL_GLContext context;
};

void RenderWindow::endFrame()
{
	SDL_GL_SwapWindow(static_cast<HiddenMembers*>(m_hiddenMembers)->window);
}

void* RenderWindow::getHandle() const
{
	return static_cast<HiddenMembers*>(m_hiddenMembers)->window;
}

/* A simple function that prints a message, the error code returned by SDL,
 * and quits the application */
void sdldie(const char *msg)
{
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}

RenderWindow::RenderWindow(const char* p_name, int p_screenWidth, int p_screenHeight, bool p_fullScreen)
{
	m_hiddenMembers = new HiddenMembers();

	if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
        sdldie("Unable to initialize SDL"); /* Or die on error */
 
    /* Request opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
 
    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
 
    /* Create our window centered at 512x512 resolution */
	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	if (p_fullScreen)
		flags |= SDL_WINDOW_FULLSCREEN;
    static_cast<HiddenMembers*>(m_hiddenMembers)->window = SDL_CreateWindow(p_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_screenWidth, p_screenHeight, flags);
    if (!static_cast<HiddenMembers*>(m_hiddenMembers)->window) /* Die if creation failed */
        sdldie("Unable to create window");

	//SDL_DisplayMode mode;
	//if (SDL_GetWindowDisplayMode(static_cast<HiddenMembers*>(m_hiddenMembers)->window, &mode) < 0)
	//	sdldie("Unable to get window display mode");
	// modify display mode
	// [snip]
	//if (SDL_SetWindowDisplayMode(static_cast<HiddenMembers*>(m_hiddenMembers)->window, &mode) < 0)
	//	sdldie("Unable to set window display mode");
 
    /* Create our opengl context and attach it to our window */
    static_cast<HiddenMembers*>(m_hiddenMembers)->context = SDL_GL_CreateContext(static_cast<HiddenMembers*>(m_hiddenMembers)->window);
 
    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);
}

RenderWindow::~RenderWindow()
{
	// Delete our opengl context, destroy our window, and shutdown SDL
	SDL_GL_DeleteContext(static_cast<HiddenMembers*>(m_hiddenMembers)->context);
    SDL_DestroyWindow(static_cast<HiddenMembers*>(m_hiddenMembers)->window);
	SDL_Quit();

	delete m_hiddenMembers;
}


