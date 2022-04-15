#ifndef INCLUDE_RULES_HPP
#define INCLUDE_RULES_HPP
#define Height 200
#define Width 200
#include<fstream>
#include<iostream>



class Rules{
	protected : int radius;
	public : virtual void SaveToFile(std::ofstream &file){
		return;
	}
	public : virtual bool Check(int x, int y, int c, int board[2][Width][Height]){
		return false;
	}
};

class Count : public Rules{
	int proliferateLow, proliferateHigh;
	int prevailLow, prevailHigh;
	public : Count(int r, int a, int b, int c, int d);
	public : void SaveToFile(std::ofstream &file);
	public : bool Check(int x, int y, int c, int board[2][Width][Height]);

};

class Pattern : public Rules{
	int selectedPattern[3][3];
	public: Pattern(int s[3][3]);
	public : void SaveToFile(std::ofstream &file);
	public : bool Check(int x, int y, int c, int board[2][Width][Height]);

};


#endif