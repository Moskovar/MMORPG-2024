#pragma once
#include "MapFragment.h"

class Map
{
	public:
		Map(int line, int col, SDL_Renderer* renderer);
		~Map();

		vector<Element*> getElements(bool withFragments = false);

		void addFragment(int r, int c, string src, vector<Element*> v_elements, SDL_Renderer* renderer);
		void addFragment(int r, int c, MapFragment* mf) { fragments[r][c] = mf; }
		void setFragmentVisible(int r, int c, bool state) { fragments[r][c]->setVisible(state); }
		void addOffset(int r, int c, float xOffset, float yOffset);
		void addOffset(float xOffset, float yOffset);
		void updateXOffset(float cameraSpeed) { for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) fragments[i][j]->updateXOffset(cameraSpeed); }
		void updateYOffset(float cameraSpeed) { for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) fragments[i][j]->updateYOffset(cameraSpeed); }
		void resetPos() { for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) { fragments[i][j]->resetPos(); } }

		void draw(SDL_Renderer* renderer); 

	private:
		int rows, cols;
		MapFragment*** fragments;
};

