#include "Camera.h"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Camera::Camera(vec3 e, vec3 t, vec3 u)
{
	LookAt(e, t, u);
}

Camera::Camera(glm::vec3 e, float U, float V) :
	eye(e),
	u(U),
	v(V)
{
	UpdateView();
}

void Camera::LookAt(vec3 e, vec3 t, vec3 u)
{
	eye = e;
	vec3 vz = normalize(e - t);
	vec3 vx = normalize(cross(u, vz));
	vec3 vy = cross(vz, vx);
	
	view = mat4(
		vec4(vx.x, vy.x, vz.x, 0),
		vec4(vx.y, vy.y, vz.y, 0),
		vec4(vx.z, vy.z, vz.z, 0),
		vec4(-dot(vx, eye), -dot(vy, eye), -dot(vz, eye), 1)
	);
}

const vec3& Camera::GetEye() const
{
	return eye;
}

vec3&& Camera::GetForward() const
{
	return std::move(vec3(row(view, 2)));
}

vec3&& Camera::GetStrafe() const
{
	return std::move(vec3(row(view, 0)));
}

vec3&& Camera::GetUp() const
{
	return std::move(vec3(row(view, 1)));
}

float Camera::GetU() const
{
	return u;
}

float Camera::GetV() const
{
	return v;
}

const mat4& Camera::GetView() const
{
	return view;
}

/*mat4 Camera::GetViewInv() const
{
	return mat4(
		vec4(vx, 0),
		vec4(vy, 0),
		vec4(vz, 0),
		vec4(eye, 1)
	);
}*/

void Camera::SetEye(const vec3& e)
{
	eye = e;
}

void Camera::SetUV(float U, float V)
{
	u = U;
	v = V;
}
	
void Camera::UpdateView()
{
	mat4 I(1);
	view = glm::translate(I, eye)  * glm::rotate(I, v, vec3(0, 1, 0)) * glm::rotate(I, u, vec3(1, 0, 0));
	view = inverse(view);
}