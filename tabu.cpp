#include "tabu.h"
using namespace std;

// calcula o powergrid medio de um heroi ou vilao
double calculate_avg_powergrid_character(vector<character> heroes, vector<character> p, int idP)
{
	unsigned int id, idInc;
	double value = 0;
	
	if(p.size() == heroes.size()) idInc = 1;
	else idInc = heroes.size() + 1;
	id = idP - idInc;
	
	value += p[id].intelligence;
	value += p[id].strength;
	value += p[id].speed;
	value += p[id].durability;
	value += p[id].energy;
	value += p[id].fighting;
	value /= 6;
	return value;
}

// calcula o powergrid medio de um time de herois ou viloes
double calculate_avg_powergrid_team(vector<character> heroes, vector<character> p, vector<int> team)
{
	unsigned int i, id, idInc;
	double value = 0;
	
	if(p.size() == heroes.size()) idInc = 1;
	else idInc = heroes.size() + 1;
	
	for (i = 0; i < team.size(); i++) {
		id = team[i] - idInc;
		value += p[id].intelligence;
		value += p[id].strength;
		value += p[id].speed;
		value += p[id].durability;
		value += p[id].energy;
		value += p[id].fighting;
	}
	value /= 6;
	value /= team.size();
	return value;
}

// calcula o powergrid medio de herois ou viloes
double calculate_avg_powergrid(vector<character> p)
{
	unsigned int i;
	double value = 0;
	for (i = 0; i < p.size(); i++) {
		value += p[i].intelligence;
		value += p[i].strength;
		value += p[i].speed;
		value += p[i].durability;
		value += p[i].energy;
		value += p[i].fighting;
	}
	value /= 6;
	value /= p.size();
	return value;
}

// calcula  a popularidade media de um time de herois ou viloes
double calculate_avg_popularity_team(vector<character> heroes, vector<character> p, vector<int> team)
{
	unsigned int i, id, idInc;
	double value = 0;
	
	if(p.size() == heroes.size()) idInc = 1;
	else idInc = heroes.size() + 1;
	
	for (i = 0; i < team.size(); i++) {
		id = team[i] - idInc;
		value += p[id].numberAppeared;
	}
	value /= team.size();
	return value;
}

// calcula a popularidade media de herois ou viloes
double calculate_avg_popularity(vector<character> p)
{
	unsigned int i;
	double value = 0;
	for (i = 0; i < p.size(); i++) {
		value += p[i].numberAppeared;
	}
	value /= p.size();
	return value;
}

double calculate_budget(vector<character> heroes, vector<character> villains,
	map<pair<int, int>, collaboration> collab, vector<int> team_villains)
{
	unsigned int i;
	int id; 
	double exp1, ratiopg, ratiopop, vtcost = 0, exp2, factor, avgpg, 
		avgpop, vtsize, teamVilAvgPG, vilAvgPG, teamVilAvgPop;
	
	teamVilAvgPG = calculate_avg_powergrid_team(heroes, villains, team_villains);
	vilAvgPG = calculate_avg_powergrid(villains);
	teamVilAvgPop = calculate_avg_popularity_team(heroes, villains, team_villains);
	avgpg = calculate_avg_powergrid(heroes);
	avgpop = calculate_avg_popularity(heroes);
	vtsize = team_villains.size();

	// Exp1
	ratiopg 	= 	avgpg / teamVilAvgPG;
	ratiopop = 	avgpop / teamVilAvgPop;

	for (i = 0; i < team_villains.size(); i++) {
		id = team_villains[i] - (heroes.size()+1);
		vtcost	+= calculate_avg_powergrid_character(heroes, villains, team_villains[i]) *
						villains[id].numberAppeared;
	}

	exp1 = ratiopg * ratiopop * vtcost;

	// Exp2
	factor = teamVilAvgPG / vilAvgPG;
	exp2 = factor * avgpg * avgpop * vtsize;

	// return budget
	return max(exp1, exp2);
}

