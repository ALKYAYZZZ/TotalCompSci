#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


#include "Vertex.hpp"

using namespace std;

class Graph {
public:
    // we use a vector to store vertices in the graph
    // and use label (int) to be a subscript to access a vertex
    ifstream inputFile;
    vector<Vertex> vertices;
    Graph(string& inputPath);
    // build a graph according to the input file
    void buildGraph();
    // display the graph
    void displayGraph();
	// depth first search
};

#endif /* Graph_hpp */
