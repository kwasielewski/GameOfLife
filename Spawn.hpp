#ifndef INCLUDE_SPAWN_HPP
#define INCLUDE_SPAWN_HPP
#define Height 200
#define Width 200
#include<fstream>
#include<iostream>

class Spawn{
private:
	int initialCount;
	int population[Width][Height];
	bool randomness;
	float prob;
public : 
	Spawn();
	Spawn(float init);
	Spawn(int toPopulate[Width][Height]);
	void SaveToFile(std::ofstream &file);
	void Restart();
	int Get(int i, int j);
};

#endif