// calcula o custo do time de herois
double calculate_cost(vector<character> heroes, vector<int> team_heroes)
{
	unsigned int i;
	double cost = 0, avgPg, avgPop;
	for (i = 0; i < team_heroes.size(); i++) {
		// calcula o powergrid do heroi
		avgPg = calculate_avg_powergrid_character(heroes, heroes, team_heroes[i]);
	
		// busca a popularidade do heroi
		avgPop = heroes[team_heroes[i]-1].numberAppeared;
		
		cost += avgPg * avgPop;
	}
	return cost;
}

// calcula o custo de um herois
double calculate_cost(vector<character> heroes, int idHero)
{
	double pg, pop;
	pg = calculate_avg_powergrid_character(heroes, heroes, idHero);
	pop = heroes[idHero-1].numberAppeared;
	return pg * pop;
}

int fighting_experience(vector<int> team_heroes, vector<int> team_villains, 
	map<pair<int, int>, collaboration> collab)
{
	unsigned int i, j;
	int fighting_exp = 0;
	
	//cout << "\nFIGHTING EXPERIENCE\n";
	for (i = 0; i < team_heroes.size(); i++) {
		for (j = 0; j < team_villains.size(); j++) {
			fighting_exp += collab[make_pair(team_heroes[i],team_villains[j])].value;
		}
	}
	
	return fighting_exp;
}

int collaboration_level(vector<int> team_heroes, map<pair<int, int>, collaboration> collab)
{
	unsigned int i, j;
	int collaboration_lv = 0;
	
	//cout << "\nCOLABORACOES HEROIS\n";
	// calcula a colaboracao entre os herois
	for (i = 0; i < team_heroes.size()-1; i++) {
		for (j = i+1; j < team_heroes.size(); j++) {
			//cout << team_heroes[i] << "->" << team_heroes[j] << "=" << collab[k].value << "\n" ;
			collaboration_lv += collab[make_pair(team_heroes[i], team_heroes[j])].value;
		}
	}	
	
	return collaboration_lv;
}

// verifica se existe este id em um vetor de int
bool in_vector(vector<int> v, int id) {
	unsigned int i;
	for (i = 0; i < v.size(); i++) {
		if(v[i] == id)
			return true;
	}
	return false;
}

bool is_viable_solution(vector<character> heroes, vector<character> villains, 
	map<pair<int, int>, collaboration> collab, vector<int> team_heroes, vector<int> team_villains, double budget)
{
	unsigned int i, idHero, idVillain, teamMaxSize = team_villains.size();
	vector<int> pgHeroes (6);
	vector<int> pgVillains (6);
	
	// verifica se o time de herois tem tamanho <= ao time de viloes
	if(team_heroes.size() > teamMaxSize) {
		return false;
	}
	
	// calcula o powergrid de cada time por habilidade
	for (i = 0; i < 6; i++) {
		pgHeroes[i] = 0;
		pgVillains[i] = 0;
	}
	
	for (i = 0; i < team_heroes.size(); i++) {
		idHero = team_heroes[i] - 1; // ajusta o id
		pgHeroes[0] += heroes[idHero].intelligence;
		pgHeroes[1] += heroes[idHero].strength;
		pgHeroes[2] += heroes[idHero].speed;
		pgHeroes[3] += heroes[idHero].durability;
		pgHeroes[4] += heroes[idHero].energy;
		pgHeroes[5] += heroes[idHero].fighting;
	}
	
	for (i = 0; i < team_villains.size(); i++) {
		idVillain = team_villains[i] - heroes.size()+1; // ajusta o id
		pgVillains[0] += villains[idVillain].intelligence;
		pgVillains[1] += villains[idVillain].strength;
		pgVillains[2] += villains[idVillain].speed;
		pgVillains[3] += villains[idVillain].durability;
		pgVillains[4] += villains[idVillain].energy;
		pgVillains[5] += villains[idVillain].fighting;
	}
	
	// verifica a media do powergrid do time de herois se eh maior ou igual 
	// a media do powergrid do time de viloes para cada habilidade
	for (i = 0; i < 6; i++) {
		if(pgHeroes[i] < pgVillains[i]) return false;
	}
	
	if (budget > 0) {
		if(calculate_cost(heroes, team_heroes) > budget) return false;
	}
	
	return true;
}

