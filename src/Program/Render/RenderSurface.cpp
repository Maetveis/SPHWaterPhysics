#include "RenderSurface.hpp"

#include "../../SPHSimulation/SimulationState.hpp"
#include "../../Log/Logger.h"

#include <glm/vec3.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

static constexpr const char* EdgeBufferName = "edgeBuffer";
static constexpr const char* DistanceSource = "../shaders/Render/distanceField.comp";
static constexpr const char* VertexSource = "../shaders/Render/quad.vert";
static constexpr const char* FragmentSource = "../shaders/Render/raycast.frag";

static constexpr const unsigned DistanceTextureUnit = 0;

static constexpr const unsigned TextureLocation = 0;
static constexpr const unsigned EyeLocation = 1;
static constexpr const unsigned WorldLocation = 2;

RenderSurface::RenderSurface(SimulationState& _state) :
	state(_state),
	camera(glm::vec3(0.5, 0.5, 0.5))
{
	CompileShaders();

	state.AttachEdge(distanceFieldProgram, EdgeBufferName);
	state.AttachEdge(raycastProgram, EdgeBufferName);

	SetDistanceTextureSize(64);
}

static bool CompileProgram(GL::Program& program, const char* source)
{
	GL::Shader shader(GL_COMPUTE_SHADER);
	if(!shader.FromFile(source))
	{
		Logger::Error() << "Shader compilation [" << source <<"] failed with message: " << shader.GetInfoLog() << '\n';
		return false;
	}

	program.AttachShader(shader);
	if(!program.Link())
	{
		Logger::Error() << "Shader linking failed with message: " << program.GetInfoLog() << '\n';
		return false;
	}

	return true;
}

void RenderSurface::CompileShaders()
{
	CompileProgram(distanceFieldProgram, DistanceSource);

	if(!raycastProgram.VsFsProgram(VertexSource, FragmentSource))
	{
		Logger::Error() << "Render Program linking failed: " << raycastProgram.GetInfoLog() <<  '\n';
	}
}

void RenderSurface::DistanceField()
{
	unsigned edgeCount = state.GetEdgeCount();

	float max = 1.0;
	glClearTexImage(distanceFieldTexture->GetId(), 0, GL_RED, GL_FLOAT, &max);
	glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	distanceFieldProgram.Use();

	glUniform1i(TextureLocation, DistanceTextureUnit);
	glDispatchCompute(edgeCount / 64 + 1, 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void RenderSurface::Raycast()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	raycastProgram.Use();
	va.Bind();

	glUniformMatrix4fv(WorldLocation, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&camera.GetView()[0][0]));

	//texture unit 1
	glUniform1i(TextureLocation, DistanceTextureUnit);

	//glm::vec3 eye = world * glm::vec4(0.0, 0.0, 2.0, 1.0);
	glUniform3fv(EyeLocation, 1, reinterpret_cast<const GLfloat*>(&camera.GetEye()[0]));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RenderSurface::Update(float delta)
{
	camera.Update(delta);
}

void RenderSurface::UpdateParticles()
{
	DistanceField();
}

void RenderSurface::Render()
{
	Raycast();
}

void RenderSurface::SetDistanceTextureSize(unsigned length)
{
	distanceFieldTexture = std::make_unique<GL::Texture>(GL_TEXTURE_3D);

	glTextureStorage3D(distanceFieldTexture->GetId(), 1, GL_R32F, length, length, length);
	glTextureParameteri(distanceFieldTexture->GetId(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(distanceFieldTexture->GetId(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(distanceFieldTexture->GetId(), GL_TEXTURE_WRAP_S,  GL_MIRRORED_REPEAT);
	glTextureParameteri(distanceFieldTexture->GetId(), GL_TEXTURE_WRAP_T,  GL_MIRRORED_REPEAT);
	glTextureParameteri(distanceFieldTexture->GetId(), GL_TEXTURE_WRAP_R,  GL_MIRRORED_REPEAT);

	glBindImageTexture(DistanceTextureUnit, distanceFieldTexture->GetId(), 0, true, 0, GL_READ_WRITE, GL_R32F);
	glBindTextureUnit(DistanceTextureUnit, distanceFieldTexture->GetId());
}
