#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <locale>

using namespace std;

class Edge {
public:
    int node1;
    int node2;
    int weight;

    Edge(int n1, int n2, int w = 1) : node1(n1), node2(n2), weight(w) {}
};

class Node {
public:
    int id;
    vector<Edge*> edges;

    Node(int node_id) : id(node_id) {}
};

class TreeDiameterFinder {
private:
    unordered_map<int, Node*>& graph;

public:
    TreeDiameterFinder(unordered_map<int, Node*>& g) : graph(g) {}

    pair<int, int> findFurthestNode(int startNode) {
        if (graph.find(startNode) == graph.end()) {
            return { -1, -1 };
        }

        unordered_set<int> visited;
        queue<pair<int, int>> q;
        int furthestNode = startNode;
        int maxDistance = 0;

        q.push({ startNode, 0 });
        visited.insert(startNode);

        while (!q.empty()) {
            int currentNode = q.front().first;
            int distance = q.front().second;
            q.pop();

            if (distance > maxDistance) {
                maxDistance = distance;
                furthestNode = currentNode;
            }

            for (Edge* edge : graph[currentNode]->edges) {
                int neighbor = (edge->node1 == currentNode) ? edge->node2 : edge->node1;

                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push({ neighbor, distance + edge->weight });
                }
            }
        }

        return { furthestNode, maxDistance };
    }

    int findTreeDiameter(int startNode) {
        pair<int, int> firstBFS = findFurthestNode(startNode);
        int endpoint1 = firstBFS.first;

        if (endpoint1 == -1) return 0;

        pair<int, int> secondBFS = findFurthestNode(endpoint1);
        return secondBFS.second;
    }

    vector<vector<int>> findConnectedComponents() {
        vector<vector<int>> components;
        unordered_set<int> visited;

        for (auto& pair : graph) {
            int nodeId = pair.first;
            if (visited.find(nodeId) == visited.end()) {
                vector<int> component;
                bfsComponent(nodeId, visited, component);
                components.push_back(component);
            }
        }

        return components;
    }

    void bfsComponent(int startNode, unordered_set<int>& visited, vector<int>& component) {
        queue<int> q;
        q.push(startNode);
        visited.insert(startNode);
        component.push_back(startNode);

        while (!q.empty()) {
            int currentNode = q.front();
            q.pop();

            for (Edge* edge : graph[currentNode]->edges) {
                int neighbor = (edge->node1 == currentNode) ? edge->node2 : edge->node1;

                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    component.push_back(neighbor);
                    q.push(neighbor);
                }
            }
        }
    }

    vector<pair<vector<int>, int>> findForestDiameters() {
        vector<pair<vector<int>, int>> forestDiameters;
        vector<vector<int>> components = findConnectedComponents();

        for (auto& component : components) {
            if (!component.empty()) {
                int diameter = findTreeDiameter(component[0]);
                forestDiameters.push_back(make_pair(component, diameter));
            }
        }

        return forestDiameters;
    }

    int findMaxDiameter() {
        auto forestDiameters = findForestDiameters();
        int maxDiameter = 0;

        for (auto& pair : forestDiameters) {
            if (pair.second > maxDiameter) {
                maxDiameter = pair.second;
            }
        }

        return maxDiameter;
    }
};

void cleanupGraph(unordered_map<int, Node*>& graph) {
    for (auto& pair : graph) {
        for (Edge* edge : pair.second->edges) {
            delete edge;
        }
        delete pair.second;
    }
    graph.clear();
}

// 1. Пустой граф
unordered_map<int, Node*> createEmptyGraph() {
    return unordered_map<int, Node*>();
}

// 2. Граф с 3 изолированными вершинами
unordered_map<int, Node*> createThreeIsolatedNodes() {
    unordered_map<int, Node*> graph;
    for (int i = 1; i <= 3; i++) {
        graph[i] = new Node(i);
    }
    return graph;
}

