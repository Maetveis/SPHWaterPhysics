#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <memory>
#include <stack>

#include "../Main/GameComponent.h"

struct Scene;

class SceneManager :
	public GameComponent
{
public:
	void PushScene(std::unique_ptr<Scene> ptr);
	void PopScene();
	void ChangeScene(std::unique_ptr<Scene> ptr);
	Scene* currentScene() const;
	
	Scene* operator->() const;
	
	~SceneManager();
protected:
private:
	std::stack< std::unique_ptr<Scene> > sceneStack;
};

#endif //SCENE_MANAGER_H