#ifndef SCENE_H
#define SCENE_H

#include "../Event/EventListener.h"

struct Scene :
	public EventListener
{
	virtual bool Begin(){ return false; }
	virtual void End(){}
	virtual void Pause(){}
	virtual void Update(const double){}

	virtual void PrepareRender(){}
	virtual void Render(){}

	virtual ~Scene(){}
};

#endif //SCENE_H
