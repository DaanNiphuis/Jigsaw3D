#include "ObjectCamera.h"

#include "Vector3.h"

#include <vector>

ObjectCamera::ObjectCamera(float p_distance):
	m_startPosition(0, 0, p_distance),
	m_mousePositionFed(false),
	m_prevMousePosInitialized(false)
{
	setTarget(Vector3(0, 0, 0));
	setPosition(m_startPosition);
}

ObjectCamera::~ObjectCamera()
{
}

void ObjectCamera::update(float /*p_timePassed*/)
{
	if (m_prevMousePosInitialized)
	{
		const float velocityFactor = 0.01f;
		float mouseVelocityX = float(m_mouseX - m_prevMouseX) * velocityFactor;
		float mouseVelocityY = float(m_mouseY - m_prevMouseY) * velocityFactor;
		m_rotation.x += mouseVelocityY;
		m_rotation.y += mouseVelocityX;

		float rotationXBound = HALF_PI * 0.98f;
		m_rotation.x = clamp(m_rotation.x, -rotationXBound, rotationXBound);

		while (m_rotation.y > TWO_PI)
			m_rotation.y -= TWO_PI;
		while (m_rotation.y < -TWO_PI)
			m_rotation.y += TWO_PI;

		Vector3 pos = m_startPosition;

		pos.rotateAroundX(m_rotation.x);
		pos.rotateAroundY(-m_rotation.y);

		setTarget(Vector3(0, 0, 0));
		setPosition(pos);
	}

	m_prevMouseX = m_mouseX;
	m_prevMouseY = m_mouseY;
	if (m_mousePositionFed)
		m_prevMousePosInitialized = true;
}

void ObjectCamera::startMouseMotion()
{
	m_mousePositionFed = false;
	m_prevMousePosInitialized = false;
}

void ObjectCamera::stopMouseMotion()
{
	m_mousePositionFed = false;
	m_prevMousePosInitialized = false;
}

void ObjectCamera::feedMousePosition(int p_x, int p_y)
{
	m_mouseX = p_x;
	m_mouseY = p_y;
	m_mousePositionFed = true;
}