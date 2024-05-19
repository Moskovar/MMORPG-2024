#pragma once
#include "MapFragment.h"

class Map
{
	public:
		Map(int line, int col);
		~Map();



		void addFragment(int r, int c, string src, SDL_Renderer* renderer) { fragments[r][c] = MapFragment("grass", renderer); }
		void setFragmentVisible(int r, int c, bool state) { fragments[r][c].setVisible(state); }
		void addOffset(float xOffset, float yOffset);
		void updateXOffset(float cameraSpeed) { for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) fragments[i][j].updateXOffset(cameraSpeed); }
		void updateYOffset(float cameraSpeed) { for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) fragments[i][j].updateYOffset(cameraSpeed); }
		void resetPos() { for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) fragments[i][j].resetPos(); }

		void draw(SDL_Renderer* renderer);

	private:
		int rows, cols;
		MapFragment** fragments;
};

