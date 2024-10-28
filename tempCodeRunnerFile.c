#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    Node** adjLists;
    int* visited;
} Graph;

Node* createNode(int v) {
    Node* newNode = malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int vertices) {
    Graph* graph = malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(Node*));
    graph->visited = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }

    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// Check if graph is complete
int isComplete(Graph* graph) {
    int vertices = graph->numVertices;
    for (int i = 0; i < vertices; i++) {
        int degree = 0;
        Node* temp = graph->adjLists[i];
        while (temp) {
            degree++;
            temp = temp->next;
        }
        if (degree != vertices - 1)
            return 0;
    }
    return 1;
}

// Obtain degree of a node
int degree(Graph* graph, int vertex) {
    int degree = 0;
    Node* temp = graph->adjLists[vertex];
    while (temp) {
        degree++;
        temp = temp->next;
    }
    return degree;
}

// Check if there is a path between two vertices
int isPathExists(Graph* graph, int startVertex, int endVertex) {
    if (startVertex == endVertex) return 1;
    
    graph->visited[startVertex] = 1;
    Node* temp = graph->adjLists[startVertex];
    
    while (temp) {
        int adjVertex = temp->vertex;
        if (!graph->visited[adjVertex]) {
            if (isPathExists(graph, adjVertex, endVertex))
                return 1;
        }
        temp = temp->next;
    }
    return 0;
}

// Breadth-First Traversal
void BFT(Graph* graph, int startVertex) {
    int queue[MAX], front = 0, rear = 0;
    int distance[MAX];
    for (int i = 0; i < graph->numVertices; i++) {
        graph->visited[i] = 0;
        distance[i] = -1;
    }

    graph->visited[startVertex] = 1;
    distance[startVertex] = 0;
    queue[rear++] = startVertex;

    printf("BFT Order: ");
    while (front < rear) {
        int currentVertex = queue[front++];
        printf("%d ", currentVertex);

        Node* temp = graph->adjLists[currentVertex];
        while (temp) {
            int adjVertex = temp->vertex;
            if (!graph->visited[adjVertex]) {
                graph->visited[adjVertex] = 1;
                distance[adjVertex] = distance[currentVertex] + 1;
                queue[rear++] = adjVertex;
            }
            temp = temp->next;
        }
    }
    printf("\nShortest distances from %d: ", startVertex);
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%d:%d ", i, distance[i]);
    }
    printf("\n");
}

// Depth-First Traversal
void DFTUtil(Graph* graph, int vertex, int* finishTime, int* time, int* hasCycle) {
    graph->visited[vertex] = 1;
    printf("%d ", vertex);

    Node* temp = graph->adjLists[vertex];
    while (temp) {
        int adjVertex = temp->vertex;
        if (!graph->visited[adjVertex]) {
            DFTUtil(graph, adjVertex, finishTime, time, hasCycle);
        } else if (graph->visited[adjVertex] == 1) {
            *hasCycle = 1;
        }
        temp = temp->next;
    }
    graph->visited[vertex] = 2;
    finishTime[vertex] = ++(*time);
}

void DFT(Graph* graph) {
    int finishTime[MAX];
    int time = 0;
    int hasCycle = 0;

    for (int i = 0; i < graph->numVertices; i++) {
        graph->visited[i] = 0;
    }

    printf("DFT Order: ");
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->visited[i] == 0) {
            DFTUtil(graph, i, finishTime, &time, &hasCycle);
        }
    }
    printf("\nCycle Present: %s\n", hasCycle ? "Yes" : "No");

    printf("Finishing times:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%d:%d ", i, finishTime[i]);
    }
    printf("\n");
}

int main() {
    int vertices = 4;
    Graph* graph = createGraph(vertices);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 0, 3);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);
    

    printf("Graph is %scomplete\n", isComplete(graph) ? "" : "not ");
    
    int u = 1;
    printf("Degree of node %d: %d\n", u, degree(graph, u));

    int v1 = 0, v2 = 4;
    printf("Path between %d and %d: %s\n", v1, v2, isPathExists(graph, v1, v2) ? "Yes" : "No");

    int startVertex = 0;
    BFT(graph, startVertex);
    DFT(graph);

    return 0;
}
