#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include "lib/csv.h"
#include "tabu.h"

using namespace std;

void readInstance(const char *instance, vector<character> &heroes,
                  vector<character> &villains, map<pair<int, int>, collaboration> &collab,
                  vector<int> &team_villains) {
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
        collab[make_pair(c1, c2)] = collaboration(c1, c2, value);
    }

    // faz a leitura do villains_file e cria o vetor de team_villains
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

void printSolution(vector<int> team_heroes, vector<int> team_villains,
                   int collaboration_lv, int fighting_exp, double budget) {
    unsigned int i;
    int solution = collaboration_lv + fighting_exp;

    if(budget) {
        cout << solution <<" "<< collaboration_lv <<" "<< fighting_exp
             <<" "<< team_heroes[0];
        for (i = 1; i < team_heroes.size(); i++) {
            cout << "," << team_heroes[i];
        }
        cout <<" "<< budget << endl;
    } else {
        cout << solution <<" "<< collaboration_lv <<" "<< fighting_exp << endl;
    }
}

int main(int argc, char **argv) {
    vector<character> heroes, villains;
    map<pair<int, int>, collaboration> collab;
    vector<int> team_heroes, team_villains;
    int has_budget, collaboration_lv, fighting_exp;
    double budget;

    if(argc != 3) {
        cout << argv[0] << " <instancia> <budget=(0,1)>\n";
        return 0;
    }

	 // tem budget ou nao
    has_budget = atoi(argv[2]);
    
    // le a instancia
    readInstance(argv[1], heroes, villains, collab, team_villains);

	 // se tem budget
    if(has_budget) {
    	  // calcula o budget e chama a funcao que encontra a solucao
        budget = calculate_budget(heroes, villains, collab, team_villains);
        team_heroes = solution_with_budget(heroes,villains,collab,team_villains, budget, has_budget);
    } else {
        budget = 0;
        // chama a funcao que encontra a solucao sem budget
        team_heroes = solution_without_budget(heroes,villains,collab,team_villains, has_budget);
    }
	 
	 // calcula os valores da funcao objetivo da solucao encontrada
    collaboration_lv = collaboration_level(team_heroes, collab);
    fighting_exp = fighting_experience(team_heroes, team_villains, collab);
    // imprime a solucao
    printSolution(team_heroes, team_villains, collaboration_lv, fighting_exp, budget);

    return 0;
}


