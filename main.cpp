#include <bits/stdc++.h>

#include "graph.cpp"

using namespace std;

const int K = 10;

int main(){

	load_graphs("files/", 727);
	auto r_doc = resize_documents(K);
	//int x = msc(r_doc[2], r_doc[277]);
	//cout << x << endl;
	auto y = k_means(r_doc, K);
	return 0;
}