#ifndef ORBITER_CAMERA_HPP
#define ORBITER_CAMERA_HPP

#include "OrbiterCameraState.hpp"
#include "Direction.hpp"

#include <glm/vec3.hpp>

class OrbiterCamera
{
private:
	glm::vec3 origo;
	float dist;
	float eyeDist;

	float horizontalAngle;
	float verticalAngle;

	Direction horizontalDirection;
	Direction verticalDirection;

	bool updated;

	const float VerticalAngleLimit;

	OrbiterCameraState state;
public:
	OrbiterCamera(
		const glm::vec3& _origo,
		float _horizontalAngle = 0,
		float _verticalAngle = 0,
		float _dist = 1.5f,
		float _eyeDist = 2.0f,
		float _VerticalAngleLimit = 1.3f
	);

	const glm::mat4& GetView() const
	{
		return state.GetView();
	}

	const glm::vec3& GetEye() const
	{
		return state.GetEye();
	}

	void SetHorizontalDirection(Direction dir)
	{
		updated = true;
		horizontalDirection = dir;
	}

	void SetVerticalDirection(Direction dir)
	{
		updated = true;
		verticalDirection = dir;
	}

	void Update(float delta);
};

#endif
