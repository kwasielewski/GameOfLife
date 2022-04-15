#include "Rules.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include<fstream>
#include<iostream>
#define Height 200
#define Width 200

int modulo(int a, int b){
	if(a >= b){
		return a-b;
	}
	if (a < 0){
		return a + b;
	}
	return a;
}

Count::Count(int r, int a, int b, int c, int d){
	radius = r;
	proliferateHigh = b;
	proliferateLow  = a;
	prevailLow = c;
	prevailHigh = d;
}
void Count::SaveToFile(std::ofstream &file){
	file << radius << " ";
	
	file << 0 << "\n"<< proliferateLow << " " << proliferateHigh << " " 
		<< prevailLow << " " << prevailHigh << "\n"; 
}


bool Count::Check(int x, int y, int c, int board[2][Width][Height]){
	int cnt = 0;
	for (int i = -radius; i <= radius; i++){
		for (int j = -radius; j <= radius; j++){
			if (i == 0 and j == 0){
				continue;
				}
				if (board[c][modulo((x+i),Width)][modulo((y+j),Height)]==1){
				cnt++;
			}
		}
	}
	if(cnt <= proliferateHigh and cnt >= proliferateLow){
		return true;
	}
	if (cnt <= prevailHigh and cnt >= prevailLow and board[c][x][y] == 1){
		return true;
	}
		return false;
}





Pattern::Pattern(int s[3][3]){
	radius = 1;
	for(int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			selectedPattern[i][j] = s[i][j];
		}
	}
}

void Pattern::SaveToFile(std::ofstream &file) {
	file << radius << " ";
	file << 1 << "\n";
	for(int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			file << selectedPattern[i][j] << " ";
		}
	}
	file << "\n";
}

bool Pattern::Check(int x, int y, int c, int board[2][Width][Height]){
	int cnt = 0;
	for (int i = -radius; i <= radius; i++){
		for (int j = -radius; j <= radius; j++){
			if (i == 0 and j == 0){
				continue;
				}
			if(board[c][modulo((x+i),Width)][modulo((y+j),Height)]==1){
				cnt++;
			}
			if(board[c][modulo((x+i),Width)][modulo((y+j),Height)]==0 and selectedPattern[i+radius][j+radius] == 2){
				return false;
			}
			if (board[c][modulo((x+i),Width)][modulo((y+j),Height)]==1 and selectedPattern[i+radius][j+radius] == 0){
				return false;
			}
		}
	}
	return (cnt > 0);
}

