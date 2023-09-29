///Student Name : Noura Awni Jaber Manassra
///Student ID : 1212359
///Instructor : Dr. Anas Arram
///Section No.: 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CITIES 70
#define INFINITY 99999999

typedef struct {
    char name[25];
    int distance;
    int visited;
    int val;// It is used in the context of graph traversal algorithms to keep track of the previous or parent city visited during the traversal
} City;

int graph[CITIES][CITIES];// to deal with 2D arrays better since it won't take so much time
City cities[CITIES];//array of struct to add each one alone
int numCities = 0;//this will be used as the counter of the cities

int cityIndex(const char* Name) {
    //------>> This function is used to return the index of the current city
    for (int i = 0; i < numCities; i++) {
            //i used the string compare built in function to make sure if it exists or not
        if (strcmp(cities[i].name, Name) == 0) {
            return i;
        }
    }
    return -1;
}

void loadCities() {
    //----------------->>> The most important thing to read the data from a file
    FILE* file = fopen("cities.txt", "r"); //open the file which has the data
    char city1[25], city2[25], dis[25]; // these chars are temp just to save the data inside
    int distance;//temp int to save the data inside

    while (fscanf(file, "%s%s%s", city1, city2, dis) != EOF) {
        distance = atoi(dis);// convert the text into int

        int index1 = cityIndex(city1);// to check the index of the city later when its needed
        if (index1 == -1) {
            strncpy(cities[numCities].name, city1, sizeof(cities[numCities].name));//if it's -1 that means this city is new and should be inserted
            //-------->>> Note here that this way i create new city, where the weight is infinity, no any connection yet
            cities[numCities].distance = INFINITY;
            cities[numCities].visited = 0;
            cities[numCities].val = -1;
            index1 = numCities;
            numCities++;//it should be changing since this will be representing the number of cities :)
        }

        int index2 = cityIndex(city2); // here is the destenation city
        if (index2 == -1) {
                //--------->>> Repeat the steps as the previous one
            strncpy(cities[numCities].name, city2, sizeof(cities[numCities].name));
            cities[numCities].distance = INFINITY;
            cities[numCities].visited = 0;
            cities[numCities].val = -1;
            index2 = numCities;
            numCities++;
        }
              //------->> Actually this is the graph   data :)
        graph[index1][index2] = distance;
        ///---------------->>>>>> Here, since it's directed i commented the above line, if it's not directed I will keep it!
       // graph[index2][index1] = distance;
    }

    fclose(file);
}

int getCity() {
    //----->>>> this will return the closet city for the current one to get the shortest path possible!
    int mindis = INFINITY;
    int minind = -1;
    for (int i = 0; i < numCities; i++) {
        if (cities[i].visited == 0 && cities[i].distance < mindis) {
            mindis = cities[i].distance;
            minind = i;
        }
    }
    return minind;
}

void dij(const char* source) {
    //---------------------------------------------------------------------->> WHY TO USE IT ? & HOW?
    // First,calculates the shortest path from a given source city to all other cities in a graph
    //then,It starts by initializing the distance of the source city to 0 and marks it as visited. Then, it iterates through the remaining cities,
    //selecting the closest unvisited city and marking it as visited,For each unvisited city, it checks if there is a connection between the current city and the unvisited city.
    // If there is, it calculates the new distance from the source city to the unvisited city through the current city.
    //If this new distance is shorter than the recorded distance, it updates the distance and predecessor of the unvisited city.
    //The function continues this process until all cities have been visited or the shortest paths to all cities have been found.
    int sourceind = cityIndex(source);//it's important to get the index to get the distances
    if (sourceind == -1) { // Check if the source city is valid
        printf("Invalid source city.\n");
        return;
    }
    for (int i = 0; i < numCities; i++) {
    cities[i].distance = INT_MAX; // Set distance to a large value
    cities[i].visited = 0; // Mark as unvisited
    cities[i].val = -1; // Reset predecessor
}
    cities[sourceind].distance = 0;// Set the distance of the source city to 0
    for (int i = 0; i < numCities - 1; i++) {// Loop through all cities except the source city
        int currentCityIndex = getCity();// Get the index of the closest unvisited city
        if (currentCityIndex == -1) {// If no unvisited city is found, break the loop
            break;
        }
        cities[currentCityIndex].visited = 1; // Mark the current city as visited

        for (int j = 0; j < numCities; j++) { // Mark the current city as visited
            if (graph[currentCityIndex][j] != 0 && cities[j].visited == 0) {
               //---> If there is a connection between the current city and city 'j', and 'j' is unvisited,Calculate
               //the new distance from the source city to city 'j' via the current city
                int newDistance = cities[currentCityIndex].distance + graph[currentCityIndex][j];
               //---> If the new distance is shorter than the current recorded distance to city 'j',Update the distance of city 'j'
                if (newDistance < cities[j].distance) {
                    cities[j].distance = newDistance;
                    cities[j].val = currentCityIndex;//Update the predecessor of city 'j'
                }
            }
        }
    }
}
int z=0;
void printPath(int cityIndex, int x) {
     //--->>>> s a recursive function that prints the path from the source city to a given city by following the predecessor links stored in the cities array.

    if (cityIndex == -1) {
        return;
    }
    printPath(cities[cityIndex].val, x);
    int m = cities[cityIndex].distance ;
    int k = m-z;
    if (k < 0) k=0;
    printf("%s with distance %d... ", cities[cityIndex].name,k);
    z=cities[cityIndex].distance;
}

