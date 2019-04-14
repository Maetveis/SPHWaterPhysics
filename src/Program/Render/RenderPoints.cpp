#include "RenderPoints.hpp"

#include "../../SPHSimulation/SimulationState.hpp"
#include "../../Log/Logger.h"

#include <glm/vec3.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

static constexpr const char* VertexSource = "../shaders/Render/passthrough.vert";
static constexpr const char* FragmentSource = "../shaders/Render/passthrough.frag";

static constexpr const char* PositionBufferName = "positionBuffer";
static constexpr const char* VelocityBufferName = "velocityBuffer";
static constexpr const char* PressureBufferName = "pressureBuffer";
static constexpr const char* ForceBufferName = "forceBuffer";

static constexpr const unsigned ProjLocation = 0;

RenderPoints::RenderPoints(SimulationState& _state) :
	state(_state)
{
	CompileShaders();

	state.AttachPressure(renderProgram, PressureBufferName);
	state.AttachForce(renderProgram, ForceBufferName);
}

void RenderPoints::CompileShaders()
{
	if(!renderProgram.VsFsProgram(VertexSource, FragmentSource))
	{
		Logger::Error << "Render Program linking failed: " << renderProgram.GetInfoLog() <<  '\n';
	}
}

void RenderPoints::Render(float time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderProgram.Use();
	va.Bind();

	state.AttachPosition(renderProgram, PositionBufferName);
	state.AttachVelocity(renderProgram, VelocityBufferName);

	glm::mat4 proj =
		glm::perspective(45.0f, 1280/720.0f, 0.01f, 500.0f) *
		glm::lookAt(glm::vec3( 0.f,  0.f,  3.f), glm::vec3( 0,  0,  0), glm::vec3( 0,  1,  0)) *
		//glm::rotate<float>(t * 0.002, glm::vec3(0.f, 0.f, 1.f)) *
		//glm::rotate<float>(t * 0.3, glm::vec3(0.f, 1.f, 0.f)) *
		glm::rotate<float>(time * 0.1, glm::vec3(0.f, 1.f, 0.f));

	glUniformMatrix4fv(ProjLocation, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&proj[0][0]));

	glDrawArrays(GL_POINTS, 0, state.ResX() * state.ResY() * state.ResZ());
}
