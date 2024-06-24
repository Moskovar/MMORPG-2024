#pragma once
#include <SDL_ttf.h>
#include <string>

using namespace std;

class Font
{
	public:
		Font() {}
		Font(string text, int policeSize, short r, short g, short b, short a, SDL_Renderer* renderer);
		void clear();



		string getText() { return text; }
		SDL_Texture* getTexture() { return texture; }
		int getWidth()  { int width  = 0; TTF_SizeText(font, text.c_str(), &width, NULL);  return width; }
		int getHeight() { int height = 0; TTF_SizeText(font, text.c_str(), &height, NULL); return height; }

		void setText(string text) 
		{ 
			this->text = text;
		}
		void setPos(int x, int y) { pos.x = x; pos.y = y; }
		void updatePos() { pos.x = 1920 / 2 - img->w / 2; pos.y = 1080 / 4; }

		void draw(SDL_Renderer* renderer, int x, int y);
		void draw(SDL_Renderer* renderer) { SDL_RenderCopy(renderer, texture, NULL, &pos); }

	private:
		string text;
		SDL_Rect pos;
		SDL_Color color;

		TTF_Font* font = nullptr;
		SDL_Surface* img = nullptr;
		SDL_Texture* texture = nullptr;

		SDL_Renderer* renderer;
};

