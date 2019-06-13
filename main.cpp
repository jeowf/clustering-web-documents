#include <bits/stdc++.h>

#include "graph.cpp"

using namespace std;

const int K = 10;
const int G = 5;

void f(){

	vector<int> ks = {5, 10, 15, 30, 50};
	vector<int> gs = {3, 5, 10, 20, 50};

	for (auto & k : ks) {
		cout << "K = " << k << endl;

		for (auto & g : gs) {
			cout << " G = " << g << endl;

			load_graphs("files/", 727);
			auto r_doc = resize_documents(g);
			auto means = k_means(r_doc, k);
			
			for (int i = 0; i < k; i++){
				cout << "  " << i << " : " << clusters[i].size() << endl;

			}

			documents.clear();
			frequency.clear();
			clusters.clear();
			assignments.clear();

		}
		cout << endl;
	}

	
}

int main(){

#ifdef DATA
	f();
	return 0;

#endif

	cout << "Carregando arquivos...\n";
	load_graphs("files/", 727);

	cout << "Tratando os dados...\n";
	auto r_doc = resize_documents(G);

	cout << "Clusterizando documentos...\n";
	auto means = k_means(r_doc, K);
	
	cout << "Processamento concluído...\n";

#ifdef SOL
	cout << endl;

	cout << "Páginas por cluster:\n";
	for (int i = 0; i < K; i++){
		cout << i << " : " << clusters[i].size() << endl;
	}
	cout << endl;

#endif

#ifdef LIST

	for (int i = 0; i < K; ++i)
	{	
		cout << "CLUSTER = " << i << endl;
		for (auto & e : clusters[i]){
		 	print_graph(e);
			cout << endl;

		}
		cout << endl;

	}

#endif
/*
	for (int i = 0; i < K; ++i)
	{	
		cout << "CLUSTER " << i << endl;
		for (auto & e : clusters[i]){
		 	print_graph(e);
		}
		cout << " ------------------------------------- \n";
		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
	}
	

	for (int j = 0; j < K; j++){
		cout << "CLUSTER : " << j << endl;
		for (int i = 0; i < assignments.size(); ++i) {
			if (assignments[i] == j) 
				cout << "   " << i << endl;
		}
		cout << endl;
	}

*/

	string str;
	while (1){
		cout << "Digite sua busca (números de 2 à 500.000):\n";
		getline(cin, str);
		istringstream ss(str);


		auto g = gen_key_search(ss);
		int clt = best_cluster(g, means);
		
		cout << endl;
		cout << "Lista dos documentos (em files/) que possuem a chave <" << str << ">:\n";
		for (int i = 0; i < assignments.size(); ++i) {
			if (assignments[i] == clt) 
				cout << " .../files/" << i << endl;
		}

	}
	
	return 0;
}