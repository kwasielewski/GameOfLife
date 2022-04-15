#ifndef INCLUDE_REGION_HPP
#define INCLUDE_REGION_HPP
#define Height 200
#define Width 200
#include<fstream>
#include<iostream>

enum class RegionType {Normal=0, Creator, Destructor};

class Region{
	int number;
	RegionType type[200];
	float probability[200];
	int where[Width][Height];
	int cr, cg, cb, dr, dg, db;
	public : Region (int n, int span[][4], RegionType t[], float chance[]);

	public : Region(int n, RegionType t[], float chance[], int typesOfRegion[Width][Height]);

	public : void SaveToFile(std::ofstream &file);
	
	public : bool Activate (int i, int j, int c, int board[2][Width][Height]);
	
	public : RegionType GetType(int i, int j);

	public : void SetCreatorColor(int a, int b, int c);
	public : void SetDestructorColor(int a, int b, int c);
	public : int GetCreatorColor();
	public : int GetDestructorColor();

};

#endif