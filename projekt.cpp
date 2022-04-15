#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Spawn.hpp"
#include "Rules.hpp"
#include "Wall.hpp"
#include "Region.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include<fstream>
#include<iostream>
#define Height 200
#define Width 200



class Simulation : public olc::PixelGameEngine{
	int board[2][Width][Height];
	int c;
	float t;
	bool paused, step;
	
	int wallColor[3];
	int creatorColor[3];
	int destructorColor[3];
	public :
	Spawn* initial;
	Rules* propagationRules;
	Wall* obstacle;
	Region* fields;
	int populationColor[3];


	public : Simulation(Spawn *beginning, Rules *howToProliferate, Wall* barricade, Region* areas){
		//srand(time(NULL));
		sAppName = "Game of Life";
		t = 0.0f;
		c = 0;
		paused = false;
		step = false;
		populationColor[0] = 57;
		populationColor[1] = 201;
		populationColor[2] = 237;

		initial = beginning;
		propagationRules = howToProliferate;
		obstacle = barricade;
		fields = areas;

		wallColor[0] = obstacle->GetColors()>>16;
		wallColor[1] = (obstacle->GetColors()>>8)&0xff;
		wallColor[2] = (obstacle->GetColors())&0xff;

		creatorColor[0] = fields->GetCreatorColor()>>16;
		creatorColor[1] = (fields->GetCreatorColor()>>8)&0xff;
		creatorColor[2] = (fields->GetCreatorColor())&0xff;

		destructorColor[0] = fields->GetDestructorColor()>>16;
		destructorColor[1] = (fields->GetDestructorColor()>>8)&0xff;
		destructorColor[2] = (fields->GetDestructorColor())&0xff;

		for (int i = 0; i < Width; i++){
			for (int j = 0; j < Height; j++){
				
				if (initial->Get(i, j)==1){
					board[c][i][j] = 1;
				}else{
					board[c][i][j] = 0;
				}
				if(obstacle->Get(i, j)){
					board[c][i][j] = -1;
				}
			}	
		}
		
	}
	public : bool OnUserCreate() override{
		return true;
	}

	public : void SetColor(int a, int b, int c){
		populationColor[0] = a;
		populationColor[1] = b;
		populationColor[2] = c;
	}
	void calculateNextStep(int _c){
		//apply gamer rules, regions, walls
		for (int i = 0; i < Width; i++){
			for (int j = 0; j < Height; j++){
				if (propagationRules->Check(i, j, _c, board)){
					board[1-_c][i][j] = 1;
				}else{
					board[1-_c][i][j] = 0;
				}

				if (fields->Activate(i, j, 1-_c, board)){
					board[1-_c][i][j] = 1;
				}else{
					board[1-_c][i][j] = 0;
				}
				if (obstacle->Get(i, j)){
					board[1-_c][i][j] = -1;
				}
			}	
		}

		return;
	}

	bool OnUserUpdate(float elapedTime){
		t += elapedTime;

		Clear(olc::Pixel(0, 0, 0));



		if (GetKey(olc::Key::SPACE).bPressed){
			paused = not paused;
		}

		if (GetKey(olc::Key::RIGHT).bPressed){
			
			step = true;
		}

		if (GetKey(olc::Key::R).bPressed){
			initial->Restart();
			for (int i = 0; i < Width; i++){
				for (int j = 0; j < Height; j++){
					
					if (initial->Get(i, j)==1){
						board[c][i][j] = 1;
					}else{
						board[c][i][j] = 0;
					}
					if(obstacle->Get(i, j)){
						board[c][i][j] = -1;
					}
				}	
			}
		}

		for (int j = 0; j < Height; j++){
			for (int i = 0; i < Width; i++){
				int r=0, g=0, b=0;

				if(board[c][i][j] == 1){
					r = populationColor[0];
					g = populationColor[1];
					b = populationColor[2];
				}else if (board[c][i][j] == -1){
					r = wallColor[0];
					g = wallColor[1];
					b = wallColor[2];
				}else if(fields->GetType(i, j) == RegionType::Creator){
					r = creatorColor[0];
					g = creatorColor[1];
					b = creatorColor[2];
				}else if(fields->GetType(i, j) == RegionType::Destructor){
					r = destructorColor[0];
					g = destructorColor[1];
					b = destructorColor[2];
				}
				
				Draw(i, j, olc::Pixel(r, g, b));
			}
		}

		if (!paused){
			calculateNextStep(c);

			c = 1 - c;
		}else if(step){
			calculateNextStep(c);
			c = 1 - c;

		}
		step = false;

		return true;
	}

	
};

