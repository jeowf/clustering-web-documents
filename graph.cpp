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

const float MAX_DIST = 1.1;

// ------ //
// Global //
// ------ //
map<int, graph_t> documents;
map<int, map<node_t, int>> frequency;
vector< vector<graph_t> > clusters;
vector<int> assignments;

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

graph_t gen_key_search(istringstream& ss){

	int n;
	int prev = -1;
	
	graph_t g;

	while (ss >> n){
		if (prev == -1){
			prev = n;

		} else if (n > 1 and prev != n){
			g[prev].push_back(edge(edge::label_t::TEXT,n));
			prev = n;
		}
	}

	return g;
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

	graph_t g_msc;

	for(auto & e : a){
		if (!e.second.empty() and !b[e.first].empty()){

			for (auto & f : e.second) {
				for (auto & g : b[e.first]) {
					if (f.node == g.node and f.label == g.label){
						g_msc[e.first].push_back(f);
						intersec++;
						break;

					}
				}
			}
		}
			
	}
	//cout << intersec << endl;
	return intersec;

}



//DISTANCIA
float dist(graph_t a, graph_t b ){
	//float r = 1 - ((float) msc(a,b).size()/ ( (float) max(a,b)));
	float r = 1 - ((float) msc(a,b)/ ( (float) max(a,b)));
	return r;
}

//MEDIANA
// retorna índice da mediana
int median(vector<graph_t> & pages){
	int menor = 0;
	float backup_somatorio = INT_MAX;

	for (int i = 0; i < pages.size(); ++i) {
		float somatorio = 0;

		for (int j = 0; j < pages.size(); ++j){
			if (i!= j)
				somatorio += dist(pages[i],pages[j]);
		}

		float med = (somatorio/pages.size());

		if (med < backup_somatorio) {
			menor = i;
			backup_somatorio = med;
		}
	}
	return menor;
}

bool comp(graph_t & lhs, graph_t & rhs){

	if (lhs.size() != rhs.size())
		return false;

	for (int i = 0; i < lhs.size(); i++){
		if (lhs[i].size() != rhs[i].size()){
			return false;
			for (int j = 0; j < lhs[i].size(); i++){
				if (lhs[i][j].node != rhs[i][j].node)
					return false;
			}
		}
	}

	return true;
}

bool comp_graph_vec(vector<graph_t>  lhs, vector<graph_t>  rhs){

	if (lhs.size() != rhs.size())
		return true;

	for (int i = 0; i < lhs.size(); ++i) {
		if (!comp(lhs[i], rhs[i]))
			return false;
	}

	return true;

}

/*
vector<graph_t> k_means(const vector<graph_t> & docs, int k){
	vector<graph_t> prev_means(k);
	vector<graph_t> means(k);

	vector<int> r_index(docs.size());
	for (int i = 0; i < docs.size(); ++i)
		r_index.push_back(i);

	random_shuffle(r_index.begin(), r_index.end());

	for (int i = 0; i < k; ++i) {
		means[i] = docs[r_index[i]];
	}

	vector<int> assignments(docs.size());

	int it = 0;
	int max_it = 100;
	//while(1){
	while(!comp_graph_vec(means, prev_means)){

		for (int i = 0; i < docs.size(); i++) {
			float min_dist = 999999999;
			int c = 0;

			for (int clt = 0; clt < k; ++clt) {
				float d = dist(docs[i], means[clt]);
				if (d < min_dist){
					c = clt;
					min_dist = d;
				}
			}

			assignments[i] = c;

		}

		vector< vector<graph_t> > c_elements(k);

		for (int i = 0; i < docs.size(); ++i) {
			c_elements[ assignments[i] ].push_back(docs[i]);
		}

		for (int i = 0; i < k; ++i)	{
			if (!c_elements[i].empty()){
				int m = median(c_elements[i]);
				//centroides[i].clear();
				means[i] = c_elements[i][m];
				//c_elements[i].clear();
			}
		}

		// vector<graph_t> new_means(k);
		// vector<int> counts(k,0);

		// for (int i = 0; i < docs.size(); ++i) {
		// 	const auto cluster = assignments[i];
		// 	new_means[cluster].x += data[i].x;
		// 	new_means[cluster].y += data[i].y;
		// 	counts[cluster] += 1;
		// }

		prev_means = means;

		cout << it << endl;


		if (it++ >= max_it){
			cout << "BREAK!!\n";
			break;
		}
	}

	vector<int> res(k, 0);

	for (int i = 0; i < docs.size(); ++i) {
		res[assignments[i]]++;
	}

	int total = 0;
	for (int i = 0; i < k; ++i)
	{
		cout << i << " : " << res[i] << endl;
		total += res[i];
	}

	cout << "TOTAL: " << total << endl;
	cout << "opa";

	return means;
}*/


int best_cluster(graph_t g, const vector<graph_t> & centroides){


	float min_dist = 999999;
	int c = 0;
	for (int clt = 0; clt < centroides.size(); ++clt) {
		float d = dist(g,centroides[clt]);
		if (d < min_dist){
			c = clt;
			min_dist = d;
		}
	}
	return c;

}

vector<graph_t> k_means(const vector<graph_t> & docs, int k){
	clusters.resize(k);
	assignments.resize(docs.size());
	vector<graph_t> centroides_backup(k);

	vector<graph_t> centroides(k);

	// Atribuição arbitrária dos centroides
	for (int i = 0; i < k; ++i) {
		centroides[i] = docs[i];
		clusters[i].push_back(docs[i]);
	}

	// atribuição arbitrária dos documentos por cluster
	for (int i = 0; i < docs.size(); ++i){
		clusters[i%k].push_back(docs[i]);
	}


	int iterations = 0;
	int max_it = docs.size() * docs.size();

	while(!comp_graph_vec(centroides, centroides_backup)){
	//while(iterations < 100){
		
		centroides_backup = centroides;

		// calcular medianas
		for (int i = 0; i < k; ++i)	{
			if (!clusters[i].empty()){
				int m = median(clusters[i]);
				centroides[i].clear();
				centroides[i] = clusters[i][m];
				clusters[i].clear();
			}
		}


		for (int i = 0; i < docs.size(); i++) {
			float min_dist = 999999;
			int c = 0;
			for (int clt = 0; clt < k; ++clt) {
				float d = dist(docs[i],centroides[clt]);
				if (d < min_dist){
					c = clt;
					min_dist = d;
				}
			}
			clusters[c].push_back(docs[i]);
			assignments[i] = c;

		}

		//cout << iterations << endl;
		if (iterations++ > max_it)
			break;

	}
/*
	cout << "nodes of centroid 0:\n";

	for(int i = 0; i < k; i++){
		cout << i << ": " << centroides[i].size() << endl;
	}

	cout << endl;

	cout << "docs in the cluster 0:\n";

	int total = 0;
	for (int i = 0; i < k; i++){
		cout << i << ": " << clusters[i].size() << endl;

		total += clusters[i].size();
	}
	cout << "TOTAL: " << total << endl;


*/

	return centroides;
}

// ----- //
// Debug //
// ----- //
void print_graph(int graph){
	for(auto & e : documents[graph]){
		cout << e.first << "\n";

		for (auto &f: e.second) {
			cout << " " << f.node; 
		}
		cout << endl;
	}
}

void print_graph(graph_t & graph){

	for(auto & e : graph){
		cout << e.first << ":{";

		for (auto &f: e.second) {
			cout << " " << f.node; 
		}
		cout << "}" << endl;
	}
}


#endif