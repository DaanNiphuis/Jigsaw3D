#include "Camera.h"

#include "GlobalProperties.h"
#include "OpenGL.h"
#include "MathFunctions.h"
#include "Matrix44.h"
#include "Renderer.h"

Camera::Camera() :
	m_position(),
	m_target(0,0,-1),
	m_up(0, 1, 0),
	m_fov(Math::HALF_PI),
	m_projectionType(ProjectionType::Orthographic),
	m_nearPlane(10),
	m_farPlane(1000)
{
}

Camera::Camera(const Vector3& p_position, const Vector3& p_target, float p_fov):
	m_position(p_position),
	m_target(p_target),
	m_up(0, 1, 0),
	m_fov(p_fov),
	m_projectionType(ProjectionType::Perspective),
	m_nearPlane(10),
	m_farPlane(1000)
{
}

Camera::~Camera()
{
}

void Camera::select()
{
	Renderer::getInstance()->setActiveCamera(this);
}

void Camera::deselect()
{
	Renderer::getInstance()->setActiveCamera(0);
}

float Camera::getPixelPerfectDistance() const
{
	return getPixelPerfectDistance(static_cast<float>(gp::SCREEN_HEIGHT), m_fov);
}

float Camera::getPixelPerfectDistance(float p_screenHeight, float p_fov)
{
	return (p_screenHeight * 0.5f) / Math::tangent(p_fov * 0.5f);
}

void Camera::update(float p_timePassed)
{
	updateImpl(p_timePassed);
	syncView();
	syncProjection();
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void Camera::syncView()
{
	// gluLookAt code from Mesa.

	/* Z vector */
	Vector3 z = m_position - m_target;
	z.normalize();
	/* Y vector */
	Vector3 y = m_up;
	/* X vector = Y cross Z */
	Vector3 x = y.crossProduct(z);
	/* Recompute Y = Z cross X */
	y = z.crossProduct(x);

	x.normalize();
	y.normalize();

	m_viewMatrix[0][0] = x.x;
	m_viewMatrix[0][1] = x.y;
	m_viewMatrix[0][2] = x.z;
	m_viewMatrix[0][3] = 0.0;
	m_viewMatrix[1][0] = y.x;
	m_viewMatrix[1][1] = y.y;
	m_viewMatrix[1][2] = y.z;
	m_viewMatrix[1][3] = 0.0;
	m_viewMatrix[2][0] = z.x;
	m_viewMatrix[2][1] = z.y;
	m_viewMatrix[2][2] = z.z;
	m_viewMatrix[2][3] = 0.0;
	m_viewMatrix[3][0] = 0.0;
	m_viewMatrix[3][1] = 0.0;
	m_viewMatrix[3][2] = 0.0;
	m_viewMatrix[3][3] = 1.0;

	float tx = -m_position.x;
	float ty = -m_position.y;
	float tz = -m_position.z;

	m_viewMatrix._m[3] = m_viewMatrix._m[0]  * tx + m_viewMatrix._m[1]  * ty + m_viewMatrix._m[2]  * tz + m_viewMatrix._m[3];
	m_viewMatrix._m[7] = m_viewMatrix._m[4]  * tx + m_viewMatrix._m[5]  * ty + m_viewMatrix._m[6]  * tz + m_viewMatrix._m[7];
	m_viewMatrix._m[11] = m_viewMatrix._m[8]  * tx + m_viewMatrix._m[9]  * ty + m_viewMatrix._m[10] * tz + m_viewMatrix._m[11];
	m_viewMatrix._m[15] = m_viewMatrix._m[12]  * tz + m_viewMatrix._m[13]  * ty + m_viewMatrix._m[14] * tz + m_viewMatrix._m[15];	
}

void Camera::syncProjection()
{
	switch (m_projectionType)
	{
		case ProjectionType::Perspective:
		{
			float radiansFov = m_fov * 0.5f;

			const float deltaZ = m_farPlane - m_nearPlane;
			const float sineFov = Math::sine(radiansFov);
			const float aspect = static_cast<float>(gp::SCREEN_WIDTH) / static_cast<float>(gp::SCREEN_HEIGHT);
			if ((deltaZ == 0) || (sineFov == 0) || (aspect == 0))
			{
				return;
			}
			const float cotangent = Math::cosine(radiansFov) / sineFov;

			m_projectionMatrix[0][0] = cotangent / aspect;
			m_projectionMatrix[0][1] = 0;
			m_projectionMatrix[0][2] = 0;
			m_projectionMatrix[0][3] = 0;
			m_projectionMatrix[1][0] = 0;
			m_projectionMatrix[1][1] = cotangent;
			m_projectionMatrix[1][2] = 0;
			m_projectionMatrix[1][3] = 0;
			m_projectionMatrix[2][0] = 0;
			m_projectionMatrix[2][1] = 0;
			m_projectionMatrix[2][2] = -(m_farPlane + m_nearPlane) / deltaZ;
			m_projectionMatrix[2][3] = -2 * m_nearPlane * m_farPlane / deltaZ;
			m_projectionMatrix[3][0] = 0;
			m_projectionMatrix[3][1] = 0;
			m_projectionMatrix[3][2] = -1;
			m_projectionMatrix[3][3] = 0;

			break;
		}
		case ProjectionType::Orthographic:
		{
			const float farVal = 300.0f;
			const float nearVal = -300.0f;

			const float scaleX = 1 / (static_cast<float>(gp::SCREEN_WIDTH) * 0.5f);
			const float scaleY = 1 / (static_cast<float>(gp::SCREEN_HEIGHT) * 0.5f);
			const float scaleZ = -2 / (farVal - nearVal);

			m_projectionMatrix= Matrix44(scaleX, 0, 0, 0,
										 0, scaleY, 0, 0,
										 0, 0, scaleZ, 0,
										 0, 0, 0, 1);		

			break;
		}
	}
}