/* retorna o id do heroi com maior
 fighting_experience contra os viloes de um determinado time */
int fighting_experience_max(vector<character> heroes, map<pair<int, int>, collaboration> collab, 
	vector<int> team_villains)
{
	unsigned int i, j, idHero = 0, fight_exp, fight_exp_max = 0;

	for (i = 0; i < heroes.size(); i++) {
		fight_exp = 0;
		for (j = 0; j < team_villains.size(); j++) {
			fight_exp += collab[make_pair(heroes[i].id, team_villains[j])].value;
		}

		// se a fighting_experience desse heroi for maior atualiza
		if(fight_exp_max < fight_exp) {
			fight_exp_max = fight_exp;
			idHero = heroes[i].id;
		}
	}
	return idHero;
}

/* retorna o id do heroi que melhor maximize a solucao */
int collab_fighting_max(vector<character> heroes, map<pair<int, int>, collaboration> collab, 
	vector<int> team_heroes, vector<int> team_villains, vector<int> tabu_list)
{
	unsigned i, j, idHero = 0, value, value_max = 0;
	
	for (i = 0; i < heroes.size(); i++) {
		// se o heroi nao esta na solucao e nem na lista tabu
		if(!in_vector(team_heroes, heroes[i].id) && !in_vector(tabu_list, heroes[i].id)){
			value = 0;
			// calcula a colaboracao dele com os herois do time
			for (j = 0; j < team_heroes.size(); j++) {
				value += collab[make_pair(heroes[i].id, team_heroes[j])].value;
			}
			
			// calcula a fighting_experience desse heroi com os viloes
			for (j = 0; j < team_villains.size(); j++) {
				value += collab[make_pair(heroes[i].id, team_villains[j])].value;
			}
			
			// se o valor encontrado para esse heroi for maior, atualiza
			if(value_max < value) {
				value_max = value;
				idHero = heroes[i].id;
			}
		}
	}

	return idHero;
}

void printTeam(vector<int> team){
	unsigned int i;
	cout << "Team\n";
	for (i = 0; i < team.size(); i++) {
		cout << team[i] << " ";
	}
	cout << "\n";
}

// adiciona um heroi na lista tabu, FIFO
void add_tabu_list(vector<int> &tabu_list, vector<int> team_villains, 
	int idHero, int hasbudget){
	if(hasbudget && tabu_list.size() == TABU_LIST_MAX_2){
		// retira o primeiro da lista
		tabu_list.erase(tabu_list.begin());	
	}
	else if(!hasbudget && tabu_list.size() == TABU_LIST_MAX_1) {
		// retira o primeiro da lista
		tabu_list.erase(tabu_list.begin());	
	}
	
	tabu_list.push_back(idHero);
}

vector<int> solution_without_budget(vector<character> heroes, vector<character> villains, 
	map<pair<int, int>, collaboration> collab, vector<int> team_villains, int hasbudget)
{
	unsigned int idHero, idRm = 1, cont = 0, teamMaxSize = team_villains.size();
	vector<int> team_heroes, tabu_list;
	bool is_viavel = false;
	
	// busca heroi com maior fighting_experience contra os viloes inclui na solucao
	idHero = fighting_experience_max(heroes, collab, team_villains);
	team_heroes.push_back(idHero);
	
	do {
		if(team_heroes.size() == teamMaxSize && 
			is_viable_solution(heroes,villains,collab,team_heroes,team_villains,hasbudget)){
			is_viavel = true;
		}
		else{ // cria um contador para que a solucao permute entre outros candidatos
			if(cont == TABU_LIST_MAX_1){
				idRm++;
				cont = 0;
			}
		
			// se o numero de herois superou o numero de viloes
			if(team_heroes.size() >= teamMaxSize){
				// retire um heroi e acrescenta na lista tabu
				/*idRand = rand() % team_heroes.size();
				idHero = team_heroes[idRand];
				team_heroes.erase(team_heroes.begin()+idRand);*/
				
				idHero = team_heroes[team_heroes.size()-idRm];
				team_heroes.erase(team_heroes.end()-idRm);	
				add_tabu_list(tabu_list, team_villains, idHero, hasbudget);
				
				idRm = 1;
				cont++;
				//cout << "Removeu\n";
				//printTeam(team_heroes);
			}
			
			// busca o heroi que tem maior collaboration_level + fighting_experience
			// e que nao esteja na lista tabu, acrescenta na solucao
			idHero = collab_fighting_max(heroes, collab, team_heroes, team_villains, tabu_list);
			team_heroes.push_back(idHero);
			
			//printTeam(team_heroes);
		}
	} while(!is_viavel);
	
	return team_heroes;
}

