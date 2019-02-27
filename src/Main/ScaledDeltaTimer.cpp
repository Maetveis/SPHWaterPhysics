#include "ScaledDeltaTimer.h"

#include <SDL2/SDL.h>

void ScaledDeltaTimer::Start()
{
	lastFrameTime = SDL_GetTicks();
}

void ScaledDeltaTimer::Update()
{
	std::uint32_t ticks = SDL_GetTicks();
	unscaledDelta = static_cast<double>(ticks - lastFrameTime) / 1000.;
	scaledDelta = unscaledDelta * timeScale;
	lastFrameTime = ticks;
}
