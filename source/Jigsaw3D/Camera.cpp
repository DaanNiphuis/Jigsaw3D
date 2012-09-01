#include "Camera.h"

#include "GlobalProperties.h"
#include "OpenGL.h"
#include "MathFunctions.h"
#include "Matrix44.h"
#include "Renderer.h"

Camera::Camera() :
	m_target(),
	m_up(0, 1, 0),
	m_fov(Math::HALF_PI),
	m_projectionType(ProjectionType::Perspective),
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
	Renderer::getInstance()->setWorldCamera(this);
}

void Camera::deselect()
{
	Renderer::getInstance()->setWorldCamera(0);
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
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    
    /* Make rotation matrix */
    
    /* Z vector */
    z[0] = m_position.x - m_target.x;
    z[1] = m_position.y - m_target.y;
    z[2] = m_position.z - m_target.z;
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) 
	{          /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    
    /* Y vector */
    y[0] = m_up.x;
    y[1] = m_up.y;
    y[2] = m_up.z;
    
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    
    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
    
	mag = Math::squareRoot(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    
    mag = Math::squareRoot(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }

#define M(row,col)  p_viewMatrix[row][col]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M

	float tx = -m_position.x - 0.1f;
	float ty = -m_position.y - 0.1f;
	float tz = -m_position.z;

	p_viewMatrix._m[3] = p_viewMatrix._m[0]  * tx + 
						  p_viewMatrix._m[1]  * ty + 
						  p_viewMatrix._m[2]  * tz + 
						  p_viewMatrix._m[3];
	p_viewMatrix._m[7] = p_viewMatrix._m[4]  * tx + 
						  p_viewMatrix._m[5]  * ty + 
						  p_viewMatrix._m[6]  * tz + 
						  p_viewMatrix._m[7];
	p_viewMatrix._m[11] = p_viewMatrix._m[8]  * tx + 
						  p_viewMatrix._m[9]  * ty + 
						  p_viewMatrix._m[10] * tz + 
						  p_viewMatrix._m[11];
	p_viewMatrix._m[15] = p_viewMatrix._m[12]  * tz + 
						  p_viewMatrix._m[13]  * ty + 
						  p_viewMatrix._m[14] * tz + 
						  p_viewMatrix._m[15];	
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

			p_projectionMatrix = Matrix44::IDENTITY;
			p_projectionMatrix[0][0] = cotangent / aspect;
			p_projectionMatrix[1][1] = cotangent;
			p_projectionMatrix[2][2] = -(m_farPlane + m_nearPlane) / deltaZ;
			p_projectionMatrix[2][3] = -2 * m_nearPlane * m_farPlane / deltaZ;
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
