#include <bits/stdc++.h>

#include "graph.cpp"

using namespace std;

int main(){

	load_graphs("files/", 727);
	print_graph(2);
	cout << endl;
	cout << endl;
	cout << endl;
	graph_t g = resize_graph(2, 10);
	cout << "MAGICA" << endl;
	cout << "MAGICA" << endl;
	cout << "MAGICA" << endl;
	cout << "MAGICA" << endl;
	cout << "MAGICA" << endl;
	cout << "MAGICA" << endl;
	cout << "MAGICA" << endl;
	cout << "MAGICA" << endl;
	print_graph(g);

	cout << documents[2].size() << endl;
	cout << g.size() << endl;


	return 0;
}