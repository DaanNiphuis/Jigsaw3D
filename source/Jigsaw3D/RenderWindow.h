#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include "Uncopyable.h"

class RenderWindow: Uncopyable
{
public:
	void endFrame();

	void* getHandle() const;
	
private:
	// Only the renderer can construct the render target.
	friend class Renderer;

	RenderWindow(const char* p_name, int p_screenWidth, int p_screenHeight, bool p_fullScreen);
	~RenderWindow();

	void* m_hiddenMembers;
};

#endif
