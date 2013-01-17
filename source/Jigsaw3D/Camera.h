#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix44.h"
#include "Vector3.h"

class Matrix44;

class Camera
{
public:
	struct ProjectionType 
	{
		enum Enum
		{
			Perspective,
			Orthographic
		};
	};

	Camera();
	Camera(const Vector3& p_position, const Vector3& p_target, float p_fov);
	virtual ~Camera();

	void select();
	void deselect();
	
	inline Vector3	getPosition()	const {return m_position;}
	inline void translate	(const Vector3& p_translation)		{m_position += p_translation;}
	inline void setPosition	(const Vector3& p_position)			{m_position = p_position;}
	inline void setX		(float p_x)							{m_position.x = p_x;}
	inline void setY		(float p_y)							{m_position.y = p_y;}
	inline void setZ		(float p_z)							{m_position.z = p_z;}

	inline void setTarget(const Vector3& p_target) { m_target = p_target; }
	inline float getFov() const {return m_fov;}
	inline void setFov(float p_fov) { m_fov = p_fov; }
	inline void setProjectionType(ProjectionType::Enum p_projectionType) {m_projectionType = p_projectionType;}

	inline float getNearPlane() const {return m_nearPlane;}
	inline float getFarPlane() const {return m_farPlane;}
	inline void setNearPlane(float p_nearPlane) {m_nearPlane = p_nearPlane;}
	inline void setFarPlane(float p_farPlane) {m_farPlane = p_farPlane;}

	inline const Matrix44& getViewMatrix() const {return m_viewMatrix;}
	inline const Matrix44& getProjectionMatrix() const {return m_projectionMatrix;}
	inline const Matrix44& getViewProjectionMatrix() const {return m_viewProjectionMatrix;}
	
	float getPixelPerfectDistance() const;
	static float getPixelPerfectDistance(float p_screenHeight, float p_fov);

	void update(float /*p_timePassed*/);
	virtual void draw() const {}

private:
	virtual void updateImpl(float /*p_timePassed*/) {};

	void syncView();
	void syncProjection();

	Vector3 m_position;
	Vector3 m_target;
	Vector3 m_up;
	float m_fov;
	ProjectionType::Enum m_projectionType;
	float m_nearPlane;
	float m_farPlane;
	
	Matrix44 m_viewMatrix;
	Matrix44 m_projectionMatrix;
	Matrix44 m_viewProjectionMatrix;
};

#endif
