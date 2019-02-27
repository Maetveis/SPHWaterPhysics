#ifndef Event_Listener_H
#define Event_Listener_H

#include "../Main/GameComponent.h"

#include <SDL2/SDL.h>

class EventListener :
	public GameComponent
{
public:
	virtual void OnAudioDevice(SDL_AudioDeviceEvent&){}
	
	virtual void OnControllerAxis(SDL_ControllerAxisEvent&){}
	
	virtual void OnControllerButton(SDL_ControllerButtonEvent&){}
	
	virtual void OnControllerDevice(SDL_ControllerDeviceEvent&){}
	
	virtual void OnDrop(SDL_DropEvent&){}
	
	virtual void OnTouchFinger(SDL_TouchFingerEvent&){}
	
	virtual void OnKeyboard(SDL_KeyboardEvent&){}
	
	virtual void OnJoyAxis(SDL_JoyAxisEvent&){}
	
	virtual void OnJoyBall(SDL_JoyBallEvent&){}

	virtual void OnJoyHat(SDL_JoyHatEvent&){}
	
	virtual void OnJoyButton(SDL_JoyButtonEvent&){}
	
	virtual void OnJoyDevice(SDL_JoyDeviceEvent&){}
	
	virtual void OnMouseMotion(SDL_MouseMotionEvent&){}
	
	virtual void OnMouseButton(SDL_MouseButtonEvent&){}

	virtual void OnMouseWheel(SDL_MouseWheelEvent&){}
	
	virtual void OnQuit(SDL_QuitEvent&){}
	
 	// virtual void OnTextInput(SDL_TextInputEvent&){}
	
	virtual void OnUser(SDL_UserEvent&){}
	
	virtual void OnWindow(SDL_WindowEvent&){}
	
protected:
private:
};

#endif //Event_Listener_H