void fprintPath(int cityIndex) {
    FILE* file = fopen("shortest_distance.txt", "a");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    if (cityIndex == -1) {
        fclose(file);
        return;
    }
    int m = cities[cityIndex].distance ;
    int k = m-z;
    if (k < 0) k=-1*k;
    fprintf(file, "%s ... ", cities[cityIndex].name);
    fprintPath(cities[cityIndex].val); // Call recursively after printing the city name

    fclose(file);
}

void printShortestPath(const char* destinationCity) {
    int destinationIndex = cityIndex(destinationCity); // Get the index of the destination city
    if (destinationIndex == -1) { // Get the index of the destination city
        printf("Invalid destination city.\n");
        return;
    }
    int x = 0; // Initialize x to 0
    printf("\nThe best full path to %s:\n", destinationCity);
    printPath(destinationIndex, x); // Pass x as an argument
    printf("\nTotal distance: %d\n", cities[destinationIndex].distance);
}

void saveToFile(const char* destinationCity) {
    FILE* file = fopen("shortest_distance.txt", "a");//to print to the file correclty
    if (file == NULL) {
        printf("there is a problem\n");
        return ;
    }

    int destinationIndex = cityIndex(destinationCity);
    if (destinationIndex == -1) {
        printf("there is a problem with the city.\n");
        fclose(file);
        return;
    }

    fprintf(file, "is the best way to go to %s\n", destinationCity);
    fprintPath(destinationIndex);
    fprintf(file, "\ndistance: %d\n", cities[destinationIndex].distance);

    fclose(file);
}

void bfs(const char* source) {
    // Get the index of the source city
    int sourceind = cityIndex(source);

    // Check if the source city is valid
    if (sourceind == -1) {
        printf("Invalid source city.\n");
        return;
    }

    // Reset cities' properties
    for (int i = 0; i < numCities; i++) {
        // Mark all cities as unvisited
        cities[i].visited = 0;
        // Set the initial distance to -1
        cities[i].distance = -1;
        // Set the val to -1
        cities[i].val = -1;
    }

    // Set the distance and visited status of the source city
    cities[sourceind].distance = 0;
    cities[sourceind].visited = 1;

    // Initialize the queue
    int queue[CITIES];
    int front = 0, rear = 0;

    // Enqueue the source city
    queue[rear++] = sourceind;

    // BFS traversal
    while (front != rear) {
        // Dequeue the current city
        int currentind = queue[front++];
        int currentdis = cities[currentind].distance;

        // Explore neighboring cities
        for (int i = 0; i < numCities; i++) {
            // Check if there is an edge and the city has not been visited
            if (graph[currentind][i] != 0 && !cities[i].visited) {
                // Update the distance of the neighboring city
                cities[i].distance = currentdis + graph[currentind][i];
                // Set the val of the neighboring city
                cities[i].val = currentind;
                // Mark the neighboring city as visited
                cities[i].visited = 1;
                // Enqueue the neighboring city
                queue[rear++] = i;
            }
        }
    }
}

int main() {

FILE* filwe = fopen("shortest_distance.txt", "w");
FILE* file = fopen("cities.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        exit(1);
    }
     int x; //this will be used as the choices option from the user
    char sourceCity[100]; //this will be entered by the user
    char destinationCity[100]; //this will be entered by the user
    while (1) {
        printf("Welcone to my GPS system, please choose the option you need \n1. Load cities\n2. Enter source\n3. Enter destination\n4. Exit and save shortest distance\n");
        scanf("%d", &x);
        switch (x) {
            case 1:
                printf("Done :) \n");
                loadCities();
                break;
            case 2:
                loadCities();
                printf("Please enter the source city :)\n");
                scanf("%s", sourceCity);
                //----->>>> Here we will use the dijkstra technique :), since here the we calcualte the shortest path :)
                break;

            case 3:
                loadCities();
                printf("Please enter the destination city :) \n");
                scanf("%s", destinationCity);
                printf("\nFirst, this is using the Dijkstra Algorithm\n");
                dij(sourceCity);
                saveToFile(destinationCity);
                printShortestPath(destinationCity);
                printf("\nSecond, this is using the BFS Algorithm\n");
                bfs(sourceCity);
                saveToFile(destinationCity);
                // Print the shortest path to a destination city
                printShortestPath(destinationCity);
                break;

            case 4:

                exit(0);

            default:
                printf("Wrong Option.\n");
        }
    }

    return 0;
}
