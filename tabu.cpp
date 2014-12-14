#include "tabu.h"
using namespace std;

bool myfunction (collaboration i,collaboration j) {return (i.value>j.value);}

// calcula o powergrid medio de um heroi ou vilao
unsigned int calculate_avg_powergrid_character(vector<character> p, int idP)
{
	unsigned int id, idInc, value = 0;
	
	if(p.size() == HEROES_SIZE) idInc = 1;
	else idInc = HEROES_SIZE + 1;
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
unsigned int calculate_avg_powergrid_team(vector<character> p, vector<int> team)
{
	unsigned int i, id, idInc, value = 0;
	
	if(p.size() == HEROES_SIZE) idInc = 1;
	else idInc = HEROES_SIZE + 1;
	
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
unsigned int calculate_avg_powergrid(vector<character> p)
{
	unsigned int i, value = 0;
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
unsigned int calculate_avg_popularity_team(vector<character> p, vector<int> team)
{
	unsigned int i, id, idInc, value = 0;
	
	if(p.size() == HEROES_SIZE) idInc = 1;
	else idInc = HEROES_SIZE + 1;
	
	for (i = 0; i < team.size(); i++) {
		id = team[i] - idInc;
		value += p[id].numberAppeared;
	}
	value /= team.size();
	return value;
}

// calcula a popularidade media de herois ou viloes
unsigned int calculate_avg_popularity(vector<character> p)
{
	unsigned int i, value = 0;
	for (i = 0; i < p.size(); i++) {
		value += p[i].numberAppeared;
	}
	value /= p.size();
	return value;
}

unsigned int calculate_budget(vector<character> heroes, vector<character> villains,
	vector<collaboration> collab, vector<int> team_villains)
{
	unsigned int i, id, exp1, ratiopg, ratiopop, vtcost = 0, 
		exp2, factor, avgpg, avgpop, vtsize, teamVilAvgPG, vilAvgPG, 
		teamVilAvgPop;
	
	teamVilAvgPG = calculate_avg_powergrid_team(villains, team_villains);
	vilAvgPG = calculate_avg_powergrid(villains);
	teamVilAvgPop = calculate_avg_popularity_team(villains, team_villains);
	avgpg = calculate_avg_powergrid(heroes);
	avgpop = calculate_avg_popularity(heroes);
	vtsize = team_villains.size();
		
	// Exp1
	ratiopg 	= 	avgpg / teamVilAvgPG;
	ratiopop = 	avgpop / teamVilAvgPop;
									
	for (i = 0; i < team_villains.size(); i++) {
		id = team_villains[i] - (HEROES_SIZE+1);
		vtcost	+= calculate_avg_powergrid_character(villains, team_villains[i]) *
						villains[id].numberAppeared;
	}
					
	exp1 = ratiopg * ratiopop * vtcost;
	
	// Exp2
	factor = teamVilAvgPG / vilAvgPG;
	exp2 = factor * avgpg * avgpop * vtsize;
	
	// return budget	
	return max(exp1, exp2);
}

unsigned int calculate_cost(vector<character> heroes, vector<int> team_heroes)
{
	unsigned int i, id, cost = 0, avgPg, avgPop;
	for (i = 0; i < team_heroes.size(); i++) {
		// calcula o powergrid do heroi
		avgPg = calculate_avg_powergrid_character(heroes, team_heroes[i]);
	
		// busca a popularidade do heroi
		id = team_heroes[i] - 1;
		avgPop = heroes[id].numberAppeared;
		
		cost += avgPg * avgPop;
	}
	return cost;
}

int fighting_experience(vector<int> team_heroes, vector<int> team_villains, 
	vector<collaboration> collab)
{
	unsigned int i, j, k;
	int fighting_exp = 0;
	
	//cout << "\nFIGHTING EXPERIENCE\n";
	for (i = 0; i < team_heroes.size(); i++) {
		for (j = 0; j < team_villains.size(); j++) {
			for (k = 0; k < collab.size(); k++) {
				if(collab[k].c1 == team_heroes[i] && collab[k].c2 == team_villains[j]){
					//cout << team_heroes[i] << "->" << team_villains[j] << "=" << collab[k].value << "\n" ;
					fighting_exp += collab[k].value;
					break;
				}
			}
		}
	}
	
	return fighting_exp;
}

int collaboration_level(vector<int> team_heroes, vector<collaboration> collab)
{
	unsigned int i, j, k;
	int collaboration_lv = 0;
	
	//cout << "\nCOLABORACOES HEROIS\n";
	// calcula a colaboracao enter os herois
	for (i = 0; i < team_heroes.size()-1; i++) {
		for (j = i+1; j < team_heroes.size(); j++) {
			for (k = 0; k < collab.size(); k++) {
				if(collab[k].c1 == team_heroes[i] && collab[k].c2 == team_heroes[j]){
					//cout << team_heroes[i] << "->" << team_heroes[j] << "=" << collab[k].value << "\n" ;
					collaboration_lv += collab[k].value;
					break;
				}
			}
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

bool isHero(int idCharacter) {
	if(idCharacter <= HEROES_SIZE)
			return true;
	return false;
}

bool is_viable_solution(vector<character> heroes, vector<character> villains, 
	vector<collaboration> collab, vector<int> team_heroes, vector<int> team_villains, unsigned int budget)
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
		idVillain = team_villains[i] - HEROES_SIZE+1; // ajusta o id
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
unsigned int fighting_experience_max(vector<collaboration> collab, 
	vector<int> team_villains)
{
	int i;
	unsigned int j, k, idHero = 0, fight_exp, fight_exp_max = 0;
	
	for (i = 1; i <= HEROES_SIZE; i++) {
		fight_exp = 0;
		for (j = 0; j < team_villains.size(); j++) {
			for (k = 0; k < collab.size(); k++) {
				if(collab[k].c1 == i && collab[k].c2 == team_villains[j]){
					fight_exp += collab[k].value;
					break;
				}
			}
		}
		// se a fighting_experience desse heroi for maior atualiza
		if(fight_exp_max < fight_exp) {
			fight_exp_max = fight_exp;
			idHero = i;
		}
	}
	return idHero;
}	

/* retorna o id do heroi que melhor maximize a solucao */
unsigned int collab_fighting_max(vector<collaboration> collab, 
	vector<int> team_heroes, vector<int> team_villains, vector<int> tabu_list)
{
	int i;
	unsigned j, k, idHero = 0, value, value_max = 0;
	
	for (i = 1; i <= HEROES_SIZE; i++) {
		// se o heroi nao esta na solucao e nem na lista tabu
		if(!in_vector(team_heroes, i) && !in_vector(tabu_list, i)){
			value = 0;
			// calcula a colaboracao dele com os herois do time
			for (j = 0; j < team_heroes.size(); j++) {
				for (k = 0; k < collab.size(); k++) {
					if(collab[k].c1 == i && collab[k].c2 == team_heroes[j]){
						value += collab[k].value;
						break;
					}
				}
			}
			
			// calcula a fighting_experience desse heroi com os viloes
			for (j = 0; j < team_villains.size(); j++) {
				for (k = 0; k < collab.size(); k++) {
					if(collab[k].c1 == i && collab[k].c2 == team_villains[j]){
						value += collab[k].value;
						break;
					}
				}
			}
			
			// se o valor encontrado para esse heroi for maior, atualiza
			if(value_max < value) {
				value_max = value;
				idHero = i;
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
	int idHero){
	//if(tabu_list.size() == team_villains.size()*3){
	if(tabu_list.size() == TABU_LIST_MAX){
		// retira o primeiro da lista
		tabu_list.erase(tabu_list.begin());	
	}
	tabu_list.push_back(idHero);
}

vector<int> initial_solution(vector<character> heroes, vector<character> villains, 
	vector<collaboration> collab, vector<int> team_villains, unsigned int budget)
{
	unsigned int idHero, idRm = 1, cont = 0, teamMaxSize = team_villains.size();
	vector<int> team_heroes, tabu_list;
	bool is_viavel = false;
	
	// busca heroi com maior fighting_experience contra os viloes inclui na solucao
	idHero = fighting_experience_max(collab, team_villains);
	team_heroes.push_back(idHero);
	
	do {
		if(team_heroes.size() == teamMaxSize && 
			is_viable_solution(heroes,villains,collab,team_heroes,team_villains,budget)){
			is_viavel = true;
		}
		else{
			if(cont == TABU_LIST_MAX){
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
				add_tabu_list(tabu_list, team_villains, idHero);
				
				idRm = 1;
				cont++;
				//cout << "Removeu\n";
				//printTeam(team_heroes);
			}
			
			// busca o heroi que tem maior collaboration_level + fighting_experience
			// e que nao esteja na lista tabu, acrescenta na solucao
			idHero = collab_fighting_max(collab, team_heroes, team_villains, tabu_list);
			team_heroes.push_back(idHero);
			
			//printTeam(team_heroes);
		}
	} while(!is_viavel);
	
	return team_heroes;
}

/* busca de uma boa solucao (mesmo que inviavel) 
partindo da solucao retornada pelo algoritmo de construcao inicial 
verifico se ela eh tem custo > budget se sim, troca
o heroi de maior custo por um heroi de custo mais baixo 
se nao, troca o heroi de menor custo por um heroi de maior custo. 
Faça isso por um numero de iteracoes fixo.
*/

/* busca local por uma solucao viavel
partindo da solucao do algoritmo que encontra uma boa solucao, 
executa uma busca local, ou seja, troca heroi pelo
seus vizinhos em termos de custo, a fim de encontrar a melhor solucao que 
seja viavel
DICA: criar um vetor que contenha o custo de cada heroi
*/
