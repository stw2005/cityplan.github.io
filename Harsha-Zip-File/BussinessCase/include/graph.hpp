#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <limits>
#include <queue>
#include <functional>
#include <algorithm>
#include <type_traits>
#include "disjoint-set.hpp"
#include "heap.hpp"

// Implements a undirected graph data structure with numeric weights
template<typename T, typename = typename std::enable_if<(
    std::is_integral<T>::value or std::is_floating_point<T>::value
     or std::is_same<T, bool>::value)>::type>
class Graph {
private:
    std::vector<std::vector<T>> adj_matrix;
    unsigned int vertices;
    std::vector<std::tuple<INDEX, INDEX, T>> edge_list;

    // Sorts the edge list based on the given comparator
    void sort_edges(std::function<bool(T, T)>);

public:
    // Constructor
    Graph(unsigned int);

    // Returns the size of graph
    unsigned int size() const;

    // Adds edge to the adjacency matrix.
    void add_edge(INDEX, INDEX, T);

    // Returns the cost of the spanning tree
    std::pair<std::vector<std::tuple<INDEX, INDEX, T>>, T> make_spanning_tree(std::function<bool(T, T)> = std::function<bool(T, T)>(std::less<T>()));

    // Displays the adjacency matrix.
    void display() const;

    // Displays the edge list.
    void display_edge_list() const;

    // Traverses the graph level by level
    std::vector<long double> bfs();

    // Finds the single source shortest path
    std::vector<long double> Dijkstra_Algorithm(INDEX, std::function<bool(std::pair<INDEX, long double>, std::pair<INDEX, long double>)>);

};

template<typename T, typename numbers>
Graph<T, numbers>::Graph(unsigned int vertices): vertices(vertices) {
    adj_matrix = std::vector<std::vector<T>>(this->vertices, std::vector<T>(this->vertices, std::numeric_limits<T>::max()));
}

template<typename T, typename numbers>
unsigned int Graph<T, numbers>::size() const {
    return vertices;
}

template<typename T, typename numbers>
void Graph<T, numbers>::add_edge(INDEX a, INDEX b, T weight) {
    try {
        if (a < vertices and b < vertices) {
            adj_matrix[a][b] = adj_matrix[b][a] = weight;
            edge_list.push_back(std::make_tuple(a, b, weight));
        } else
            throw OutOfBoundException();
    } catch(const OutOfBoundException& e) {
        std::cerr << e.what() << '\n';
    }
}

template<typename T, typename numbers>
void Graph<T, numbers>::display() const {
    for (const std::vector<T>& x: adj_matrix) {
        for (const T& y: x)
            std::cout << y << '\t';
        std::cout << std::endl;
    }
}

template<typename T, typename numbers>
void Graph<T, numbers>::display_edge_list() const {
    for (const std::tuple<INDEX, INDEX, T>& x: edge_list) {
        std::cout << std::get<0>(x) << '\t'
                  << std::get<1>(x) << '\t' 
                  << std::get<2>(x) << std::endl;
    }
}

template<typename T, typename numbers>
void Graph<T, numbers>::sort_edges(std::function<bool(T, T)> comparator) {
    sort(edge_list.begin(), edge_list.end(),
     [&comparator](std::tuple<INDEX, INDEX, T>& a, std::tuple<INDEX, INDEX, T>& b) {
         return comparator(std::get<2>(a), std::get<2>(b)); 
        });
}

template<typename T, typename numbers>
std::pair<std::vector<std::tuple<INDEX, INDEX, T>>, T> Graph<T, numbers>::make_spanning_tree(std::function<bool(T, T)> comparator) {
    std::vector<std::tuple<INDEX, INDEX, T>> spanning_tree;
    T cost = T(0);
    sort_edges(comparator);
    DisjointSet set(vertices);
    for (std::tuple<INDEX, INDEX, T>& edge: edge_list) {
        INDEX a = std::get<0>(edge), b = std::get<1>(edge);
        if (!set.FIND(a, b)) {
            set.UNION(a, b);
            spanning_tree.push_back(edge);
            cost += adj_matrix[a][b];
        }
    }
    return std::make_pair(spanning_tree, cost);
}

template<typename T, typename numbers>
std::vector<long double> Graph<T, numbers>::bfs() {
    int n = size();
    std::vector<long double> impact(n, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (adj_matrix[i][j] != std::numeric_limits<T>::max())
                impact[i] += adj_matrix[i][j];
        }
    }
    return impact;
}

template<typename T, typename numbers>
std::vector<long double> Graph<T, numbers>::Dijkstra_Algorithm(INDEX source, std::function<bool(std::pair<INDEX,long double>, std::pair<INDEX,long double>)> comparator) {
    std::vector<long double> distances(size(), std::numeric_limits<double>::max());
    distances[source] = T(0);
    Heap<std::pair<INDEX, long double>> PQ = Heap<std::pair<INDEX, long double>>(size(), comparator);
    PQ.push(std::make_pair(source, 0));
    while (!PQ.empty()) {
        long double u_dist = PQ.front().second;
        INDEX u = PQ.front().first;
        PQ.pop();
        if (u_dist > distances[u])
            continue;
        for (INDEX v = 0; v < vertices; ++v) {
            if (adj_matrix[u][v] != std::numeric_limits<T>::max()) {
                long double weight = adj_matrix[u][v];
                long double alt = u_dist + weight;
                if (alt < distances[v]) {
                    distances[v] = alt;
                    PQ.push(std::make_pair(v, alt));
                }
            }
        }
    }
    return distances;
}

#endif