class FileManager{
	std::string path;
	public : FileManager(std::string filename){
		path = filename;
	}
	public : void SaveToFile(Simulation* toSave){
		std::ofstream file(path);
		if(!file.is_open()){
			std::cout << "An error occurred while saving to file\n";
			exit(1);
		}
		file << toSave->populationColor[0] << " " << toSave->populationColor[1] << " " <<toSave->populationColor[2] << "\n"; 
		toSave->initial->SaveToFile(file);
		toSave->propagationRules->SaveToFile(file);
		toSave->obstacle->SaveToFile(file);
		toSave->fields->SaveToFile(file);
		file.close();
	}
	public : Simulation* ReadFromFile(){
		std::ifstream file(path);
		if(!file.is_open()){
			std::cout << "An error occurred while reading from file\n";
			exit(1);
		}
		int randomness;
		int toPopulate[Width][Height];
		float spawnRatio;
		Simulation* toReturn = NULL;
		Spawn* toSpawn;
		Rules* toRule;
		Wall* toWall;
		Region* toRegion;
		int type, radius;
		int r0, g0, b0;
		file >> r0 >> g0 >> b0;
		//reading Spawner
		file >> randomness;
		if(randomness == 1){
			file >> spawnRatio;
			toSpawn = new Spawn(spawnRatio);
		}else{
			for(int i = 0; i < Width; i++){
				for(int j = 0; j < Height; j++){
					file >> toPopulate[i][j];
				}
			}
			toSpawn = new Spawn(toPopulate);
		}
		//reading Rules
		file >> radius >> type;

		if(!type){
			int a, b, c, d;
			file >> a >> b >> c >> d;
			toRule = new Count(radius,  a, b, c, d);
		}else{
			int  pattern[3][3];
			for(int  i =0; i < 3; i++){
				for(int j =0; j < 3; j++){
					file >> pattern[i][j];
				}
			}
			toRule = new Pattern(pattern);
		}

		//reading Walls
		int toBuild[Width][Height];
		for(int i = 0; i < Width; i++){
			for(int j = 0; j < Height; j++){
				file >> toBuild[i][j];
			}
		}
		int r1, g1, b1;
		file >> r1 >> g1 >> b1;
		toWall = new Wall(toBuild);
		toWall->SetColor(r1, g1, b1);

		//reading Region
		int n;
		RegionType types[200];
		float probabilities[200];
		int regionTypes[Width][Height];
		file >> n;
		int a;
		for(int i = 0; i < n; i++){
			file >> a;
			if(a == 1){
				types[i] = RegionType::Creator;
			}else{
				types[i] = RegionType::Destructor;
			}
		}
		for(int i = 0; i < n; i++){
			file >> probabilities[i];
		}
		for(int i = 0; i < Width; i++){
			for(int j = 0; j < Height; j++){
				file >> regionTypes[i][j];
			}
		}
		toRegion = new Region(n, types, probabilities, regionTypes);
		int r2, r3, g2, g3, b2, b3;
		file >> r2 >> g2 >> b2 >> r3 >> g3 >> b3;
		toRegion->SetCreatorColor(r2, g2, b2);
		toRegion->SetDestructorColor(r3, g3, b3);

		toReturn = new Simulation(toSpawn, toRule, toWall, toRegion);
		toReturn->SetColor(r0, g0, b0);
		file.close();
		return toReturn;
	}
};

class InputReader{
	std::string path;

