#include "ObjectCamera.h"

#include "Input.h"
#include "Vector3.h"

#include <vector>

ObjectCamera::ObjectCamera(float p_distance):
	Camera(),
	m_mouseX(0),
	m_mouseY(0),
	m_prevMouseX(0),
	m_prevMouseY(0),
	m_startPosition(0, 0, p_distance)
{
	setTarget(Vector3(0, 0, 0));
	setPosition(m_startPosition);
	setProjectionType(ProjectionType::Perspective);
}

ObjectCamera::~ObjectCamera()
{
}

void ObjectCamera::updateImpl(float /*p_timePassed*/)
{
	const Input* input = Input::getInstance();

	if (input->isDown(Key::PointerLeft))
	{
		const float velocityFactor = 0.01f;
		const Vector2 mouseVelocity = Input::getInstance()->getPointerVelocity() * velocityFactor;
		m_rotation.x += mouseVelocity.y;
		m_rotation.y += mouseVelocity.x;

		float rotationXBound = Math::HALF_PI * 0.98f;
		m_rotation.x = Math::clamp(m_rotation.x, -rotationXBound, rotationXBound);

		while (m_rotation.y > Math::TWO_PI)
			m_rotation.y -= Math::TWO_PI;
		while (m_rotation.y < -Math::TWO_PI)
			m_rotation.y += Math::TWO_PI;

		Vector3 pos = m_startPosition;

		pos.rotateAroundX(-m_rotation.x);
		pos.rotateAroundY(m_rotation.y);

		setTarget(Vector3(0, 0, 0));
		setPosition(pos);
	}

}
