#include <bits/stdc++.h>

#include "graph.cpp"

using namespace std;

int main(){

	load_graphs("files/", 727);
	auto r_doc = resize_documents(10);
	int x = msc(r_doc[2], r_doc[277]);
	cout << x << endl;

	return 0;
}