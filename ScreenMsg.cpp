#include "ScreenMsg.h"

ScreenMsg::ScreenMsg(string text, int policeSize, SDL_Renderer* renderer)
{
	msg = Font(text, policeSize, 255, 50, 50, 255, renderer);
	updatePos();
}
