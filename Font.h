#pragma once
#include <SDL_ttf.h>
#include <string>

using namespace std;

class Font
{
	public:
		Font() {}
		Font(string text, int size, SDL_Window* window, SDL_Renderer* renderer);
		void clear();



		string getText() { return text; }
		SDL_Texture* getTexture() { return texture; }
		int getWidth()  { int width  = 0; TTF_SizeText(font, text.c_str(), &width, NULL);  return width; }
		int getHeight() { int height = 0; TTF_SizeText(font, text.c_str(), &height, NULL); return height; }

		void draw(SDL_Renderer* renderer, int x, int y);

	private:
		string text;
		SDL_Rect pos;

		TTF_Font* font = nullptr;
		SDL_Surface* img = nullptr;
		SDL_Texture* texture = nullptr;

};

