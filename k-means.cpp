#include <iostream>
#include <vector>
#include <map>
#include <algorithm>


//ASSSUMINDO QUE TEMOS O VETOR DE GRAFOS "DOCUMENTS"

typedef std::map<int, std::vector<int>> Graphs;

//MAX
int max(Graphs a, Graphs b ){
	return std::max(a.size(), b.size());
}

//MAX SUBGRAFO COMUM

void msc(Graphs a, Graphs b ){

}

//DISTANCIA

int dist(Graphs a, Graphs b ){
	return 1 - (msc(a,b)/max(a,b));
}

//MEDIANA

Graphs median(std::vector<Graphs> pages){
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

//MAIN

int main(int argc, char const *argv[])
{
	std::vector<Graphs> centroides_backup;
    
    std::vector<Graphs> centroides;

	std::vector<Graphs> pages[10];

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
		if(count == 10){
			count = 0;
		}
	}
	//VERIFICAR SE MUDOU ALGO NOS CENTROIDES PARA PARAR
	while(std::equal(centroides.begin(), centroides.end(), centroides_backup.begin()) == false){

		std::vector<Graphs> pages_new[pages.size()];

		//CALCULANDO NOVOS CENTROIDES
		centroides_backup = centroides;
		for (int i = 0; i < pages.size(); ++i)
		{
			centroides[i] = median(pages[i]);
		}

		//MUDANDO AS PAGES DE CLUSTER
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
		//ATUALIZANDO CLUSTERS
		pages = pages_new;
	}

	return 0;
}