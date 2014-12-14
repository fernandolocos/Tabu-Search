// uncomment if error 'undefined reference to `pthread_create'' appear
//#define CSV_IO_NO_THREAD 1;

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include "lib/csv.h"
#include "tabu.h"

using namespace std;

void readInstance(const char *instance, vector<character> &heroes,
	vector<character> &villains, vector<collaboration> &collab,
	vector<int> &team_villains)
{
	int id, intelligence, strength, speed, durability, energy, fighting,
		numberAppeared, c1, c2, value, idVillain;
   string name, type;

	// abre os arquivos de leitura
	ifstream villains_file;

  // le os arquivos csv de entrada
	io::CSVReader<10, io::trim_chars<' ', '\t'>, io::no_quote_escape<';'> > marvel_characters("data/marvel_characters.csv");
	marvel_characters.read_header(io::ignore_extra_column,
		"Character ID",
		"Character Name",
		"Hero or Villain",
		"Intelligence",
		"Strength",
		"Speed",
		"Durability",
		"Energy Projection",
		"Fighting Skills",
		"Number of Comic Books Where Character Appeared");

	io::CSVReader<3, io::trim_chars<' ', '\t'>, io::no_quote_escape<';'> > shared_comic_books("data/shared_comic_books.csv");
	shared_comic_books.read_header(io::ignore_extra_column,
		"Character 1 ID",
		"Character 2 ID",
		"Number of Comic Books Where Character 1 and Character 2 Both Appeared");

	villains_file.open(instance);

	// faz a leitura do marvel_characters e cria os vetores de personagens
	while (marvel_characters.read_row(id, name, type, intelligence, strength, speed, durability,
		energy, fighting, numberAppeared)) {
		if(type == "hero") {
			heroes.push_back(character(id, name, intelligence, strength, speed, 
				durability, energy, fighting, numberAppeared));
		} else {
			villains.push_back(character(id, name, intelligence, strength, speed, 
				durability, energy, fighting, numberAppeared));
		}
	}

	// faz a leitura do shared_comic_books e cria o vetor de colaboracoes
	while (shared_comic_books.read_row(c1, c2, value)) {
		collab.push_back(collaboration(c1, c2, value));
	}

	// faz a leitura do file3 e cria o vetor de team_villains
	string line;
	while (getline(villains_file, line, ' ')) {
		if (!line.empty()) {
			istringstream tmp(line);
			tmp >> idVillain;
			team_villains.push_back(idVillain);
		}
	}

	villains_file.close();
}

void printInstance(vector<character> heroes, vector<character> villains, 
	vector<collaboration> collab, vector<int> team_villains)
{
	unsigned int i;
	cout << "HEROES\n";
	//for (i = 0; i < heroes.size(); i++) {
	for (i = 0; i < heroes.size(); i++) {
		cout << heroes[i].id  << " " << heroes[i].name << " " << 
			heroes[i].intelligence << " " <<
			heroes[i].strength << " " << heroes[i].speed << " " << 
			heroes[i].durability << " " << heroes[i].energy << " " << 
			heroes[i].fighting << " " << heroes[i].numberAppeared << "\n";
	}
	
	cout << "\nVILLAINS\n";
	for (i = 0; i < villains.size(); i++) {
		cout << villains[i].id  << " " << villains[i].name << " " << 
			villains[i].intelligence << " " <<
			villains[i].strength << " " << villains[i].speed << " " << 
			villains[i].durability << " " << villains[i].energy << " " << 
			villains[i].fighting << " " << villains[i].numberAppeared << "\n";
	}
	
	/*cout << "\nCOLLAB\n";
	for (i = 0; i < collab.size(); i++) {
		cout << collab[i].c1  << " " << collab[i].c2 << " " << 
			collab[i].value << "\n";
	}*/
	
	cout << "\nTEAM VILLAINS\n";
	cout << team_villains.size() << endl;
	for (i = 0; i < team_villains.size(); i++) {
		cout << team_villains[i] << "\n";
	}
}

void printSolution(vector<int> team_heroes, vector<int> team_villains, 
	int collaboration_lv, int fighting_exp)
{
	//unsigned int i;
	int solution = collaboration_lv + fighting_exp;
	
	/*cout << "\nTEAM VILLAINS\n";
	for (i = 0; i < team_villains.size(); i++) {
		cout << team_villains[i] << "\n";
	}
	
	cout << "\nTEAM HEROES\n";
	for (i = 0; i < team_heroes.size(); i++) {
		cout << team_heroes[i] << "\n";
	}*/
	
	cout << "  " << solution << "       " << collaboration_lv << "      " 
		<<	fighting_exp << "\n";
}

int main(int argc, char **argv)
{
	vector<character> heroes, villains;
	vector<collaboration> collab;
	vector<int> team_heroes, team_villains;
	int has_budget, collaboration_lv, fighting_exp;
	unsigned int budget;

   if(argc != 3) {
      cout << argv[0] << " <instancia> <budget=(0,1)>\n";
      return 0;
   }
   
   has_budget = atoi(argv[2]);

   readInstance(argv[1], heroes, villains, collab, team_villains);
   //printInstance(heroes, villains, collab, team_villains);


	if(has_budget){
		budget = calculate_budget(heroes, villains, collab, team_villains);
		// fiz isso so pra rodar e nao dar bug
		team_heroes = initial_solution(heroes,villains,collab,team_villains, budget);
	}
	else {
		budget = 0;
		team_heroes = initial_solution(heroes,villains,collab,team_villains, budget);
	}
	
	collaboration_lv = collaboration_level(team_heroes, collab);
	fighting_exp = fighting_experience(team_heroes, team_villains, collab);
	printSolution(team_heroes, team_villains, collaboration_lv, fighting_exp);
	
   return 0;
}


