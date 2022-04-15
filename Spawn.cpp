#include "Spawn.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include<fstream>
#include<iostream>
#define Height 200
#define Width 200

Spawn::Spawn(){
	initialCount = 0;
	randomness = true;
	prob = 0.5f;
	for (int i = 0; i < Width; i++){
		for (int j = 0; j < Height; j++){
			
			if (rand()/(float)RAND_MAX < prob){
				population[i][j] = 1;
				initialCount++;
			}else{
				population[i][j] = 0;
			}
		}	
	}
}


Spawn::Spawn(float init){
	srand(time(NULL));
	initialCount = 0;
	randomness = true;
	prob = init;
	for (int i = 0; i < Width; i++){
		for (int j = 0; j < Height; j++){
			
			if (rand()/(float)RAND_MAX < init){
				population[i][j] = 1;
				initialCount++;
			}else{
				population[i][j] = 0;
			}
		}	
	}
}

Spawn::Spawn(int toPopulate[Width][Height]){

	randomness = false;
	for (int i = 0; i < Width; i++){
		for (int j = 0; j < Height; j++){
			
			if (toPopulate[i][j]==1){
				population[i][j] = 1;
				initialCount++;
			}else{
				population[i][j] = 0;
			}
		}	
	}
}

void Spawn::Restart(){
	if (!randomness){
		return;
	}
	initialCount = 0;
	for (int i = 0; i < Width; i++){
		for (int j = 0; j < Height; j++){
			
			if (rand()/(float)RAND_MAX < prob){
				population[i][j] = 1;
				initialCount++;
			}else{
				population[i][j] = 0;
			}
		}	
	}
}

void Spawn::SaveToFile(std::ofstream &file){
	if (randomness){
		file << 1;
	}else{
		file << 0;
	}
	file << "\n";
	if(!randomness){
		for (int i = 0; i < Width; i++){
			for (int j = 0; j < Height; j++){
				file << population[i][j] << " ";
			}
			file << "\n";
		}	
	}else{
		file << prob << "\n";
	}
}

int Spawn::Get(int i, int j){
	return population[i][j];
}
