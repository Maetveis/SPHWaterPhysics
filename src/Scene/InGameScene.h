#ifndef IN_GAME_SCENE_H
#define IN_GAME_SCENE_H

#include "Scene.h"
#include "../DataStore/ManagedBuffer.hpp"
#include "../Program/Mesh3DColor.h"
#include "../Model/RigidModel.hpp"
#include "../Model/Mesh/BasicVertexDescriptor.hpp"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

class InGameScene :
	public Scene
{
public:
	InGameScene() :
		vertexBuffer(100000000, GL_STATIC_DRAW),
		indexBuffer (100000000, GL_STATIC_DRAW)
	{
	}

	virtual bool Begin();

	virtual void End();

	virtual void Pause();

	virtual void PrepareRender();
	virtual void Render();

	virtual void Update(double);

	virtual void OnKeyboard(SDL_KeyboardEvent& event);

	virtual void OnWindow(SDL_WindowEvent& event);

	virtual void OnQuit(SDL_QuitEvent& event);

	void Quit();
protected:
private:
	bool LoadData();
	bool LoadShaders();

	GLuint modelID;

	BasicVertexDescriptor desc;

	ManagedBuffer vertexBuffer;
	ManagedBuffer indexBuffer;

	RigidModel car;
	RigidModel cube;

	glm::mat4 matModel;

	Mesh3DColor program;

	/*FrameParams frame;
	LightParams light;*/

	float time;
};

#endif //IN_GAME_SCENE_H
