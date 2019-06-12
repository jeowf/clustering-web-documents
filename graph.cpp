#ifndef GRAPH
#define GRAPH

#include <bits/stdc++.h>

using namespace std;

// ---------- //
// Structures //
// ---------- //
struct edge{
	enum class label_t{
		TITLE,
		HYPERLINK,
		TEXT
	};
	label_t label;
	int node;

	edge(){}
	edge(label_t l, int n) : 
	label(l), node(n) {}

};

// ----- //
// Alias //
// ----- //
typedef int node_t;
typedef pair<int, int> pii;
typedef map<node_t, vector<edge>> graph_t;

// ------ //
// Global //
// ------ //
map<int, graph_t> documents;
map<int, map<node_t, int>> frequency;

// -------------- //
// Pre-processing //
// -------------- //
void load_section(edge::label_t section, int graph, ifstream& inp){

	int n;
	int prev = -1;
	while (inp >> n and n != 0){
		if (prev == -1){
			prev = n;
			frequency[graph][n]++;

		} else if (n == 1){
			prev = -1;

		} else if (n > 1 and prev != n){
			documents[graph][prev].push_back(edge(section,n));
			prev = n;
			frequency[graph][n]++;
		}
	}
}

void load_graphs(string folder, int n_files){
	for (int page = 0; page < n_files; ++page) {
		string page_index = to_string(page);
		ifstream doc(folder + page_index);

		while (!doc.eof()){
			load_section(edge::label_t::TITLE, page, doc);
			load_section(edge::label_t::HYPERLINK, page, doc);
			load_section(edge::label_t::TEXT, page, doc);
		}

		doc.close();
	}
}

bool comp(pii lhs, pii rhs){

	return lhs.second > rhs.second;
}

void dfs(int graph, int node, set<int> & node_set, graph_t & g){
	stack<int> ctrl;
	map<int, bool> visited;

	ctrl.push(node);
	visited[node] = true;

	for (auto & e : documents[graph][ctrl.top()]){
		auto test = node_set.find(e.node) != node_set.end();

		if (!test and !visited[e.node]){
			ctrl.push(e.node);
			visited[e.node] = true;

		} else if (test and !visited[e.node]) {
			g[node].push_back(edge(e.label, e.node));
			visited[e.node] = true;
		}
	}
	ctrl.pop();

	while(!ctrl.empty()){

		for (auto & e : documents[graph][ctrl.top()]){
			auto test = node_set.find(e.node) != node_set.end();

			if (!test and !visited[e.node]){
				ctrl.push(e.node);
				visited[e.node] = true;

			} else if (test and !visited[e.node]) {
				g[node].push_back(edge(e.label, e.node));
				visited[e.node] = true;

			}
		}
		ctrl.pop();
	}
}

graph_t resize_graph(int graph, int new_size){
	
	graph_t g;
	vector<pii> freq;
	set<int> node_set;

	for (auto & e : frequency[graph])
		freq.push_back(make_pair(e.first, e.second));

	sort(freq.begin(), freq.end(), comp);

	for (int i = 0; i < new_size; i++) {
		node_set.insert(freq[i].first);
	}

	//cout << node_set.size() << endl;

	for (int i = 0; i < new_size; i++){
		dfs(graph, freq[i].first, node_set, g);
	}
	return g;
}

vector<graph_t> resize_documents(int new_size){
	vector<graph_t> resized;
	for (auto & e : documents) {
		resized.push_back(resize_graph(e.first, new_size));
	}

	return resized;
}


// --------------- //
// K-means methods //
// --------------- //

//MAX
int max(graph_t & a, graph_t & b ){
	return max(a.size(), b.size());
}

//MAX SUBGRAFO COMUM

int msc(graph_t & a, graph_t & b){
	int intersec = 0;

	for(auto & e : a){
		if (!e.second.empty() and !b[e.first].empty())
			intersec++;
	}

	return intersec;

}

/*
graph_t msc(graph_t a, graph_t b ){

	graph_t graph;

	return graph;
}*/
/*
//DISTANCIA
float dist(graph_t a, graph_t b ){
	float r = 1 - ((float) msc(a,b).size()/ ( (float) max(a,b)));
	return r;
}

//MEDIANA
graph_t median(vector<graph_t> pages){
	int menor = 0;
	int backup_somatorio = INT_MAX;
	for (int i = 0; i < pages.size(); ++i) {
		int somatorio = 0;

		for (int j = 0; j < pages.size(); ++j){
			somatorio += dist(pages[i],pages[j]);
		}

		float med = (somatorio/pages.size());

		if (med < backup_somatorio) {
			menor = i;
			backup_somatorio = med;
		}
	}
	return pages[menor];
}

void k_means(vector<graph_t> & docs, int k){
	vector<graph_t> centroides_backup(k);

	vector<graph_t> centroides(k);
	vector<vector<graph_t>> clusters(k);

	// Atribuição arbitrária dos centroides
	for (int i = 0; i < 10; ++i) {
		centroides[i] = docs[i];
		clusters[i].push_back(docs[i]);
	}

	// atribuição arbitrária dos documentos por cluster
	for (int i = 10; i < docs.size(); ++i){
		clusters[i%k].push_back(docs[i]);
	}


	while(equal(centroides.begin(), centroides.end(), centroides_backup.begin()) == false){

		//vector<graph_t> pages_new[pages.size()];

		centroides_backup = centroides;

		// calcular medianas
		for (int i = 0; i < k; ++i)	{
			centroides[i] = median(clusters[i]);
		}

		//CALCULANDO NOVOS CENTROIDES

		//new_centroides(pages, centroides);

		//MUDANDO AS PAGES DE CLUSTER
		change_cluster(documents, centroides, pages_new);
		


		//ATUALIZANDO CLUSTERS
		//pages = pages_new;
	}

	

	while
}*/
/*
void initial(vector<graph_t> pages, graph_t documents){
	int count = 0;

	//DEFINIÇÕES INICIAIS
	for (int i = 0; i < pages.size(); ++i) {
		centroides.push_back(documents[i]);
	}

	for (int i = 10; i < documents.size(); ++i)	{
		pages[count].push_back(documents[i]);

		count++;
		if(count == page.size()){
			count = 0;
		}
	}
}

void new_centroides(vector<graph_t> pages, vector<graph_t> centroides ){
	for (int i = 0; i < pages.size(); ++i)
	{
		centroides[i] = median(pages[i]);
	}
}



void change_cluster(vector<graph_t> documents, 
	                vector<graph_t> centroides, 
	                vector<graph_t> pages_new ){
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
*/
// ----- //
// Debug //
// ----- //
void print_graph(int graph){
	for(auto & e : documents[graph]){
		cout << e.first 
		     << "(" << frequency[graph][e.first] 
		     << "," <<e.second.size() << ")\n";

		for (auto &f: e.second) {
			cout << " " << f.node; 
		}
		cout << endl;
	}
}

void print_graph(graph_t & graph){
	//cout << "aaa\n";
	for(auto & e : graph){
		cout << e.first << " : " << e.second.size() << "\n";

		for (auto &f: e.second) {
			cout << " " << f.node; 
		}
		cout << endl;
	}
}


#endif