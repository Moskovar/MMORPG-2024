#include "MapFragment.h"
#include "uti.h"

MapFragment::MapFragment(string src, SDL_Renderer* renderer, vector<Element*> v_elements)
{
	img   = IMG_Load(string("img/mapfragments/" + src + ".png").c_str());
	imgBg = IMG_Load(string("img/mapfragments/" + src + "_bg.png").c_str());
	//uti::loadTexture(text, img, renderer, "MapFragment -> " + src);
	if (img) { text = SDL_CreateTextureFromSurface(renderer, img); }
	else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: mapfragment").c_str()); exit(0); }

	this->v_elements = v_elements;

	pos.w = 1920;
	pos.h = 1080;

	isSolid = true;
}

MapFragment::~MapFragment()
{
	for (Element* e : v_elements)
	{
		delete e;
		e = nullptr;
	}

	SDL_FreeSurface(img);
	SDL_FreeSurface(imgBg);
	SDL_DestroyTexture(text);

	printf("MapFragment is cleared now !\n");
}

bool MapFragment::check_collisions(int x, int y)
{
	if (!isSolid) return false;

	if (x < this->pos.x || x > this->pos.x + imgBg->w - 1 || y < this->pos.y || y > this->pos.y + imgBg->h - 1) return false;// check table de vérité ?

	int count = 0;
	Uint32 pixel_color = uti::get_pixel(imgBg, x - pos.x, y - pos.y); // - pos.x / - pos.y pour ???
	Uint8 red, green, blue, alpha;
	SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
	if (red == 255 && green == 0 && blue == 0) count++;

	pixel_color = uti::get_pixel(imgBg, x - pos.x + 10, y - pos.y); // - pos.x / - pos.y pour ???
	SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
	if (red == 255 && green == 0 && blue == 0) count++;

	pixel_color = uti::get_pixel(imgBg, x - pos.x - 10, y - pos.y); // - pos.x / - pos.y pour ???
	SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
	if (red == 255 && green == 0 && blue == 0) count++;

	pixel_color = uti::get_pixel(imgBg, x - pos.x, y - pos.y + 10); // - pos.x / - pos.y pour ???
	SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
	if (red == 255 && green == 0 && blue == 0) count++;

	pixel_color = uti::get_pixel(imgBg, x - pos.x, y - pos.y - 10); // - pos.x / - pos.y pour ???
	SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
	if (red == 255 && green == 0 && blue == 0) count++;

	return (count > 3) ? true : false;
}

void MapFragment::resetPos()
{
	x += xOffset;
	pos.x = x;
	y += yOffset;
	pos.y = y;
	xOffset = 0;
	yOffset = 0;

	for (Element* e : v_elements) e->resetPos();
}
