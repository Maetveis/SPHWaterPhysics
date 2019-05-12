#ifndef ORBITER_CAMERA_STATE_HPP
#define ORBITER_CAMERA_STATE_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class OrbiterCameraState
{
private:
	glm::vec3 eye;
	glm::mat4 viewMatrix;
public:
	OrbiterCameraState(
		const glm::vec3& origo,
		float horizontalAngle = 0,
		float verticalAngle = 0,
		float dist = 1.5,
		float eyeDist = 1
	)
	{
		Update(origo, horizontalAngle, verticalAngle, dist, eyeDist);
	}

	OrbiterCameraState(const OrbiterCameraState&) = default;
	OrbiterCameraState(OrbiterCameraState&&) = default;

	OrbiterCameraState& operator= (const OrbiterCameraState&) = default;
	OrbiterCameraState& operator= (OrbiterCameraState&&) = default;

	const glm::mat4& GetView() const
	{
		return viewMatrix;
	}

	const glm::vec3& GetEye() const
	{
		return eye;
	}

	void Update(
		const glm::vec3& origo,
		float horizontalAngle = 0,
		float verticalAngle = 0,
		float dist = 1.5,
		float eyeDist = 1
	)
	{
		viewMatrix =
			glm::translate(origo) *
			glm::rotate(verticalAngle, glm::vec3(1, 0, 0)) *
			glm::rotate(horizontalAngle, glm::vec3(0, 1, 0)) *
			glm::translate(-1.0f * origo) *
			glm::translate(glm::vec3(.5, .5, dist)) *
			glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));

		eye = viewMatrix * glm::vec4(0.0, 0.0, eyeDist, 1.0);
	}
};

#endif
