#include "Wall.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include<fstream>
#include<iostream>
#define Height 200
#define Width 200

Wall::Wall(int wallSetup[Width][Height]){
	r = 255;
	g = 0;
	b = 0;
	for (int i = 0; i < Width; i++){
		for (int j = 0; j < Height; j++){
			walls[i][j] = wallSetup[i][j];
		}
	}
}

void Wall::SaveToFile(std::ofstream &file){
	for (int i = 0; i < Width; i++){
		for (int j = 0; j < Height; j++){
			file << walls[i][j] << " ";
		}
		file << "\n";
	}
	file << r << " " << g << " " << b << "\n";
}

void Wall::SetColor(int red, int green, int blue){
	r = red;
	g = green;
	b = blue;
}

bool Wall::Get(int i, int j){
	return (walls[i][j]==1);
}

int Wall::GetColors(){
	return (r << 16)|(g<<16)|b; 
}