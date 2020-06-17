// Digraph.hpp
//
// ICS 46 Spring 2020
// Project #5: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph.  The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <queue>



// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the predecessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> * graph;
    int vertexCounter = 0;
    int edgeCounter = 0;

    struct daTableVertex {
        bool isShortestPathKnown;
        int shortestPathFound;
        int predecessor;
    };

    bool recursiveConnection(int fromVertex, int toVertex, std::vector<bool>& visited) const;

    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.
};



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
    : graph{new std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>}
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
    : graph{new std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>}
{
    //Vector containing all the vertices
    std::vector<int> verticesToCopy = d.vertices();

    //For each vertex, add a copy to this.graph
    for (int vertex : verticesToCopy) {
        //Create a new vertex to copy to
        auto newVertex = DigraphVertex<VertexInfo, EdgeInfo>();

        //Set the value of the copied vertex's vinfo to the value of the original vertex
        newVertex.vinfo = d.vertexInfo(vertex);

        //Create a copy of the edges list
        std::list<DigraphEdge<EdgeInfo>> copyList;
        std::vector<std::pair<int, int>> vertexEdges = d.edges(vertex);
        for (std::pair<int, int> edge : vertexEdges) {
            auto newEdge = DigraphEdge<EdgeInfo>();
            newEdge.fromVertex = edge.first;
            newEdge.toVertex = edge.second;
            newEdge.einfo = d.edgeInfo(edge.first, edge.second);
            copyList.push_back(newEdge);

            //Increment the # of edges in the graph
            edgeCounter++;
        }
        //Assign it
        newVertex.edges = copyList;

        graph->insert(std::make_pair(vertex, newVertex));

        //Increment the # of vertices in the graph
        vertexCounter++;
    }
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
    graph = d.graph;
    vertexCounter = d.vertexCount();
    edgeCounter = d.edgeCount();

    d.graph = nullptr;
    d.vertexCounter = 0;
    d.edgeCounter = 0;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
    delete graph;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    //Remove all old values
    delete graph;
    graph = new std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>;
    vertexCounter = 0;
    edgeCounter = 0;

    //Vector containing all the vertices
    std::vector<int> verticesToCopy = d.vertices();

    //For each vertex, add a copy to this.graph
    for (int vertex : verticesToCopy) {
        //Create a new vertex to copy to
        auto newVertex = DigraphVertex<VertexInfo, EdgeInfo>();

        //Set the value of the copied vertex's vinfo to the value of the original vertex
        newVertex.vinfo = d.vertexInfo(vertex);

        //Create a copy of the edges list
        std::list<DigraphEdge<EdgeInfo>> copyList;
        std::vector<std::pair<int, int>> vertexEdges = d.edges(vertex);
        for (std::pair<int, int> edge : vertexEdges) {
            auto newEdge = DigraphEdge<EdgeInfo>();
            newEdge.fromVertex = edge.first;
            newEdge.toVertex = edge.second;
            newEdge.einfo = d.edgeInfo(edge.first, edge.second);
            copyList.push_back(newEdge);

            //Increment the # of edges in the graph
            edgeCounter++;
        }
        //Assign it
        newVertex.edges = copyList;

        graph->insert(std::pair(vertex, newVertex));

        //Increment the # of vertices in the graph
        vertexCounter++;
    }

    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    //Swap graphs
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> * tempGraph = graph;
    graph = d.graph;
    d.graph = tempGraph;

    //Swap vertex counters
    int tempVertexCounter = vertexCounter;
    vertexCounter = d.vertexCount();
    d.vertexCounter = tempVertexCounter;

    //Swap edge counters
    int tempEdgeCounter = edgeCounter;
    edgeCounter = d.edgeCount();
    d.edgeCounter = tempEdgeCounter;

    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
    std::vector<int> toReturn;

    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr = graph->begin();
    while (itr != graph->end()) {
        toReturn.push_back(itr->first);
        itr++;
    }

    return toReturn;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int, int>> toReturn;

    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr = graph->begin();
    while (itr != graph->end()) {
        for (DigraphEdge<EdgeInfo> edge : itr->second.edges) {
            toReturn.push_back(std::make_pair(edge.fromVertex, edge.toVertex));
        }
        itr++;
    }

    return toReturn;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr = graph->find(vertex);

    if (itr != graph->end()) {
        std::vector<std::pair<int, int>> toReturn;

        for (DigraphEdge<EdgeInfo> edge : itr->second.edges) {
            toReturn.push_back(std::make_pair(edge.fromVertex, edge.toVertex));
        }

        return toReturn;
    } else {
        throw DigraphException("Given vertex does not exist");
    }
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr = graph->find(vertex);

    if (itr != graph->end()) {
        return itr->second.vinfo;
    } else {
        throw DigraphException("Given vertex does not exist");
    }
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr1 = graph->find(fromVertex);
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr2 = graph->find(toVertex);

    if (itr1 != graph->end() && itr2 != graph->end()) {
        for (DigraphEdge<EdgeInfo> edge : itr1->second.edges) {
            if (edge.fromVertex == fromVertex && edge.toVertex == toVertex) {
                return edge.einfo;
            }
        }
        throw DigraphException("The edge does not exist");
    } else {
        throw DigraphException("Invalid vertices (One or both do not exist)");
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    auto newVertex = DigraphVertex<VertexInfo, EdgeInfo>();
    newVertex.vinfo = vinfo;

    if (!graph->insert(std::make_pair(vertex, newVertex)).second) {
        throw DigraphException("There is already a vertex with that vertex number in this Digraph");
    }

    vertexCounter++;
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr1 = graph->find(fromVertex);
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr2 = graph->find(toVertex);

    if (itr1 != graph->end() && itr2 != graph->end()) {

        for (DigraphEdge<EdgeInfo> edge : itr1->second.edges) {
            if (edge.fromVertex == fromVertex && edge.toVertex == toVertex) {
                throw DigraphException("That edge already exists in this Digraph");
            }
        }

        auto newEdge = DigraphEdge<EdgeInfo>();
        newEdge.fromVertex = fromVertex;
        newEdge.toVertex = toVertex;
        newEdge.einfo = einfo;

        itr1->second.edges.push_back(newEdge);
    } else {
        throw DigraphException("Invalid vertices (One or both do not exist)");
    }

    edgeCounter++;
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr = graph->find(vertex);

    if (itr != graph->end()) {
        edgeCounter -= edgeCount(vertex);
        graph->erase(itr);

        itr = graph->begin();
        while (itr != graph->end()) {
            typename std::list<DigraphEdge<EdgeInfo>>::iterator itrEdges = itr->second.edges.begin();
            while (itrEdges != itr->second.edges.end()) {
                if (itrEdges->toVertex == vertex) {
                    itr->second.edges.erase(itrEdges);
                    edgeCounter--;
                }
                itrEdges++;
            }
            itr++;
        }
    } else {
        throw DigraphException("Given vertex already does not exist");
    }

    vertexCounter--;
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr1 = graph->find(fromVertex);
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr2 = graph->find(toVertex);
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator vertex = graph->begin();

    if (itr1 != graph->end() && itr2 != graph->end()) {
        while (vertex != graph->end()) {
            typename std::list<DigraphEdge<EdgeInfo>>::iterator itrEdges = vertex->second.edges.begin();
            while (itrEdges != vertex->second.edges.end()) {
                if (itrEdges->fromVertex == fromVertex && itrEdges->toVertex == toVertex) {
                    vertex->second.edges.erase(itrEdges);
                    edgeCounter--;
                    return;
                }
                itrEdges++;
            }
            vertex++;
        }
        throw DigraphException("That edge already does not exist");
    } else {
        throw DigraphException("Invalid vertices (One or both do not exist)");
    }
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return vertexCounter;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    return edgeCounter;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr = graph->find(vertex);
    if (itr != graph->end()) {
        int count = 0;
        typename std::list<DigraphEdge<EdgeInfo>>::iterator itrEdges = itr->second.edges.begin();
        while (itrEdges != itr->second.edges.end()) {
            count++;
            itrEdges++;
        }
        return count;
    } else {
        throw DigraphException("The given vertex does not exist");
    }
}


template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    std::vector<int> vertices = this->vertices();

    for (int vertexFrom : vertices) {
        for (int vertexTo : vertices) {
            if (vertexFrom != vertexTo) {
                std::vector<bool> visited(vertices.size(), false);
                if (!recursiveConnection(vertexFrom, vertexTo, visited)) {
                    return false;
                }
            }
        }
    }

    return true;
}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    //Create table
    std::map<int, daTableVertex> daTable;
    //Fill table
    for (std::pair<int, DigraphVertex<VertexInfo, EdgeInfo>> vertex : *graph) {
        daTableVertex temp{false, INT_MAX, -1};
        daTable[vertex.first] = temp;
    }
    //Create starting vertex
    daTable[startVertex] = daTableVertex{false, 0, -1};
    //Custom comparison function
    auto compare = [daTable](const int leftVertex, const int rightVertex) {
        return daTable.at(leftVertex).shortestPathFound > daTable.at(rightVertex).shortestPathFound;
    };

    //Shortest Path Algorithm
    std::priority_queue<int, std::vector<int>, decltype(compare)> pq(compare);

    pq.push(startVertex);
    while(!pq.empty()) {
        int lpVertex = pq.top();
        daTable[lpVertex].isShortestPathKnown = true;
        pq.pop();
        for (DigraphEdge<EdgeInfo>& edge : graph->at(lpVertex).edges) {
            if (!daTable.at(edge.toVertex).isShortestPathKnown) {
                int edgeWeight = edgeWeightFunc(edge.einfo);
                if (daTable.at(edge.toVertex).shortestPathFound > daTable.at(lpVertex).shortestPathFound + edgeWeight) {
                    daTable.at(edge.toVertex).shortestPathFound = daTable.at(lpVertex).shortestPathFound + edgeWeight;
                    daTable.at(edge.toVertex).predecessor = lpVertex;
                    pq.push(edge.toVertex);
                }
            }
        }
    }

    //Transfer calculated shortest paths to map
    std::map<int, int> shortestPaths;
    for (std::pair<int, daTableVertex> vertex : daTable) {
        if (vertex.second.predecessor < 0) {
            shortestPaths[vertex.first] = vertex.first;
        } else {
            shortestPaths[vertex.first] = vertex.second.predecessor;
        }
    }

    return shortestPaths;
}



template<typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::recursiveConnection(int fromVertex, int toVertex, std::vector<bool>& visited) const {
    std::vector<std::pair<int, int>> edges = this->edges(fromVertex);

    if (edges.empty() || visited[fromVertex]) {return false;}

    for (std::pair<int, int> e : edges) {
        if (e.second == toVertex) {
            return true;
        }
    }

    visited[fromVertex] = true;

    for (std::pair<int, int> e : edges) {
        if (recursiveConnection(e.second, toVertex, visited)) {
            return true;
        }
    }

    return false;
}


#endif // DIGRAPH_HPP