vector<int> solution_with_budget(vector<character> heroes, vector<character> villains, 
	map<pair<int, int>, collaboration> collab, vector<int> team_villains, double budget, int hasbudget)
{
	unsigned int i, teamMaxSize = team_villains.size();
	int idHero = 0, idRm, avgBudget, cost, maxCost, minCost;
	vector<int> team_heroes, tabu_list;
	bool is_viavel = false;
	vector<Costs> heroCosts;
	
	// cria um vetor com os custos de todos os herois
	for(i = 0; i < heroes.size(); i++){
		heroCosts.push_back(Costs(heroes[i].id, calculate_cost(heroes, heroes[i].id)));
	}
	
	// ordena o vetor descrescente pelo custo
	sort(heroCosts.begin(), heroCosts.end(), greater<Costs>());

	// calcula a media de budgets do time de viloes
	budget = calculate_budget(heroes, villains, collab, team_villains);
	avgBudget = budget /	(int)teamMaxSize;
	
	// Busque no vetor o heroi com maior custo que respeite a restrição: 
	// custo <= budget médio
	for(i = 0; i < heroCosts.size(); i++){
		if(heroCosts[i].cost <= avgBudget){
			idHero = heroCosts[i].id;
			break;
		}
	}
	
	// inclua este heroi na solucao
	team_heroes.push_back(idHero);
	
	do {
		// se a solucao eh viavel retorna o team heroes
		if(team_heroes.size() == teamMaxSize && 
			is_viable_solution(heroes,villains,collab,team_heroes,team_villains,budget)){
			is_viavel = true;
		}
		else{	
			// se o numero de herois superou o numero de viloes
			if(team_heroes.size() >= teamMaxSize){
				// se o custo do time eh maior que o budget retira o heroi de
				// maior custo, caso contrario tiro o de menor custo
				maxCost = 0;
				minCost = INT_MAX;
				idRm = 0;
				//cout << "cost=" << calculate_cost(heroes, team_heroes)  << " budget=" << budget << endl;
				if(calculate_cost(heroes, team_heroes) > budget){
					for(i = 0; i < team_heroes.size(); i++){
						cost = calculate_cost(heroes, team_heroes[i]);
						//cout << "idHero=" << team_heroes[i]  << " costHeroi=" << cost << endl;
						if(maxCost < cost){
							maxCost = cost;
							idRm = i;
						}
					}
				}
				else {
					for(i = 0; i < team_heroes.size(); i++){
						cost = calculate_cost(heroes, team_heroes[i]);
						if(minCost > cost){
							minCost = cost;
							idRm = i;
						}
					}
				}
				
				idHero = team_heroes[idRm];
				team_heroes.erase(team_heroes.begin()+idRm);	
				add_tabu_list(tabu_list, team_villains, idHero, hasbudget);
				
				//cout << "Removeu\n";
				//printTeam(team_heroes);
			}
			
			// busca o heroi que tem maior collaboration_level + fighting_experience
			// e que nao esteja na lista tabu, acrescenta na solucao
			idHero = collab_fighting_max(heroes, collab, team_heroes, team_villains, tabu_list);
			team_heroes.push_back(idHero);
			
			//printTeam(team_heroes);
		}
	} while(!is_viavel);	
					
	return team_heroes;
}
