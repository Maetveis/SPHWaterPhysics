#include "OrbiterCamera.hpp"

#include <algorithm>
#include <cmath>

OrbiterCamera::OrbiterCamera(
	const glm::vec3& _origo,
	float _horizontalAngle,
	float _verticalAngle,
	float _dist,
	float _eyeDist,
	float _VerticalAngleLimit
) :
	origo(_origo),
	dist(_dist),
	eyeDist(_eyeDist),
	horizontalAngle(_horizontalAngle),
	verticalAngle(_verticalAngle),
	horizontalDirection(Direction::Stop),
	verticalDirection(Direction::Stop),
	updated(false),
	VerticalAngleLimit(_VerticalAngleLimit),
	state(origo, horizontalAngle, verticalAngle, dist, eyeDist)
{
}

void OrbiterCamera::Update(float delta)
{
	if(updated)
	{
		switch(horizontalDirection)
		{
			case Direction::Forward:
				horizontalAngle -= 0.1 * delta;
				break;
			case Direction::Backward:
				horizontalAngle += 0.1 * delta;
			default:
				break;
		}
		horizontalAngle = std::fmod(horizontalAngle, 6.28f);

		switch(verticalDirection)
		{
			case Direction::Forward:
				verticalAngle -= 0.1 * delta;
				break;
			case Direction::Backward:
				verticalAngle += 0.1 * delta;
			default:
				break;
		}
		verticalAngle = std::clamp(verticalAngle, -VerticalAngleLimit, VerticalAngleLimit);

		state.Update(origo, horizontalAngle, verticalAngle, dist, eyeDist);

		updated = horizontalDirection != Direction::Stop || verticalDirection != Direction::Stop;
	}
}
