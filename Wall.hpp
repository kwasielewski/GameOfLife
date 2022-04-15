#ifndef INCLUDE_WALL_HPP
#define INCLUDE_WALL_HPP
#define Height 200
#define Width 200
#include<fstream>
#include<iostream>

class Wall{
	int walls[Width][Height];
	int r, g, b;

	public : Wall(int wallSetup[Width][Height]);

	public : void SaveToFile(std::ofstream &file);

	public : void SetColor(int red, int green, int blue);

	public : bool Get(int i, int j);

	public : int GetColors();

};

#endif