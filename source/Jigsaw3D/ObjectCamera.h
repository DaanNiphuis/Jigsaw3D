#ifndef OBJECTCAMERA_H
#define OBJECTCAMERA_H

#include "Camera.h"

class ObjectCamera: public Camera
{
public:
	ObjectCamera(float p_distance);
	virtual ~ObjectCamera();

	void startMouseMotion();
	void stopMouseMotion();
	void feedMousePosition(int p_x, int p_y);
	void feedMouseScroll(int p_scroll);

private:
	virtual void updateImpl(float p_timePassed);

	const ObjectCamera& operator= (const ObjectCamera&) {return *this;}

	int m_mouseX;
	int m_mouseY;
	int m_prevMouseX;
	int m_prevMouseY;
	bool m_prevMousePosInitialized;
	bool m_mousePositionFed;
	const Vector3 m_startPosition;
	Vector3 m_rotation;
};

#endif
