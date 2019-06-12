#ifndef GRAPH
#define GRAPH

#include <bits/stdc++.h>

using namespace std;

/*
struct node{
	int label;
	int amount;

	node(){}
	node(int l, int a = 0) : 
	label(l), amount(a){}

	inline bool operator== (const node& rhs){ return this->label == rhs.label; }
	inline bool operator!= (const node& rhs){ return this->label != rhs.label; }
	inline bool operator< (const node& rhs){ return this->label < rhs.label; }
	inline bool operator<= (const node& rhs){ return this->label <= rhs.label; }
	inline bool operator> (const node& rhs){ return this->label > rhs.label; }
	inline bool operator>= (const node& rhs){ return this->label >= rhs.label; }
};*/

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

// ------- //
// Methods //
// ------- //
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

void dfs(int graph, int node, map<int,int> & node_set, graph_t & g){
	stack<int> ctrl;
	map<int, bool> visited;

	ctrl.push(node);
	visited[node] = true;

	for (auto & e : documents[graph][ctrl.top()]){
		if (node_set[e.node] == 0 and !visited[e.node]){
			ctrl.push(e.node);
			visited[e.node] = true;

		} else if (node_set[e.node] > 0 and !visited[e.node]) {
			g[node].push_back(edge(e.label, e.node));
		}
	}
	ctrl.pop();

	while(!ctrl.empty()){
		for (auto & e : documents[graph][ctrl.top()]){
			if (node_set[e.node] == 0 and !visited[e.node]){
				ctrl.push(e.node);
				visited[e.node] = true;
			} else if (node_set[e.node] > 0 and !visited[e.node]){
				g[node].push_back(edge(e.label, e.node));
			}
		}
		ctrl.pop();
	}
}

graph_t resize_graph(int graph, int new_size){
	
	graph_t g;
	vector<pii> freq;
	map<int,int> node_set;

	for (auto & e : frequency[graph])
		freq.push_back(make_pair(e.first, e.second));

	sort(freq.begin(), freq.end(), comp);

	for (int i = 0; i < new_size; i++) {
		node_set[freq[i].first] = freq[i].second;
	}

	//cout << node_set.size() << endl;

	for (int i = 0; i < new_size; i++){
		dfs(graph, freq[i].first, node_set, g);
	}
	cout << g.size() << endl;
	return g;
}

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