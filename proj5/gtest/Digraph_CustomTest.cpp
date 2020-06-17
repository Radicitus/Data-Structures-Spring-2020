#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <gtest/gtest.h>
#include "Digraph.hpp"

TEST(Digraph_CustomTest, doesCopyConstructorCreateExactCopies)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);

    d1.addEdge(1, 2, 5.0);
    d1.addEdge(2, 3, 17.0);

    Digraph<int, double> d2 = d1;

    ASSERT_TRUE(d1.vertices() == d2.vertices());
    ASSERT_TRUE(d1.edges() == d2.edges());
}

TEST(Digraph_CustomTest, doesCopyAssignmentCreateExactCopies)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);

    d1.addEdge(1, 2, 5.0);
    d1.addEdge(2, 3, 17.0);

    Digraph<int, double> d2;
    d2.addVertex(1, 200);
    d2.addVertex(2, 300);
    d2.addVertex(3, 400);

    d2.addEdge(1, 2, 150.0);
    d2.addEdge(2, 3, 117.0);

    d2 = d1;

    ASSERT_TRUE(d1.vertices() == d2.vertices());
    ASSERT_TRUE(d1.edges() == d2.edges());
}

TEST(Digraph_CustomTest, checkVerticesFunc)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);

    ASSERT_TRUE(d1.vertices().size() == 3);
    for (int v : d1.vertices()) {
        std::cout << v << std::endl;
    }
}

TEST(Digraph_CustomTest, checkEdgesFunc)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);

    d1.addEdge(1, 2, 5.0);
    d1.addEdge(2, 3, 17.0);

    ASSERT_TRUE(d1.edges().size() == 2);
    for (std::pair<int, int> e : d1.edges()) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
}

TEST(Digraph_CustomTest, checkOverloadEdgesFunc)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);

    d1.addEdge(1, 2, 5.0);
    d1.addEdge(2, 3, 17.0);
    d1.addEdge(3, 1, 1);
    d1.addEdge(3, 2, 1);
    d1.addEdge(3, 3, 1);

    ASSERT_TRUE(d1.edges(3).size() == 3);
    for (std::pair<int, int> e : d1.edges(3)) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
}

TEST(Digraph_CustomTest, checkVertexInfoFunc)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);

    ASSERT_TRUE(d1.vertexInfo(1) == 10);
    ASSERT_TRUE(d1.vertexInfo(2) == 20);
    ASSERT_TRUE(d1.vertexInfo(3) == 30);
    for (int v : d1.vertices()) {
        std::cout << v << std::endl;
    }
}

TEST(Digraph_CustomTest, checkEdgeInfoFunc)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);

    d1.addEdge(1, 2, 5.0);
    d1.addEdge(2, 3, 17.0);
    d1.addEdge(3, 1, 100);
    d1.addEdge(3, 2, 101);
    d1.addEdge(3, 3, 102);

    ASSERT_TRUE(d1.edgeInfo(3, 1) == 100);
    ASSERT_TRUE(d1.edgeInfo(3, 2) == 101);
    ASSERT_TRUE(d1.edgeInfo(3, 3) == 102);
    for (std::pair<int, int> e : d1.edges()) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
}

TEST(Digraph_CustomTest, checkRemoveVertexFunc)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);

    d1.addEdge(1, 2, 5.0);
    d1.addEdge(2, 3, 17.0);
    d1.addEdge(3, 1, 100);
    d1.addEdge(3, 2, 101);
    d1.addEdge(3, 3, 102);

    for (std::pair<int, int> e : d1.edges()) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
    for (int v : d1.vertices()) {
        std::cout << v << std::endl;
    }

    d1.removeVertex(3);

    ASSERT_TRUE(d1.vertices().size() == 2);
    ASSERT_TRUE(d1.edges().size() == 1);
    for (std::pair<int, int> e : d1.edges()) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
    for (int v : d1.vertices()) {
        std::cout << v << std::endl;
    }
}

