#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>
#include <list>
#include <string>
#include <vector>

#include "Edge.hpp"
using namespace std;

class Vertex {
public:
    // the label of this vertex
    int label;
	int begin;
	int finish;
    // using a linked-list to manage its edges which offers O(c) insertion
    list<Edge> edgeList;
    
    // init your vertex here
    Vertex(int lab);
    
    // connect this vertex to a specific vertex (adding edge)
    void connectTo(int end);
};

#endif /* Vertex_hpp */
