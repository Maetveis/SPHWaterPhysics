#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

class Game;

class GameComponent
{
public:
	void AttachGame(Game* g)
	{
		game = g;
	}
protected:
	Game* game;
private:
};

#endif //GAME_COMPONENT_H