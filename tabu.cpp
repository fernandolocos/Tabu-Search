#include "tabu.h"
using namespace std;

bool myfunction (collaboration i,collaboration j) {return (i.value>j.value);}

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

bool inSolution(vector<int> team_heroes, int idHero) {
	unsigned int i;
	for (i = 0; i < team_heroes.size(); i++) {
		if(team_heroes[i] == idHero)
			return true;
	}
	return false;
}

bool isHero(int idCharacter) {
	if(idCharacter <= 381)
			return true;
	return false;
}

vector<int> initial_solution(vector<character> &heroes, 
	vector<character> &villains, vector<collaboration> &collab, 
	unsigned int teamSize)
{
	unsigned int i;
	vector<int> team_heroes;
	
	// ordena de forma decrescente o vetor de colaboracoes
	sort (collab.begin(), collab.end(), myfunction);

	// monta uma solucao possivel a partir do vetor ordenado
	for (i = 0; i < collab.size(); i++) {
		// se o heroi1 nao estiver na solucao, coloca na solucao
		if(isHero(collab[i].c1) && team_heroes.size() < teamSize && 
			!inSolution(team_heroes, collab[i].c1)){
				team_heroes.push_back(collab[i].c1);
		}
		
		// se o heroi2 nao estiver na solucao, coloca na solucao
		if(isHero(collab[i].c2) && team_heroes.size() < teamSize && 
			!inSolution(team_heroes, collab[i].c2)){
				team_heroes.push_back(collab[i].c2);
		}
		
		// se o numero de herois for igual ao de viloes
		if(team_heroes.size()== teamSize)
			break;
	}	
	
	return team_heroes;
}

/* busca de uma boa solucao (mesmo que inviavel) 
partindo da solucao retornada pelo algoritmo de construcao inicial (que eh a
melhor mas nao e viavel) verifico se ela eh tem custo > budget se sim, troca
o melhor heroi por um heroi pior (seguindo algum criterio) se nao, troca o pior
heroi por um heroi melhor. Faça isso por um numero de iteracoes fixo.
*/

/* busca local por uma solucao viavel
partindo da solucao do algoritmo que encontra uma boa solucao mas nao
necessariamente viavem, executa uma busca local, ou seja, troca heroi pelo
seus vizinhos em termos de custo, a fim de encontrar a melhor solucao que 
seja viavel
*/
