// please implement it
#include "Graph.hpp"

Graph::Graph(string& inputPath){
	inputFile.open(inputPath);
}
void Graph::buildGraph(){
	string line;
	int prevline = 0;
	int n;
	int lineno = 0;
	while(getline(inputFile,line)){
		lineno++;
	}
	for(int i = 1; i <= lineno; i++){
		Vertex q = Vertex(i);
		vertices.push_back(q);
	}
	inputFile.clear();
	inputFile.seekg(0,ios::beg);
	string line2;
	int firstnum = 0;
	while(getline(inputFile,line2)){
		
		prevline = 0;
		istringstream iss(line2);
			while(iss>>n && n != -1){
					if(prevline != 0){
						vertices[firstnum].connectTo(n);
						//cout << prevline << "," << n << endl;
					}
					prevline = n;
						
			}
			//cout << "end" << endl;
	firstnum++;
	}
	
}

void Graph::displayGraph(){
	int j = 0;
	for(int i = 0; i < vertices.size(); i++){
		j = 0;
		cout << vertices[i].label << "->";
		vertices[i].edgeList.begin();
		while(!vertices[i].edgeList.empty()){
			if(vertices[i].edgeList.front().end != -1){
				cout << vertices[i].edgeList.front().end << "->";
			}
			vertices[i].edgeList.pop_front();
		}
		
		cout << endl;
	}
	

}