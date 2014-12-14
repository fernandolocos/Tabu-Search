#include <iostream>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define HEROES_SIZE 381
#define VILLAINS_SIZE 386
#define TABU_LIST_MAX 10

using namespace std;

struct character {
    int id;
    std::string name;
    int intelligence;
    int strength;
    int speed;
    int durability;
    int energy;
    int fighting;
    int numberAppeared;
    
    character(int id, std::string name, int intelligence, int strength,
    	int speed, int durability, int energy, int fighting, int numberAppeared){
			this->id = id;
			this->name = name;
			this->intelligence = intelligence;
			this->strength = strength;
			this->speed = speed;
			this->durability = durability;
			this->energy = energy;
			this->fighting = fighting;
			this->numberAppeared = numberAppeared;
    };
};

struct collaboration {
    int c1;
    int c2;
    int value;
    
    collaboration(int c1, int c2, int value){
        this->c1 = c1;
        this->c2 = c2;
        this->value = value;
    };
};

void readInstance(const char *instance, vector<character> &heroes, 
	vector<character> &villains, vector<collaboration> &collab, 
	vector<int> &team_villains);

vector<int> initial_solution(vector<character> heroes, vector<character> villains, 
	vector<collaboration> collab, vector<int> team_villains, unsigned int budget);
	
int collaboration_level(vector<int> team_heroes, vector<collaboration> collab);

int fighting_experience(vector<int> team_heroes, vector<int> team_villains, 
	vector<collaboration> collab);

unsigned int calculate_budget(vector<character> heroes, vector<character> villains,
    vector<collaboration> collab, vector<int> team_villains);




