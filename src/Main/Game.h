#include "ScaledDeltaTimer.h"
#include "../Manager/SceneManager.h"
#include "../Manager/WindowManager.h"

class Game
{
public:
	Game();
	void Run();

	bool running;
protected:
private:
	bool Init();
	void HandleEvents();
	void Update();
	void Render();
	void Destroy();
	void DelayFrameTime(const unsigned frameStart, const unsigned short targetFPS);

	SceneManager sceneManager;
	WindowManager windowManager;
	ScaledDeltaTimer timer;
	unsigned short targetFPS = 60;
};
