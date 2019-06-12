#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;


//ASSSUMINDO QUE TEMOS O VETOR DE GRAFOS "DOCUMENTS"

typedef map<int, vector<int>> Graphs;

//MAX
int max(Graphs a, Graphs b ){
	return max(a.size(), b.size());
}

//MAX SUBGRAFO COMUM

void msc(Graphs a, Graphs b ){

}

//DISTANCIA

int dist(Graphs a, Graphs b ){
	return 1 - (msc(a,b)/max(a,b));
}

//MEDIANA

Graphs median(vector<Graphs> pages){
	Graphs menor = 0;
	int backup_somatorio = 10000000;
	for (int i = 0; i < pages.size(); ++i)
	{
		int somatorio = 0;

		for (int j = 0; j < pages.size(); ++j)
		{
			somatorio += dist(pages[i],pages[j]);
		}

		somatorio = (somatorio/pages.size());

		if (somatorio < backup_somatorio)
		{
			menor = i;
			backup_somatorio = somatorio;
		}
	}
	return pages[menor];
}



void initial(vector<Graphs> pages, Graphs documents){
	int count = 0;

	//DEFINIÇÕES INICIAIS
	for (int i = 0; i < pages.size(); ++i)
	{
		centroides.push_back(documents[i]);
	}

	for (int i = 10; i < documents.size(); ++i)
	{
		pages[count].push_back(documents[i]);

		count++;
		if(count == page.size()){
			count = 0;
		}
	}
}

void new_centroides(vector<Graphs> pages, vector<Graphs> centroides ){
	for (int i = 0; i < pages.size(); ++i)
	{
		centroides[i] = median(pages[i]);
	}
}


void change_cluster(Graphs documents, vector<Graphs> centroides, vector<Graphs> pages_new ){
	for (int i = 0; i < documents.size(); ++i)
	{

		int choose = 0;
		int new_cluster = dist(documents[i], centroides[0]);

		for (int j = 1; j < centroides.size(); ++j)
		{
			int other_cluster = dist(documents[i], centroides[j]);
			if(other_cluster < new_cluster){
					choose = j;
			}
		}
	pages_new[choose].push_back(documents[i]);
	}
}


//MAIN

int main(int argc, char const *argv[])
{
	vector<Graphs> centroides_backup;
    
    vector<Graphs> centroides;

	vector<Graphs> pages[10];

	initial(pages,documents);

	//VERIFICAR SE MUDOU ALGO NOS CENTROIDES PARA PARAR
	while(equal(centroides.begin(), centroides.end(), centroides_backup.begin()) == false){

		vector<Graphs> pages_new[pages.size()];

		//CALCULANDO NOVOS CENTROIDES
		centroides_backup = centroides;

		new_centroides(pages,centroides);

		//MUDANDO AS PAGES DE CLUSTER
		change_cluster(documents, centroides, pages_new);
		//ATUALIZANDO CLUSTERS
		pages = pages_new;
	}

	return 0;
}