	public : void ReadInput(){
		std::cout << "Welcome to Game of Life Simulator\n";
		std::cout << "Please specify parameters of this simulation\n";
		std::cout << "Should the cells be spawned randomly? y/n\n";
		std::string in;
		Spawn* readSpawn;
		std::cin >> in;
		if(in=="y"){
			float ratio;
			std::cout << "Please enter spawn ratio (0-1 in float)\n";
			std::cin >> ratio;
			readSpawn = new Spawn(ratio);
		}else{
			std::cout << "Please enter number of cells to be set alive\n";
			int numOfCells;
			std::cin >> numOfCells;
			int readInitialPopulation[Width][Height];
			for(int i = 0; i < Width; i++){
				for(int j = 0; j < Height; j++){
					readInitialPopulation[i][j] = 0;
				}
			}
			std::cout << "Please enter coordinates of cells to be set alive\n";
			int x, y;
			for (int i = 0; i < numOfCells; i++){
				std::cin >> x >> y;
				readInitialPopulation[x][y] = 1;
			}
			readSpawn = new Spawn(readInitialPopulation);
		}
		std::cout << "Please enter type of propagation rules - count or pattern recognition c/p\n";
		std::cin >> in;
		Rules* readRules;
		if(in == "c"){
			std::cout << "Please enter 5 numbers\n- radius of counting, minimal and maximal number of neighbours to stay alive, minimal and maximal number of neighbours to spawn\n";
			int a, b, c, d, e;
			std::cin >> a >> b >> c >> d >> e;
			readRules = new Count(a,  d, e, b, c);
		}else{
			std::cout << "Please enter 8 numbers representing neighbours of cell - (0 - dead, 1 - dead or alive, 2 - alive)\n";
			int readPattern[3][3];
			for(int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++){
					if(i==1 and j ==1){
						readPattern[i][j]= 0;
						continue;
					}
					std::cin>>readPattern[i][j];
				}
			}
			readRules = new Pattern(readPattern);			
		}

		std::cout << "Please enter number of wall points\n";
		int numberOfWalls;
		std::cin >> numberOfWalls;
		std::cout << "Please enter coordinates of wall points\n";
		int readWallPoints[Width][Height];
		for(int i = 0; i < Width; i++){
			for(int  j= 0; j < Height; j++){
				readWallPoints[i][j] = 0;
			}
		}
		for(int  i = 0; i < numberOfWalls; i++){
			int x, y;
			std::cin >> x >> y;
			readWallPoints[x][y] = 1;
		}
		Wall* readWalls = new Wall(readWallPoints);

		std::cout << "Please enter number of regions\n";
		int numberOfRegions;
		std::cin >> numberOfRegions;
		RegionType readTypes[200];
		float readProbabilities[200];
		int readWhere[Width][Height];
		for(int i = 0; i < Width; i++){
			for(int j = 0; j < Height; j++){
				readWhere[i][j] = -1;
			}
		}
		for(int i = 0; i < numberOfRegions; i++){
			std::cout << "Please enter type of region (1 - Creator, 2 - Destructor) and probability (0-1 float)\n";
			int t;
			float p;
			std::cin >> t >> p;
			if(t==1){
				readTypes[i]=RegionType::Creator;
			}else{
				readTypes[i] = RegionType::Destructor;
			}
			readProbabilities[i] = p;
			std::cout << "Please enter number of points of this region\n";
			int numOfRegPoints;
			std::cin>>numOfRegPoints;
			for(int j = 0; j < numOfRegPoints; j++){
				int x, y;
				std::cin >> x >> y;
				readWhere[x][y] = i;
			}
		}
		Region* readRegion = new Region(numberOfRegions, readTypes, readProbabilities, readWhere);
		Simulation* readSim = new Simulation(readSpawn, readRules ,readWalls, readRegion);
		std::cout << "Do you want to customize colours? y/n\n";
		std::cin >> in;
		if(in =="y"){
			std::cout << "Please enter rgb values for population, walls, creating region, destroying region\n";
			int r1, r2, r3, r4, g1, g2, g3, g4, b1, b2, b3, b4;
			std::cin >> r1 >> g1 >> b1 >> r2 >> g2 >> b2 >> r3 >> g3 >> b3 >> r4 >> g4 >> b4;
			readSim->SetColor(r1, g1, b1);
			readWalls->SetColor(r2, g2, b2);
			readRegion->SetCreatorColor(r3, g3, b3);
			readRegion->SetDestructorColor(r4, g4, b4);
		}

		std::cout << "Do you want to save this simulation? y/n\n";
		std::cin >> in;
		if(in == "y"){
			std::cout << "Please specify path\n";
			std::cin >> path;
			FileManager* readFM = new FileManager(path);
			readFM->SaveToFile(readSim);
		}
		if (readSim->Construct(Width, Height, 4, 4))
			readSim->Start();

	}
};

int main(int argc, char *argv[]){
	std::string path;
	FileManager* fm;
	Simulation *sim;
	InputReader ir;

	srand(time(NULL));
	
	if(argc>1){
		path = argv[1];
		fm = new FileManager(path);
		sim = fm->ReadFromFile();
		if (sim->Construct(Width, Height, 4, 4))
			sim->Start();
	}else{
		ir.ReadInput();
	}

	return 0;
}