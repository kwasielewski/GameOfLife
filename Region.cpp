#include "Region.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include<fstream>
#include<iostream>
#define Height 200
#define Width 200



Region::Region (int n, int span[][4], RegionType t[], float chance[]){
	number = n;
	for(int i = 0; i < Width; i++){
		for(int j = 0; j < Height; j++){
			where[i][j] = -1;
		}
	}
	for (int i = 0; i < n; i++){
		type[i] = t[i];
		probability[i] = chance[i];
		for (int  j = span[i][0]; j <= span[i][2]; j++){
			for (int k = span[i][1]; k <= span[i][3]; k++){
				where[j][k] = i;
			}
		}
	}
	cr = 0;
	cg = 175;
	cb = 0;
	dr = 150;
	dg = 0;
	db = 0;
}
Region::Region(int n, RegionType t[], float chance[], int typesOfRegion[Width][Height]){
	number = n;
	for (int i = 0; i < n; i++){
		type[i] = t[i];
		probability[i] = chance[i];
	}
	for(int i = 0; i < Width; i++){
		for(int j = 0; j < Height; j++){
			where[i][j] = typesOfRegion[i][j];
		}
	}
	cr = 0;
	cg = 175;
	cb = 0;
	dr = 150;
	dg = 0;
	db = 0;
}
void Region::SaveToFile(std::ofstream &file){
	file << number << "\n";
	for (int i = 0; i < number; i++){
		if(type[i] == RegionType::Creator){
			file << 1 << " ";
		}else{
			file << 2 << " ";
		}
	}
	file << "\n";
	for(int i = 0; i< number; i++){
		file << probability[i] << " ";
	} 
	file << "\n";
	for(int i = 0; i < Width; i++){
		for(int j = 0; j < Height; j++){
			file << where[i][j] << " ";
		}
		file << "\n";
	}
	file << cr << " " << cg << " " << cb << "\n";
	file << dr << " " << dg << " " << db << "\n";
}

bool Region::Activate (int i, int j, int c, int board[2][Width][Height]){
	if(where[i][j] == -1){
		return (board[c][i][j]==1);
	}
	if(type[where[i][j]]==RegionType::Creator){
		if ((rand()/(float)RAND_MAX) < probability[where[i][j]]){
			return true;
		}else{
			return (board[c][i][j]==1);
		}
	}else if (type[where[i][j]] ==RegionType::Destructor){
		if ((rand()/(float)RAND_MAX) < probability[where[i][j]]){
			return false;
		}else{
			
			return (board[c][i][j]==1);
		}
	}
		return (board[c][i][j]==1);
}
RegionType Region::GetType(int i, int j){
	if(where[i][j] == -1){
		return RegionType::Normal;
	}else{
		return type[where[i][j]];
	}
}

void Region::SetCreatorColor(int a, int b, int c){
	cr = a;
	cg = b;
	cb = c;
}
void Region::SetDestructorColor(int a, int b, int c){
	dr = a;
	dg = b;
	db = c;
}
int Region::GetCreatorColor(){
	return (cr<<16)|(cg<<8)|cb;
}
int Region::GetDestructorColor(){
	return (dr<<16)|(dg<<8)|db;
}

