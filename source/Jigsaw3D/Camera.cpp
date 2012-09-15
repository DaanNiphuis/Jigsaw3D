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

void Camera::createView(Matrix44& p_viewMatrix) const
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

	p_viewMatrix[0][0] = x.x;
	p_viewMatrix[0][1] = x.y;
	p_viewMatrix[0][2] = x.z;
	p_viewMatrix[0][3] = 0.0;
	p_viewMatrix[1][0] = y.x;
	p_viewMatrix[1][1] = y.y;
	p_viewMatrix[1][2] = y.z;
	p_viewMatrix[1][3] = 0.0;
	p_viewMatrix[2][0] = z.x;
	p_viewMatrix[2][1] = z.y;
	p_viewMatrix[2][2] = z.z;
	p_viewMatrix[2][3] = 0.0;
	p_viewMatrix[3][0] = 0.0;
	p_viewMatrix[3][1] = 0.0;
	p_viewMatrix[3][2] = 0.0;
	p_viewMatrix[3][3] = 1.0;

	float tx = -m_position.x;
	float ty = -m_position.y;
	float tz = -m_position.z;

	p_viewMatrix._m[3] = p_viewMatrix._m[0]  * tx + p_viewMatrix._m[1]  * ty + p_viewMatrix._m[2]  * tz + p_viewMatrix._m[3];
	p_viewMatrix._m[7] = p_viewMatrix._m[4]  * tx + p_viewMatrix._m[5]  * ty + p_viewMatrix._m[6]  * tz + p_viewMatrix._m[7];
	p_viewMatrix._m[11] = p_viewMatrix._m[8]  * tx + p_viewMatrix._m[9]  * ty + p_viewMatrix._m[10] * tz + p_viewMatrix._m[11];
	p_viewMatrix._m[15] = p_viewMatrix._m[12]  * tz + p_viewMatrix._m[13]  * ty + p_viewMatrix._m[14] * tz + p_viewMatrix._m[15];	
}

void Camera::createProjection(Matrix44& p_projectionMatrix) const
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

			p_projectionMatrix[0][0] = cotangent / aspect;
			p_projectionMatrix[0][1] = 0;
			p_projectionMatrix[0][2] = 0;
			p_projectionMatrix[0][3] = 0;
			p_projectionMatrix[1][0] = 0;
			p_projectionMatrix[1][1] = cotangent;
			p_projectionMatrix[1][2] = 0;
			p_projectionMatrix[1][3] = 0;
			p_projectionMatrix[2][0] = 0;
			p_projectionMatrix[2][1] = 0;
			p_projectionMatrix[2][2] = -(m_farPlane + m_nearPlane) / deltaZ;
			p_projectionMatrix[2][3] = -2 * m_nearPlane * m_farPlane / deltaZ;
			p_projectionMatrix[3][0] = 0;
			p_projectionMatrix[3][1] = 0;
			p_projectionMatrix[3][2] = -1;
			p_projectionMatrix[3][3] = 0;

			break;
		}
		case ProjectionType::Orthographic:
		{
			const float farVal = 300.0f;
			const float nearVal = -300.0f;

			const float scaleX = 1 / (static_cast<float>(gp::SCREEN_WIDTH) * 0.5f);
			const float scaleY = 1 / (static_cast<float>(gp::SCREEN_HEIGHT) * 0.5f);
			const float scaleZ = -2 / (farVal - nearVal);

			p_projectionMatrix= Matrix44(scaleX, 0, 0, 0,
										 0, scaleY, 0, 0,
										 0, 0, scaleZ, 0,
										 0, 0, 0, 1);		

			break;
		}
	}
}