TEST(Digraph_CustomTest, checkRemoveEdgeFunc)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);

    d1.addEdge(1, 2, 5.0);
    d1.addEdge(2, 3, 17.0);
    d1.addEdge(3, 1, 100);
    d1.addEdge(3, 2, 101);
    d1.addEdge(3, 3, 102);

    for (std::pair<int, int> e : d1.edges()) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
    for (int v : d1.vertices()) {
        std::cout << v << std::endl;
    }

    d1.removeEdge(3, 1);
    d1.removeEdge(1, 2);

    ASSERT_TRUE(d1.vertices().size() == 3);
    ASSERT_TRUE(d1.edges().size() == 3);
    for (std::pair<int, int> e : d1.edges()) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
    for (int v : d1.vertices()) {
        std::cout << v << std::endl;
    }
}

TEST(Digraph_CustomTest, checkVertexAndEdgeCountFunc)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);

    d1.addEdge(1, 2, 5.0);
    d1.addEdge(2, 3, 17.0);
    d1.addEdge(2, 2, 14.0);
    d1.addEdge(3, 1, 100);
    d1.addEdge(3, 2, 101);
    d1.addEdge(3, 3, 102);

    for (std::pair<int, int> e : d1.edges()) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
    for (int v : d1.vertices()) {
        std::cout << v << std::endl;
    }

    ASSERT_TRUE(d1.vertexCount() == 3);
    std::cout << "VertexCout: " << d1.vertexCount() << std::endl;
    ASSERT_TRUE(d1.edgeCount() == 6);
    std::cout << "EdgeCount: " << d1.edgeCount() << std::endl;
    ASSERT_TRUE(d1.edgeCount(3) == 3);

    d1.removeVertex(3);
    d1.removeEdge(1, 2);

    for (std::pair<int, int> e : d1.edges()) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
    for (int v : d1.vertices()) {
        std::cout << v << std::endl;
    }

    ASSERT_TRUE(d1.vertexCount() == 2);
    std::cout << "VertexCout: " << d1.vertexCount() << std::endl;
    ASSERT_TRUE(d1.edgeCount() == 1);
    std::cout << "EdgeCount: " << d1.edgeCount() << std::endl;
}

TEST(Digraph_CustomTest, checkConnectednessFunc)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);
    d1.addVertex(4, 40);

    d1.addEdge(1, 2, 5.0);
    d1.addEdge(2, 3, 17.0);
    d1.addEdge(3, 1, 100);
    d1.addEdge(4, 1, 105);

    for (std::pair<int, int> e : d1.edges()) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
    for (int v : d1.vertices()) {
        std::cout << v << std::endl;
    }

    ASSERT_FALSE(d1.isStronglyConnected());

    d1.addEdge(3, 4, 150);

    for (std::pair<int, int> e : d1.edges()) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
    for (int v : d1.vertices()) {
        std::cout << v << std::endl;
    }

    ASSERT_TRUE(d1.isStronglyConnected());
}

TEST(Digraph_CustomTest, checkShortestPathsFunc)
{
    Digraph<int, double> d1;
    d1.addVertex(1, 10);
    d1.addVertex(2, 20);
    d1.addVertex(3, 30);
    d1.addVertex(4, 40);


    d1.addEdge(1, 2, 10.0);
    d1.addEdge(2, 3, 20.0);
    d1.addEdge(3, 3, 20.0);
    d1.addEdge(3, 4, 10.0);
    d1.addEdge(4, 3, 10.0);
    d1.addEdge(2, 4, 5.0);
    d1.addEdge(1, 4, 17.0);
    d1.addEdge(4, 1, 100.0);


    std::map<int, int> paths = d1.findShortestPaths(
            1,
            [](double edgeInfo)
            {
                return edgeInfo;
            });

    for (std::pair<int, int> p : paths) {
        std::cout << "From " << 1 << " to " << p.first << ", Predecessor: " << p.second << std::endl;
    }

    ASSERT_EQ(4, paths.size());

    ASSERT_TRUE(paths.find(1) != paths.end());
    ASSERT_TRUE(paths.find(2) != paths.end());
    ASSERT_TRUE(paths.find(3) != paths.end());
    ASSERT_TRUE(paths.find(4) != paths.end());


    ASSERT_EQ(1, paths[1]);
    ASSERT_EQ(1, paths[2]);
    ASSERT_EQ(4, paths[3]);
    ASSERT_EQ(2, paths[4]);

}

