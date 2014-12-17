#include "tabu.h"
using namespace std;

// calcula o powergrid medio de um heroi ou vilao
double calculate_avg_powergrid_character(vector<character> heroes, vector<character> p, int idP) {
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
double calculate_avg_powergrid_team(vector<character> heroes, vector<character> p, vector<int> team) {
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
double calculate_avg_powergrid(vector<character> p) {
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
double calculate_avg_popularity_team(vector<character> heroes, vector<character> p, vector<int> team) {
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
double calculate_avg_popularity(vector<character> p) {
    unsigned int i;
    double value = 0;
    for (i = 0; i < p.size(); i++) {
        value += p[i].numberAppeared;
    }
    value /= p.size();
    return value;
}

// calcula o budget
double calculate_budget(vector<character> heroes, vector<character> villains,
                        map<pair<int, int>, collaboration> collab, vector<int> team_villains) {
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
    ratiopg = avgpg / teamVilAvgPG;
    ratiopop = avgpop / teamVilAvgPop;

    for (i = 0; i < team_villains.size(); i++) {
        id = team_villains[i] - (heroes.size()+1);
        vtcost += calculate_avg_powergrid_character(heroes, villains, team_villains[i]) *
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
double calculate_cost(vector<character> heroes, vector<int> team_heroes) {
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
double calculate_cost(vector<character> heroes, int idHero) {
    double pg, pop;
    pg = calculate_avg_powergrid_character(heroes, heroes, idHero);
    pop = heroes[idHero-1].numberAppeared;
    return pg * pop;
}

int fighting_experience(vector<int> team_heroes, vector<int> team_villains,
                        map<pair<int, int>, collaboration> collab) {
    unsigned int i, j;
    int fighting_exp = 0;

    for (i = 0; i < team_heroes.size(); i++) {
        for (j = 0; j < team_villains.size(); j++) {
            fighting_exp += collab[make_pair(team_heroes[i],team_villains[j])].value;
        }
    }
    return fighting_exp;
}

int collaboration_level(vector<int> team_heroes, map<pair<int, int>, collaboration> collab) {
    unsigned int i, j;
    int collaboration_lv = 0;

    // calcula a colaboracao entre os herois
    for (i = 0; i < team_heroes.size()-1; i++) {
        for (j = i+1; j < team_heroes.size(); j++) {
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

// verifica se e uma solucao viavel
bool is_viable_solution(vector<character> heroes, vector<character> villains,
	map<pair<int, int>, collaboration> collab, vector<int> team_heroes, 
	vector<int> team_villains, double budget) 
{
    unsigned int i, j, id, teamMaxSize = team_villains.size();
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
        id = team_heroes[i]-1; // ajusta o id
        pgHeroes[0] += heroes[id].intelligence;
        pgHeroes[1] += heroes[id].strength;
        pgHeroes[2] += heroes[id].speed;
        pgHeroes[3] += heroes[id].durability;
        pgHeroes[4] += heroes[id].energy;
        pgHeroes[5] += heroes[id].fighting;
    }

    for (i = 0; i < team_villains.size(); i++) {
        id = team_villains[i] - heroes.size()-1; // ajusta o id
        pgVillains[0] += villains[id].intelligence;
        pgVillains[1] += villains[id].strength;
        pgVillains[2] += villains[id].speed;
        pgVillains[3] += villains[id].durability;
        pgVillains[4] += villains[id].energy;
        pgVillains[5] += villains[id].fighting;
    }

    // verifica a media do powergrid do time de herois se eh maior ou igual
    // a media do powergrid do time de viloes para cada habilidade
    for (i = 0; i < 6; i++) {
       	if(pgHeroes[i] < pgVillains[i]) return false;
    }
	
	 // se respeita o budget
    if (budget > 0) {
        if(calculate_cost(heroes, team_heroes) > budget) return false;
 	 }
 	 
    // se nao tem heroi repitido
    for (i = 0; i < team_heroes.size()-1; i++) {
    	for (j = i+1; j < team_heroes.size(); j++) {
    		if(team_heroes[i] == team_heroes[j]) return false;
    	}
    }
    return true;
}

/* retorna o id do heroi que melhor maximize a solucao */
int collab_fighting_max(vector<character> heroes, map<pair<int, int>, collaboration> collab,
                        vector<int> team_heroes, vector<int> team_villains, vector<int> tabu_list) {
    unsigned i, j, idHero = 1, value, value_max = 0;

    for (i = 0; i < heroes.size(); i++) {
        // se o heroi nao esta na solucao e nem na lista tabu
        if(!in_vector(team_heroes, heroes[i].id) && !in_vector(tabu_list, heroes[i].id)) {
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

int collab_fighting_max_respect_cost(vector<character> heroes, map<pair<int, int>, 
	collaboration> collab, vector<int> team_heroes, vector<int> team_villains, 
	vector<int> tabu_list, vector<Costs> heroCosts, double cost_team) 
{
    unsigned i, j, idHero = 0, value, value_max = 0;
	
    for (i = 0; i < heroCosts.size(); i++) {
        // se o heroi nao esta na solucao, nem na lista tabu e respeita o budget residual
        if(!in_vector(team_heroes, heroCosts[i].id) && !in_vector(tabu_list, heroCosts[i].id)
        		&& heroCosts[i].cost <= cost_team) {
            value = 0;
            // calcula a colaboracao dele com os herois do time
            for (j = 0; j < team_heroes.size(); j++) {
                value += collab[make_pair(heroCosts[i].id, team_heroes[j])].value;
            }

            // calcula a fighting_experience desse heroi com os viloes
            for (j = 0; j < team_villains.size(); j++) {
                value += collab[make_pair(heroCosts[i].id, team_villains[j])].value;
            }

            // se o valor encontrado para esse heroi for maior, atualiza
            if(value_max < value) {
                value_max = value;
                idHero = heroCosts[i].id;
            }
        }
    }
    return idHero;
}

// calcula o id do heroi no time que tem menor powergrid no time de herois
int powergrid_team_heroes_min(vector<character> heroes,  map<pair<int, int>, 
	collaboration> collab, vector<int> team_heroes, vector<int> team_villains)
{
	unsigned i, j, id = 0, value, value_min = INT_MAX, pg_weig;
	
	if(team_villains.size() <= 8) pg_weig = PG_WEIGHT_1;
	else  pg_weig = PG_WEIGHT_2;
	
   for (i = 0; i < team_heroes.size(); i++) {
   	value = 0;
		// calcula o powergrid do heroi - (sistema de pontos - da peso ao valor de powergrid)
		value = calculate_avg_powergrid_character(heroes, heroes, team_heroes[i]) * pg_weig;

		// calcula a colaboracao dele com os herois do time
      for (j = 0; j < team_heroes.size(); j++) {
          value += collab[make_pair(heroes[i].id, team_heroes[j])].value;
      }

      // calcula a fighting_experience desse heroi com os viloes
      for (j = 0; j < team_villains.size(); j++) {
          value += collab[make_pair(heroes[i].id, team_villains[j])].value;
      }

		// se o valor encontrado para esse heroi for menor, atualiza
		if(value_min > value) {
		 	value_min = value;
		 	id = i;
		}
	}
	return id;
}

// calcula o id do heroi no time que tem maior custo no time de herois
int cost_max_team_heroes(vector<character> heroes, vector<int> team_heroes)
{
	unsigned i, id = 0;
	double cost, maxCost = 0;
	
	for (i = 0; i < team_heroes.size(); i++) {
		cost = calculate_cost(heroes, team_heroes[i]);
      if(maxCost < cost) {
          maxCost = cost;
          id = i;
      }
    }
    return id;
}

int powergrid_collab_max(vector<character> heroes, vector<int> tabu_list, map<pair<int, int>, 
	collaboration> collab, vector<int> team_heroes, vector<int> team_villains)
{
	unsigned i, j, idHero = 1, value, value_max = 0, pg_weig;
	
	// verifica qual o peso a ser considerado no powergrid
	if(team_villains.size() <= 8) pg_weig = PG_WEIGHT_1;
	else  pg_weig = PG_WEIGHT_2;
	
   for (i = 0; i < heroes.size(); i++) {
   	value = 0;
		// se o heroi nao esta na solucao e nem na lista tabu
		if(!in_vector(team_heroes, heroes[i].id) && !in_vector(tabu_list, heroes[i].id)) {
			// calcula o powergrid do heroi (sistema de pontos - da peso ao valor de powergrid)
			value = calculate_avg_powergrid_character(heroes, heroes, heroes[i].id) * pg_weig;
			
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

// adiciona um heroi na lista tabu, FIFO
void add_tabu_list(vector<int> &tabu_list, vector<int> team_villains,
                   int idHero, int hasbudget) {
    if(hasbudget && tabu_list.size() == TABU_LIST_MAX_2) {
        // retira o primeiro da lista
        tabu_list.erase(tabu_list.begin());
    } else if(!hasbudget && tabu_list.size() == TABU_LIST_MAX_1) {
        // retira o primeiro da lista
        tabu_list.erase(tabu_list.begin());
    }

    tabu_list.push_back(idHero);
}

// algoritmo guloso - encontra uma solucao gulosa, mesmo que nao seja viavel
vector<int> initial_solution(vector<character> heroes, map<pair<int, int>,
	 collaboration> collab, vector<int> team_villains, vector<int> tabu_list) {
    vector<int> team_heroes;
    unsigned int idHero, teamMaxSize = team_villains.size();

    do {
        idHero = collab_fighting_max(heroes, collab, team_heroes, team_villains, tabu_list);
        team_heroes.push_back(idHero);
    } while(team_heroes.size() < teamMaxSize);

    return team_heroes;
}

// busca local - algoritmo que encontra a solucao sem budget
vector<int> solution_without_budget(vector<character> heroes, vector<character> villains,
	map<pair<int, int>, collaboration> collab, vector<int> team_villains, int hasbudget) 
{
    unsigned int i, j, idHero, idRm = 1, teamMaxSize = team_villains.size();
    int solution, best_solution = 0, collaboration_lv, fighting_exp;
    vector<int> team_heroes, best_team (teamMaxSize), tabu_list;
    bool is_viavel = false;

    // inicia com time encontrado por algoritmo guloso, nao necessariamente viavel
    team_heroes = initial_solution(heroes, collab, team_villains, tabu_list);
    
    // loop externo que faz a busca local
    for(i = 0; i < ITERATIONS_1; i++) {
        do {
            if(team_heroes.size() == teamMaxSize &&
                    is_viable_solution(heroes,villains,collab,team_heroes,team_villains,hasbudget)) {
                is_viavel = true;
            } else {
                // se o numero de herois superou o numero de viloes
                if(team_heroes.size() >= teamMaxSize) {
                    // retire um heroi e acrescenta na lista tabu
                    // adicionando um pouco de randomico para sair de buscas locais repetitivas
                    idRm = rand() % team_villains.size() + 1;
                    idHero = team_heroes[team_heroes.size()-idRm];
                    team_heroes.erase(team_heroes.end()-idRm);
                    add_tabu_list(tabu_list, team_villains, idHero, hasbudget);
                }

                // busca o heroi que tem maior collaboration_level + fighting_experience
                // e que nao esteja na lista tabu, acrescenta na solucao
                idHero = collab_fighting_max(heroes, collab, team_heroes, team_villains, tabu_list);
                team_heroes.push_back(idHero);
            }
        } while(!is_viavel);

        // calcula o valor da solucao
        collaboration_lv = collaboration_level(team_heroes, collab);
        fighting_exp = fighting_experience(team_heroes, team_villains, collab);
        solution = collaboration_lv + fighting_exp;

        // se for a primeira iteracao
        if(i == 0) {
            copy(team_heroes.begin(), team_heroes.end(), best_team.begin());
            best_solution = solution;
        } else {
            if(best_solution < solution) {
                copy(team_heroes.begin(), team_heroes.end(), best_team.begin());
                best_solution = solution;
            }
        }

        // insere a solucao encontrada na lista tabu
        for(j = 0; j < team_heroes.size(); j++) {
            add_tabu_list(tabu_list, team_villains, team_heroes[j], hasbudget);
        }

        // inicializa as variaveis
        team_heroes.clear();
        is_viavel = false;
    }

    return best_team;
}

// algoritmo guloso - encontra uma solucao gulosa, mesmo que nao seja viavel
vector<int> initial_solution_with_budget(vector<character> heroes, map<pair<int, int>, 
	collaboration> collab, vector<int> team_villains, vector<int> tabu_list, 
	vector<Costs> heroCosts, double budget)
{
	vector<int> team_heroes;
   unsigned int idHero, teamMaxSize = team_villains.size();
   double cost_team = budget;

   do {
		if(calculate_cost(heroes, team_heroes) > budget){
			// calcula o id do heroi de custo maximo que respeita o budget medio
			idHero = collab_fighting_max_respect_cost(heroes, collab, team_heroes, 
						team_villains, tabu_list, heroCosts, cost_team);
			// se nao encontrar heroi, inclui um heroi aleatorio
			if(idHero == 0){	
					idHero = collab_fighting_max(heroes, collab, team_heroes, team_villains, tabu_list);
			}
		}
		else {
			// busca o id do heroi que maximizima melhor o powergrid
			idHero = powergrid_collab_max(heroes, tabu_list, collab, team_heroes, team_villains);
		}
		cost_team -= heroCosts[idHero-1].cost;
      team_heroes.push_back(idHero);
	} while(team_heroes.size() < teamMaxSize);
   return team_heroes;
}

// busca local - algoritmo que encontra a solucao com budget
vector<int> solution_with_budget(vector<character> heroes, vector<character> villains,
	map<pair<int, int>, collaboration> collab, vector<int> team_villains, double budget, int hasbudget) 
{
	unsigned int i, j, teamMaxSize = team_villains.size();
	int idHero, idRm, solution, best_solution = 0, collaboration_lv, fighting_exp;
	vector<int> team_heroes, tabu_list, best_team (teamMaxSize);
	bool is_viavel = false;
	vector<Costs> heroCosts;
	double cost_team;

	// cria um vetor com os custos de todos os herois
	for(i = 0; i < heroes.size(); i++) {
	  	heroCosts.push_back(Costs(heroes[i].id, calculate_cost(heroes, heroes[i].id)));
	}
	
	// inicia com time encontrado por algoritmo guloso, nao necessariamente viavel
	team_heroes = initial_solution_with_budget(heroes, collab, team_villains, tabu_list, heroCosts, budget);

	// loop externo que faz a busca local
	for(i = 0; i < ITERATIONS_2; i++) {
		// se for a primeira iteracao calcula o valor de custo residual da solucao inicial
		if(i > 0) cost_team = budget;
		else cost_team = budget - calculate_cost(heroes, team_heroes);

		do {
			if(team_heroes.size() == teamMaxSize &&
				     is_viable_solution(heroes,villains,collab,team_heroes,team_villains,budget)) {
				 is_viavel = true;
			} else {
				// se o numero de herois superou o numero de viloes
				if(team_heroes.size() >= teamMaxSize) {
				   // se o custo do time de herois for maior que o budget
				   if(calculate_cost(heroes, team_heroes) > budget) {
				   	// busca o id do heroi com maior custo
				   	idRm = cost_max_team_heroes(heroes, team_heroes);
				   }
				   else {
				   	// busca o id do heroi com menor powergrid
				   	idRm = powergrid_team_heroes_min(heroes, collab, team_heroes, team_villains);
				   }
					
					// retira o heroi e inclui na lista tabu
					idHero = team_heroes[idRm];
					team_heroes.erase(team_heroes.begin()+idRm);
					add_tabu_list(tabu_list, team_villains, idHero, hasbudget);	
					
					// incrementa o valor do custo residual do time
					cost_team += heroCosts[idHero-1].cost;
				}
				
				// se o custo do time de herois for maior que o budget
				if(calculate_cost(heroes, team_heroes) > budget) {
					// calcula o id do heroi de custo maximo que respeita o budget medio
					idHero = collab_fighting_max_respect_cost(heroes, collab, team_heroes, 
						team_villains, tabu_list, heroCosts, cost_team);
					// se nao encontrar um heroi, inclui um heroi que tem maior colaboracao
					if(idHero == 0){	
						idHero = collab_fighting_max(heroes, collab, team_heroes, team_villains, tabu_list);
					}
				}
				else { // se nao o problema esta no powergrid do time
					// busca o id do heroi que tem melhor powergrid + colaboracao (sistema pontuacao)
					idHero = powergrid_collab_max(heroes, tabu_list, collab, team_heroes, team_villains);
				}
				
				// inclui o heroi na solucao
				team_heroes.push_back(idHero);
				
				// decrementa o valor do custo residual do time
				cost_team -= heroCosts[idHero-1].cost;
			}
		} while(!is_viavel);
		
		// calcula o valor da solucao
		collaboration_lv = collaboration_level(team_heroes, collab);
		fighting_exp = fighting_experience(team_heroes, team_villains, collab);
		solution = collaboration_lv + fighting_exp;
		
		// se for a primeira iteracao
		if(i == 0) {
			copy(team_heroes.begin(), team_heroes.end(), best_team.begin());
			best_solution = solution;
		} else {
			if(best_solution < solution) {
				 copy(team_heroes.begin(), team_heroes.end(), best_team.begin());
				 best_solution = solution;
			}
		}
			
		// retira da solucao o primeiro par de herois e inclui na lista tabu
		for(j = 0; j < 2; j++) {
			idHero = team_heroes[0];
			team_heroes.erase(team_heroes.begin());
			add_tabu_list(tabu_list, team_villains, idHero, hasbudget);
		}		
		// inicializa a variavel
		is_viavel = false;
	}

	return best_team;
}
