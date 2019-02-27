#include "SceneManager.h"
#include "../Scene/Scene.h"

void SceneManager::PushScene(std::unique_ptr<Scene> ptr)
{
	if(!sceneStack.empty())
	{
		sceneStack.top()->Pause();
	}

	sceneStack.push(std::move(ptr));
	sceneStack.top()->AttachGame(game);
	sceneStack.top()->Begin();
}

void SceneManager::PopScene()
{
	sceneStack.top()->End();
	sceneStack.pop();

	sceneStack.top()->AttachGame(game);
	sceneStack.top()->Begin();
}

bool SceneManager::ChangeScene(std::unique_ptr<Scene> ptr)
{
	if(!sceneStack.empty())
	{
		sceneStack.top()->End();
		sceneStack.pop();
	}

	sceneStack.push(std::move(ptr));
	sceneStack.top()->AttachGame(game);
	return sceneStack.top()->Begin();
}

Scene* SceneManager::currentScene() const
{
	return sceneStack.top().get();
}

Scene* SceneManager::operator->() const
{
	return currentScene();
}

SceneManager::~SceneManager()
{
	while(!sceneStack.empty())
	{
		sceneStack.top()->End();
		sceneStack.pop();
	}
}