// 3. Звездообразное дерево
unordered_map<int, Node*> createStarTree() {
    unordered_map<int, Node*> graph;

    for (int i = 1; i <= 6; i++) {
        graph[i] = new Node(i);
    }

    for (int i = 2; i <= 6; i++) {
        graph[1]->edges.push_back(new Edge(1, i));
        graph[i]->edges.push_back(new Edge(i, 1));
    }

    return graph;
}

// 4. Лес
unordered_map<int, Node*> createCustomForest() {
    unordered_map<int, Node*> graph;

    for (int i = 1; i <= 11; i++) {
        graph[i] = new Node(i);
    }

    graph[1]->edges.push_back(new Edge(1, 3));
    graph[3]->edges.push_back(new Edge(3, 1));

    graph[3]->edges.push_back(new Edge(3, 5));
    graph[5]->edges.push_back(new Edge(5, 3));

    graph[3]->edges.push_back(new Edge(3, 4));
    graph[4]->edges.push_back(new Edge(4, 3));

    graph[1]->edges.push_back(new Edge(1, 2));
    graph[2]->edges.push_back(new Edge(2, 1));

    graph[2]->edges.push_back(new Edge(2, 6));
    graph[6]->edges.push_back(new Edge(6, 2));

    graph[6]->edges.push_back(new Edge(6, 7));
    graph[7]->edges.push_back(new Edge(7, 6));

    // ДЕРЕВО 2: 10-9-8
    graph[10]->edges.push_back(new Edge(10, 9));
    graph[9]->edges.push_back(new Edge(9, 10));

    graph[9]->edges.push_back(new Edge(9, 8));
    graph[8]->edges.push_back(new Edge(8, 9));

    // ДЕРЕВО 3: 11 (изолированная вершина)

    return graph;
}

void testGraph(const string& testName, unordered_map<int, Node*>& graph) {
    cout << "=================================" << endl;
    cout << "ТЕСТ: " << testName << endl;
    cout << "=================================" << endl;

    if (graph.empty()) {
        cout << "Граф пустой" << endl << endl;
        return;
    }

    TreeDiameterFinder finder(graph);
    auto forestDiameters = finder.findForestDiameters();

    cout << "Найдено деревьев в лесу: " << forestDiameters.size() << endl;

    for (int i = 0; i < forestDiameters.size(); i++) {
        auto component = forestDiameters[i].first;
        int diameter = forestDiameters[i].second;

        cout << "Дерево №" << i + 1 << ":" << endl;
        cout << "  Вершины: ";
        for (int node : component) {
            cout << node << " ";
        }
        cout << endl;
        cout << "  Диаметр: " << diameter << " ребер" << endl;

        if (component.size() == 1) {
            cout << "  Тип: изолированная вершина" << endl;
        }
        cout << endl;
    }

    int maxDiameter = finder.findMaxDiameter();
    cout << "Максимальный диаметр в лесу: " << maxDiameter << " ребер" << endl;

    int totalVertices = graph.size();
    int totalEdges = 0;
    for (auto& pair : graph) {
        totalEdges += pair.second->edges.size();
    }
    totalEdges /= 2;

    cout << "Статистика:" << endl;
    cout << "  Вершин: " << totalVertices << endl;
    cout << "  Ребер: " << totalEdges << endl;
    cout << "  Компонент связности: " << forestDiameters.size() << endl;
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "ТЕСТИРОВАНИЕ АЛГОРИТМА ПОИСКА ДИАМЕТРА ЛЕСА" << endl;
    cout << "============================================" << endl << endl;

    auto graph1 = createEmptyGraph();
    testGraph("Пустой граф", graph1);
    cleanupGraph(graph1);

    auto graph2 = createThreeIsolatedNodes();
    testGraph("3 изолированные вершины", graph2);
    cleanupGraph(graph2);

    auto graph3 = createStarTree();
    testGraph("Звездообразное дерево (центр 1, лучи 2-6)", graph3);
    cleanupGraph(graph3);

    auto graph4 = createCustomForest();
    testGraph("Лес", graph4);
    cleanupGraph(graph4);

    cout << "=================================" << endl;
    cout << "ВСЕ ТЕСТЫ ЗАВЕРШЕНЫ!" << endl;
    cout << "=================================" << endl;

    return 0;
}