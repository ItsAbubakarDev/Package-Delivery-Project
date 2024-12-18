#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <climits>

using namespace std;

// Number of nodes in the graph
const int vertices = 11;

// Structure to represent a graph edge
struct Edge {
    int src;
    int dest;
    int distance;
    int time;
    string source_name;
    string destination_name;

    Edge(int src, int dest, int distance, int time, string source_name, string destination_name) {
        this->src = src;
        this->dest = dest;
        this->distance = distance;
        this->time = time;
        this->source_name = source_name;
        this->destination_name = destination_name;
    }
};

// Node structure for A* algorithm
struct Node {
    int id;
    double gCost; // Cost from start
    double fCost; // gCost + hCost
    Node* parent;

    Node(int id) : id(id), gCost(INT_MAX), fCost(INT_MAX), parent(nullptr) {}
};

// Comparison operator for priority queue
struct CompareNode {
    bool operator()(const Node* a, const Node* b) {
        return a->fCost > b->fCost;
    }
};

// Heuristic function (Euclidean distance)
double heuristic(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Function to create the graph
void createGraph(vector<Edge> graph[], int vertices) {
    graph[0].push_back(Edge(0, 1, 16, 40, "comsats", "alipur"));
    graph[0].push_back(Edge(0, 9, 17, 20, "comsats", "shamshabad"));

    graph[1].push_back(Edge(1, 0, 8, 15, "alipur", "comsats"));
    graph[1].push_back(Edge(1, 2, 8, 15, "alipur", "hostel city"));
    graph[1].push_back(Edge(1, 4, 2, 1, "alipur", "bluearea"));
    graph[1].push_back(Edge(1, 8, 17, 25, "alipur", "banigala"));
    graph[1].push_back(Edge(1, 9, 5, 10, "alipur", "shamshabad"));

    graph[2].push_back(Edge(2, 1, 8, 15, "hostel city", "alipur"));
    graph[2].push_back(Edge(2, 3, 9, 10, "hostelcity", "sadar"));

    graph[3].push_back(Edge(3, 2, 9, 10, "sadar", "hostelcity"));
    graph[3].push_back(Edge(3, 4, 3, 5, "sadar", "bluearea"));
    graph[3].push_back(Edge(3, 5, 23, 37, "sadar", "I8"));
    graph[3].push_back(Edge(3, 8, 5, 10, "sadar", "banigala"));

    graph[4].push_back(Edge(4, 1, 2, 1, "bluearea", "alipur"));
    graph[4].push_back(Edge(4, 3, 3, 5, "bluearea", "sadar"));

    graph[5].push_back(Edge(5, 3, 23, 37, "I8", "sadar"));
    graph[5].push_back(Edge(5, 6, 25, 40, "I8", "Air University"));
    graph[5].push_back(Edge(5, 8, 27, 50, "I8", "banigala"));

    graph[6].push_back(Edge(6, 5, 25, 40, "Air University", "I8"));
    graph[6].push_back(Edge(6, 7, 53, 70, "Air University", "Pieas"));

    graph[7].push_back(Edge(7, 6, 53, 70, "Pieas", "Air University"));
    graph[7].push_back(Edge(7, 8, 18, 30, "Pieas", "banigala"));
    graph[7].push_back(Edge(7, 9, 20, 30, "Pieas", "shamshabad"));
    graph[7].push_back(Edge(7, 10, 8, 13, "Pieas", "pwd"));

    graph[8].push_back(Edge(8, 1, 17, 25, "banigala", "alipur"));
    graph[8].push_back(Edge(8, 3, 5, 10, "banigala", "sadar"));
    graph[8].push_back(Edge(8, 5, 27, 50, "banigala", "I8"));
    graph[8].push_back(Edge(8, 7, 18, 30, "banigala", "Pieas"));

    graph[9].push_back(Edge(9, 0, 17, 20, "shamshabad", "comsats"));
    graph[9].push_back(Edge(9, 1, 5, 10, "shamshabad", "alipur"));
    graph[9].push_back(Edge(9, 7, 20, 30, "shamshabad", "Pieas"));

    graph[10].push_back(Edge(10, 7, 8, 13, "pwd", "Pieas"));
}

// A* Algorithm
void aStar(vector<Edge> graph[], unordered_map<int, pair<int, int>> nodeCoordinates, int start, int goal) {
    priority_queue<Node*, vector<Node*>, CompareNode> openList;
    vector<bool> closedList(vertices, false);
    vector<Node*> nodes(vertices);

    for (int i = 0; i < vertices; ++i) {
        nodes[i] = new Node(i);
    }

    Node* startNode = nodes[start];
    startNode->gCost = 0;
    startNode->fCost = heuristic(nodeCoordinates[start].first, nodeCoordinates[start].second,
                                 nodeCoordinates[goal].first, nodeCoordinates[goal].second);

    openList.push(startNode);

    while (!openList.empty()) {
        Node* currentNode = openList.top();
        openList.pop();

        if (currentNode->id == goal) {
            cout << "Path found: ";
            Node* temp = currentNode;
            while (temp) {
                cout << graph[temp->id][0].source_name <<"("<<temp->id<<")"<< "<--";
                
                temp = temp->parent;
            }
            cout << endl;
            return;
        }

        closedList[currentNode->id] = true;

        for (const Edge& edge : graph[currentNode->id]) {
            if (closedList[edge.dest]) continue;

            Node* neighbor = nodes[edge.dest];
            double tentativeG = currentNode->gCost + edge.distance;

            if (tentativeG < neighbor->gCost) {
                neighbor->gCost = tentativeG;
                neighbor->fCost = neighbor->gCost + heuristic(nodeCoordinates[neighbor->id].first,
                                                              nodeCoordinates[neighbor->id].second,
                                                              nodeCoordinates[goal].first,
                                                              nodeCoordinates[goal].second);
                neighbor->parent = currentNode;

                openList.push(neighbor);
            }
        }
    }

    cout << "No path found!" << endl;
}

int main() {
    vector<Edge> graph[vertices];
    createGraph(graph, vertices);

    // Define coordinates for heuristic (x, y)
    unordered_map<int, pair<int, int>> nodeCoordinates = {
        {0, {0, 0}}, {1, {2, 2}}, {2, {4, 4}}, {3, {6, 6}}, {4, {8, 8}}, {5, {10, 10}},
        {6, {12, 12}}, {7, {14, 14}}, {8, {16, 16}}, {9, {18, 18}}, {10, {20, 20}}
    };

    cout << "Running A* Algorithm..." << endl;
    aStar(graph, nodeCoordinates, 0, 5);

    return 0;
}