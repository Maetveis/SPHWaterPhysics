#include "SPHRender.hpp"

#include "../SPHSimulation/SimulationState.hpp"
#include "../Log/Logger.h"

#include <glm/vec3.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

static constexpr const char* EdgeBufferName = "edgeBuffer";
static constexpr const char* DistanceSource = "../shaders/Render/distanceField.comp";
//static constexpr const char* VertexSource = "../shaders/Render/quad.vert";
//static constexpr const char* FragmentSource = "../shaders/Render/distanceField.comp";

static constexpr const unsigned DistanceTextureUnit = 0;

static constexpr const unsigned WorldLocation = 0;
static constexpr const unsigned TextureLocation = 3;
static constexpr const unsigned EyeLocation = 2;

SPHRender::SPHRender(SimulationState& _state) :
	state(_state),
	distanceFieldTexture(GL_TEXTURE_3D)
{
	CompileShaders();

	va.EnableAttrib(0);

	state.AttachEdge(distanceFieldProgram, EdgeBufferName);
	state.AttachEdge(raycastProgram, EdgeBufferName);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, distanceFieldTexture.GetId());

	glTextureStorage3D(distanceFieldTexture.GetId(), 1, GL_R32F, 64, 64, 64);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

static bool CompileProgram(GL::Program& program, const char* source)
{
	GL::Shader shader(GL_COMPUTE_SHADER);
	if(!shader.FromFile(source))
	{
		Logger::Error << "Shader compilation [" << source <<"] failed with message: " << shader.GetInfoLog() << '\n';
		return false;
	}

	program.AttachShader(shader);
	if(!program.Link())
	{
		Logger::Error << "Shader linking failed with message: " << program.GetInfoLog() << '\n';
		return false;
	}

	return true;
}

void SPHRender::CompileShaders()
{
	CompileProgram(distanceFieldProgram, DistanceSource);

	if(!raycastProgram.VsFsProgram("../shaders/Render/quad.vert", "../shaders/Render/raycast.frag"))
	{
		Logger::Error << "Render Program linking failed: " << raycastProgram.GetInfoLog() <<  '\n';
	}
}

void SPHRender::Render(float time)
{
	unsigned edgeCount = state.GetEdgeCount();

	float max = 120.0;
	glClearTexImage(distanceFieldTexture.GetId(), 0, GL_RED, GL_FLOAT, &max);
	glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	distanceFieldProgram.Use();

	glBindImageTexture(DistanceTextureUnit, distanceFieldTexture.GetId(), 0, true, 0, GL_READ_WRITE, GL_R32F);

	glUniform1i(0, DistanceTextureUnit);
	glDispatchCompute(edgeCount / 64 + 1, 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	raycastProgram.Use();
	va.Bind();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, distanceFieldTexture.GetId());

	glBindTextureUnit(1, distanceFieldTexture.GetId());

	//glm::mat4 proj =
	//	glm::perspective(45.0f, 1280/720.0f, 0.01f, 500.0f) *
	//	glm::lookAt(glm::vec3( 0.f,  0.f,  3.f), glm::vec3( 0,  0,  0), glm::vec3( 0,  1,  0)) *
	//	//glm::rotate<float>(t * 0.002, glm::vec3(0.f, 0.f, 1.f)) *
	//	//glm::rotate<float>(t * 0.3, glm::vec3(0.f, 1.f, 0.f)) *
	//	glm::rotate<float>(time * 0.1, glm::vec3(0.f, 1.f, 0.f));

	glm::mat4 world =
		glm::translate(glm::vec3(.5, .5, 1.0)) *
		glm::scale(glm::vec3(.5, .5, .5));

	glUniformMatrix4fv(WorldLocation, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&world[0][0]));

	//texture unit 1
	glUniform1i(3, 1);

	glm::vec3 eye = world * glm::vec4(0.0, 0.0, 2.0, 1.0);
	glUniform3fv(2, 1, reinterpret_cast<GLfloat*>(&eye[0]));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
