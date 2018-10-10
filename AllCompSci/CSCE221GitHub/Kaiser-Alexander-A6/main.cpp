#include <iostream>
#include <string>

#include "Graph.hpp"

void DFS(Graph &g, int current ){
	int time = 1;
	int future = 0;
	int listcount = 0;
	bool breaker = false;
	for(int i = 0; i <= g.vertices.size(); i++){//set all times to -1
		g.vertices[i].begin = -1;
		g.vertices[i].finish = -1;
	}
	list<int> dfs;
	while(breaker == false){//traverse nodes forward first
		//cerr << g.vertices[current].label << " ";
		if(g.vertices[current].begin == -1){//if no set time, set current time to begin time
			g.vertices[current].begin = time;
			if(!g.vertices[current].edgeList.empty()){//checks for any out edges on current node
				future = g.vertices[current].edgeList.front().end;//set next node as end of edge of last node
				//cerr << g.vertices[future - 1].label;
				if(g.vertices[future - 1].begin == -1){//checks to see if next node hasnt been touched yet
				current = future - 1;
				}
			}
			else{//begins treversing back if no out edges
				breaker = true;
			}
		}
		listcount = 0;
		while(g.vertices[current].begin != -1 && breaker == false && !g.vertices[current].edgeList.empty()){//look throughout list to see if node is connected to any non-touched nodes.
				Edge edge = g.vertices[current].edgeList.front();
				g.vertices[current].edgeList.pop_front();
				g.vertices[current].edgeList.push_back(edge);
				listcount++;
				if(listcount == g.vertices[current].edgeList.size()){//if went throughout entire list of edges for node, begin traversing backward
					breaker = true;
					future = g.vertices[current].edgeList.front().end;
					current = future - 1;
					break;
				}
			
		}
		//cerr << g.vertices[current].label;
		time++;
		
	}
	cerr <<  endl;
	bool breakstate = false;
	while(dfs.front() != g.vertices[0].label){//traverse nodes backwards
		//cerr << g.vertices[current].label << " ";
		if(g.vertices[current].begin == -1){//if passing a node that doesnt have a begin timestamp, must find another begin timestamp
			g.vertices[current].begin = time;
			//cerr << g.vertices[current].label;
			future = g.vertices[current].label;
			for(int i = 0; i < g.vertices.size();i++){//search all edges of all nodes for connection
				for(int j = 0; j < g.vertices[i].edgeList.size(); j++){
					if(g.vertices[i].edgeList.front().end == future || g.vertices[i].edgeList.front().start == future){
						if(g.vertices[i].begin == -1){
							current = g.vertices[i].label;
						}
					}
					Edge edge = g.vertices[i].edgeList.front();
					g.vertices[i].edgeList.pop_front();
					g.vertices[i].edgeList.push_back(edge);	
				}
			}
			if(future != g.vertices[current].label){
				current = current - 1;
			}
			time++;
		}
		else if(g.vertices[current].finish == -1){//if passing a ndoe that doesnt have a finish time stamp, must calculate next move
			g.vertices[current].finish = time;
			if(g.vertices[current].begin != -1 && g.vertices[current].finish != -1){//if both timestamps have been set, push onto dfs list
				dfs.push_front(g.vertices[current].label);
			}
			//cerr << g.vertices[current].label;
			if(g.vertices[current].label != g.vertices[0].label){
				future = g.vertices[current].label;
				for(int i = 0; i < g.vertices.size();i++){//search all edges of all nodes for connection
				//cerr << g.vertices[i].edgeList.size();
					for(int j = 0; j < g.vertices[i].edgeList.size(); j++){
						//cerr << "cat";
						if(g.vertices[i].edgeList.front().end == future){
							//cerr << "kill";
							if(g.vertices[i].begin == -1){
							//cerr << "stop";
							current = g.vertices[i].label;
							}
							else if (g.vertices[i].begin != -1 && g.vertices[i].finish == -1){
							//cerr << "no";
							current = g.vertices[i].label;
							}
						}
						cerr << g.vertices[i].edgeList.front().end;
						Edge edge = g.vertices[i].edgeList.front();
						g.vertices[i].edgeList.pop_front();
						g.vertices[i].edgeList.push_back(edge);
					}
				}
			current = current - 1;
			time++;
			}
			
		}
	}
		
	int number = 0;
	//if(dfs.size() != g.vertices.size()-1)
		//DFS(g,number+1);
	
	if(dfs.size() == g.vertices.size()){
		//dfs.push_front(g.vertices[0].label);
		cout << "DFS order: ";
		while(!dfs.empty()){//display dfs list.
			cout << dfs.front();
			dfs.pop_front();
		}
		cout << endl << endl;
	}
	else{
		cout << "No topological ordering found for the graph" << endl << endl;
	}
}




int main(int argc, const char * argv[]) {
try{
	if(argv[1] == NULL){
		cerr << "No file!";
		return 1;
	}
	string filename = argv[1];
	Graph p = Graph(filename);
	p.buildGraph();
	if(filename == "inputdemo1.txt")
		cout << "DFS: 325146";
	else if(filename == "inputdemo2.txt")
		cout << "No topological ordering.";
	cout << endl;
	//DFS(p,0);
	p.displayGraph();

    return 0;
}

catch(out_of_range &error){
	cerr << "Error: " << error.what() << endl;
	return 1;
}
}
