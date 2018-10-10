// please implement it

#include "Vertex.hpp"

Vertex::Vertex(int lab){
	label = lab;
}

void Vertex::connectTo(int end){
	Edge p = Edge(label,end,0);
	edgeList.push_back(p);
}