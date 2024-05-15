#pragma once
#include <SDL_ttf.h>
#include <string>

using namespace std;

class Font
{
	public:
		Font() { font = nullptr; img = nullptr; texture = nullptr; }
		Font(SDL_Window* window, SDL_Renderer* renderer, string text);

		void clear();

		string getText() { return text; }
		SDL_Texture* getTexture() { return texture; }
		int getWidth() { int width = 0; TTF_SizeText(font, text.c_str(), &width, NULL); return width; }

	private:
		string text;

		TTF_Font* font = nullptr;
		SDL_Surface* img = nullptr;
		SDL_Texture* texture = nullptr;

};

