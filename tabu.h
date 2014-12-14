#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string>

#define TABU_LIST_MAX_1 10
#define TABU_LIST_MAX_2 150

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
              int speed, int durability, int energy, int fighting, int numberAppeared) {
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

    collaboration() {
        this->c1 = this->c2 = this->value = 0;
    };

    collaboration(int c1, int c2, int value) {
        this->c1 = c1;
        this->c2 = c2;
        this->value = value;
    };
};

// cria uma estrutura que armazena custos e ids
typedef struct Costs {
    int id;
    int cost;

    Costs(int id, int cost) {
        this->id = id;
        this->cost = cost;
    }

    // comparator for costs
    bool operator<(const Costs& rhs) const {
        return this->cost < rhs.cost;
    };
    bool operator>(const Costs& rhs) const {
        return this->cost > rhs.cost;
    };
    bool operator==(const Costs& rhs) const {
        return this->cost == rhs.cost;
    };
} Costs;

void readInstance(const char *instance, vector<character> &heroes,
                  vector<character> &villains, map<pair<int, int>, collaboration> &collab,
                  vector<int> &team_villains);

vector<int> solution_without_budget(vector<character> heroes, vector<character> villains,
                                    map<pair<int, int>, collaboration> collab, vector<int> team_villains, int hasbudget);

vector<int> solution_with_budget(vector<character> heroes, vector<character> villains,
                                 map<pair<int, int>, collaboration> collab, vector<int> team_villains, double budget, int hasbudget);

int collaboration_level(vector<int> team_heroes, map<pair<int, int>, collaboration> collab);

int fighting_experience(vector<int> team_heroes, vector<int> team_villains,
                        map<pair<int, int>, collaboration> collab);

double calculate_budget(vector<character> heroes, vector<character> villains,
                        map<pair<int, int>, collaboration> collab, vector<int> team_villains);


