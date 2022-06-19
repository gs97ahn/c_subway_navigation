#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#define TRUE 1              // number for true value
#define FALSE 0             // number for false value
#define MAX_VERTICES 552    // number of total stations
#define INF 9999            // number for infinity value
#define SUBWAY_NUMBER 18    // number of total subway
#define TRANSFER_NUMBER 150 // number of total subway transfer
#define TRANSFER_CHARGE 100 // number for transfer charge

typedef struct GraphType
{
    int n;                                  // integer to hold total
    int weight[MAX_VERTICES][MAX_VERTICES]; // matrix to hold subway travel time
} GraphType;

int distance[MAX_VERTICES]; // distance from starting point to end point
int found[MAX_VERTICES];    // determine whether the subway station is found
int path[MAX_VERTICES];     // optimal path
int transfer[MAX_VERTICES]; // determine the minimum transfer

// character array for subway line names
char subway[SUBWAY_NUMBER][16] = {{"1지선"}, {"1호선"}, {"2지선"}, {"2호선"}, {"3호선"}, {"4호선"}, {"5지선"}, {"5호선"}, {"6호선"}, {"7호선"}, {"8호선"}, {"9호선"}, {"경의선"}, {"경춘선"}, {"공항철도"}, {"분당선"}, {"인천1선"}, {"중앙선"}};
char station_code[MAX_VERTICES][16];             // code of specific subway and station
char station_name[MAX_VERTICES][32];             // name of stations
char transfer_station_code[TRANSFER_NUMBER][16]; // transfer information
char subway_name[MAX_VERTICES][16];              // name of subway line name

int total_subway_time = 0;   // total travel time in subway
int total_transfer_time = 0; // total transfer time during travel
int total_station = 0;       // total stations visited
int did_transfer = 0;        // total transfers done

// choose an index position that has not ben found and is the minimum distance
// @param distance: integer array that stores distance from the starting point to end point
// @param n: integer that stores total number of distance array
// @param found: integer array that stores true or false value to determine whether a specific index is already found previously
// @return : minimum index position
int choose(int dinstance[], int n, int found[])
{
    int i, min, minpos;
    min = INT_MAX;
    for (i = 0; i < n; i++)
        if (distance[i] < min && !found[i])
        {
            min = distance[i]; // store newly minimum distance
            minpos = i;        // store position of newly minimum distance
        }
    return minpos;
}

// find the shortest path
// @param g: graph structure that stores information about subway travel
// @param start: integer that stores the starting index value
void shortest_path(GraphType *g, int start)
{
    int i, u, w;
    for (i = 0; i < g->n; i++) // reset distance and found array
    {
        distance[i] = g->weight[start][i];
        found[i] = FALSE;
    }
    found[start] = TRUE; // starting index position does not need be found
    distance[start] = 0; // starting index position is set to 0
    for (i = 0; i < g->n - 1; i++)
    {
        u = choose(distance, g->n, found);
        found[u] = TRUE; // set the chosen index position found value to true
        for (w = 0; w < g->n; w++)
            if (!found[w])                                       // check for unfound index position
                if (distance[u] + g->weight[u][w] < distance[w]) // check for the shortest distance
                {
                    distance[w] = distance[u] + g->weight[u][w]; // set the newly found value which is currently the shortest
                    path[w] = u;                                 // set the newly found path value which is currently the shortest
                }
    }
}

// find the minimum transfer path
// @param g: graph structure that stores information about subway travel
// @param start: integer that stores the starting index value
void minimum_transfer_path(GraphType *g, int start)
{
    int i, u, w;
    for (i = 0; i < g->n; i++) // reset distance, transfer, and found array
    {
        distance[i] = g->weight[start][i];
        transfer[i] = g->weight[start][i];
        found[i] = FALSE;
    }
    found[start] = TRUE; // starting index position does not need be found
    distance[start] = 0; // starting index poisition is set to 0
    transfer[start] = 0; // starting index poisition is set to 0
    for (i = 0; i < g->n - 1; i++)
    {
        u = choose(distance, g->n, found);
        found[u] = TRUE; // set the chosen index position found value to true
        for (w = 0; w < g->n; w++)
            if (!found[w])                                       // check for unfound index position
                if (distance[u] + g->weight[u][w] < distance[w]) // check for the shortest distance
                {
                    if (strcmp(subway_name[u], subway_name[w]) != 0) // check for subway transfer occurance
                    {
                        distance[w] = distance[u] + g->weight[u][w] + TRANSFER_CHARGE; // add transfer charger to avoid transfer charge
                        path[w] = u;                                                   // set newly found path value which is currently the shortest
                        continue;                                                      // skip the rest of the code if transfer occured
                    }
                    distance[w] = distance[u] + g->weight[u][w]; // set the newly found value which is currently the shortest
                    path[w] = u;                                 // set the newly found path value which is currently the shortest
                }
    }
}

// initialize graph by setting the matrix in graph to infinity
// @param g: graph structure that stores information about subway travel
void init_graph(GraphType *g)
{
    for (int i = 0; i < MAX_VERTICES; i++)     // visit every row
        for (int j = 0; j < MAX_VERTICES; j++) // vist every column
            g->weight[i][j] = INF;             // set the specific matrix value to inifinity
}

// generate random number from 1 to max
// @param max: integer that stores max number that can be generated
// @return : random number
int generate_random_number(int max)
{
    srand(time(0));
    return (rand() % max + 1);
}

// print path travelled by recursion
// @param g: graph structure that stores information about subway travel
// @param start: integer that stores the starting index value
// @param end: integer that stores the ending index value
void print_path(GraphType *g, int start, int end)
{
    if (path[start] == path[end]) // check for recursion reaching the departure station
    {
        if (strcmp(station_name[start], station_name[end]) == 0) // check to avoid transfer in departure station
            return;
        printf("-><%s> %s\n", subway_name[start], station_name[start]);
        total_station += 1;                         // add to total station visited
        total_subway_time += g->weight[start][end]; // add to total subway travel time
        return;
    }
    print_path(g, start, path[end]);                             // recursive call to get to the depature station
    if (strcmp(station_name[path[end]], station_name[end]) == 0) // check for transfer occurance
    {
        int random_transfer_time = generate_random_number(g->weight[path[end]][end]); // generate random number which occur during transfer
        total_transfer_time += random_transfer_time;                                  // add to total transfer time
        printf("-><환승 : 소요시간 %d 분> %s\n", random_transfer_time, station_name[path[end]]);
        did_transfer = 1; // set the transfer to true
    }
    else
    {
        if (did_transfer == 1) // check for transfer occurance previously
        {
            did_transfer = 0;                               // reset the transfer to false
            total_station += 1;                             // add to total station visited
            total_subway_time += g->weight[path[end]][end]; // add to total subway travel time
            return;
        }
        printf("-><%s> %s\n", subway_name[path[end]], station_name[path[end]]);
        total_station += 1;                             // add to total station visited
        total_subway_time += g->weight[path[end]][end]; // add to total subway travel time
    }
}

// read csv related to subway data to get travel time in these files:
// 1지선.csv, 1호선.csv, 2지선.csv, 2호선.csv, 3호선.csv,
// 4호선.csv, 5지선.csv, 5호선.csv, 6호선.csv, 7호선.csv,
// 8호선.csv, 9호선.csv, 경의선.csv, 경춘선.csv, 공항철도.csv,
// 분당선.csv, 인천1선.csv, 중앙선.csv
// @param g: graph structure that stores information about subway travel
void read_subway_csv(GraphType *g)
{
    int row, column;     // counter for row and column
    int cnt = 0;         // used to reset counter row and column
    g->n = MAX_VERTICES; // set csv

    for (int i = 0; i < SUBWAY_NUMBER; i++) // loop through every csv file
    {
        char file_path[64] = "./data/";   // file folder
        strcat(file_path, subway[i]);     // file name
        strcat(file_path, ".csv");        // csv file extension
        FILE *fp = fopen(file_path, "r"); // open file

        if (!fp) // check for file read error
            printf("Error: Unable to open %s\n", file_path);
        else
        {
            char buffer[2048];   // store entire row of csv
            row = cnt;           // reset row
            column = cnt;        // reset column
            char *line, *record; // point to line and single data in a line

            line = fgets(buffer, sizeof(buffer), fp); // skip the first row

            while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL) // loop until no more row
            {
                column = cnt;                      // reset column
                record = strtok(line, ",");        // get station code
                strcpy(station_code[row], record); // set station code

                record = strtok(NULL, ","); // get subway travel time
                while (record)              // loop until no more column
                {
                    g->weight[row][column] = atoi(record); // set subway travel time
                    record = strtok(NULL, ",");            // get next column's subway travel time
                    column++;                              // increment column
                }
                row++;                               // increment row
                strcpy(subway_name[row], subway[i]); // set station name
            }
            cnt = row; // reset counter
        }
        fclose(fp); // close file
    }
}

// read csv related to subway name and subway code in 역이름.csv to get station names
// @param g: graph structure that stores information about subway travel
void read_station_name_csv(GraphType *g)
{
    char file_path[] = "./data/역이름.csv"; // file path including folder and file name
    FILE *fp = fopen(file_path, "r");       // open file

    if (!fp) // check for file read error
        printf("Error: Unable to open %s\n", file_path);
    else
    {
        char buffer[64];     // store entire row of csv
        char *line, *record; // point to line and single data in a line

        line = fgets(buffer, sizeof(buffer), fp); // skip the first row

        while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL) // loop until no more row
        {
            record = strtok(line, ",");            // get station code
            for (int i = 0; i < MAX_VERTICES; i++) // loop thorugh the entire station list
            {
                if (strcmp(station_code[i], record) == 0) // check for matching station code
                {
                    record = strtok(NULL, ",");        // get station name
                    record[strcspn(record, "\n")] = 0; // get rid of \n in the end of station name
                    strcpy(station_name[i], record);   // set station name
                    break;
                }
            }
        }
    }
}

// read csv related to subway transfer data to get transfer station code in 환승정보.csv
void read_transfer_station_code_csv()
{
    int row = 0; // counter for row

    char file_path[] = "./data/환승정보.csv"; // file path including the folder and file name
    FILE *fp = fopen(file_path, "r");         // open file

    if (!fp) // check for file read error
        printf("Error: Unable to open %s\n", file_path);
    else
    {
        char buffer[2048];   // store entire row of csv
        char *line, *record; // point to line and single data in a line

        line = fgets(buffer, sizeof(buffer), fp); // skip the first row

        while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL) // loop until no more row
        {
            record = strtok(line, ",");                   // get transfer station code
            strcpy(transfer_station_code[row++], record); // set transfer station code
        }
    }
}

// read csv related to subway transfer data to get transfer time in 환승정보.csv
// @param g: graph structure that stores information about subway travel
void read_transfer_csv(GraphType *g)
{
    int row = 0;    // counter for row
    int column = 0; // counter for column

    char row_station_code[16];    // store station code for current row
    char column_station_code[16]; // store station code for current column
    int row_station;              // store row index value for station
    int column_station;           // store column index value for station

    char file_path[] = "./data/환승정보.csv"; // file path including folder and file name
    FILE *fp = fopen(file_path, "r");         // open file

    if (!fp) // check for file read error
        printf("Error: Unable to open %s\n", file_path);
    else
    {
        char buffer[2048];   // store entire row of csv
        char *line, *record; // point to line and single data in a line

        line = fgets(buffer, sizeof(buffer), fp);                  // skip the first row
        while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL) // loop until no more row
        {
            column = 0;                 // reset column
            record = strtok(line, ","); // skip the first column
            record = strtok(NULL, ","); // get subway transfer time
            while (record)              // loop until no more column
            {
                if (atoi(record) != INF) // check to make sure the transfer time is not set to infinity
                {
                    strcpy(row_station_code, transfer_station_code[row]);       // get station code for specific row
                    strcpy(column_station_code, transfer_station_code[column]); // get station code for specific column

                    for (int i = 0; i < MAX_VERTICES; i++) // loop through the entire station list
                    {
                        if (strcmp(row_station_code, station_code[i]) == 0)    // check to find index value for specific row
                            row_station = i;                                   // set the row station's index value
                        if (strcmp(column_station_code, station_code[i]) == 0) // check to find index value for specific column
                            column_station = i;                                // set the column station's index value
                    }
                    g->weight[row_station][column_station] = atoi(record); // set subway transfer travel time
                }
                record = strtok(NULL, ","); // get next column's subway transfer travel time

                column++; // increment column
            }
            row++; // increment row
        }
    }
    fclose(fp); // close file
}

// get station index by looping through all stations
// @param station: character array for a station name
// @return : index value of specific station name
int get_station_index(char station[])
{
    for (int i = 0; i < MAX_VERTICES; i++)         // loop through the entire station list
        if (strcmp(station, station_name[i]) == 0) // check to find matching station name
            return i;                              // index value found success

    printf("Error: Unable to find station index\n");
    return -1; // index value found faile
}

// check to varify whether the station exist using the station name
// @param station: character array for a station name
// @return : 1 for exisiting station and 0 for non-existing station
int is_existing_station(char station[])
{
    for (int i = 0; i < MAX_VERTICES; i++)         // loop through the entire station list
        if (strcmp(station_name[i], station) == 0) // check to find matching station name
            return 1;                              // existing station

    return 0; // non-existing station name
}

// main function to execute the program
int main(void)
{
    GraphType g;
    init_graph(&g); // reset data to initialize

    // read every csv files and get essential data
    read_subway_csv(&g);
    read_station_name_csv(&g);
    read_transfer_station_code_csv();
    read_transfer_csv(&g);

    char departure_station[32];   // store departure station from user input
    char destination_station[32]; // store destination station from user input
    do
    {
        do
        {
            printf("출발역을 입력해주세요: ");
            scanf("%s", departure_station); // read user input and set as departure station

            // check for existing station
            if (is_existing_station(departure_station) == 1)
                break; // break the loop if it is a existing station

            printf("Error: Departure station does not exist\n"); // station that user inputted does not exist
        } while (1);

        do
        {
            printf("도착역을 입력해주세요: ");
            scanf("%s", destination_station); // read user input and set as destination station

            // check for existing station
            if (is_existing_station(destination_station) == 1)
                break; // break the loop if it is a existing station

            printf("Error: Destination station does not exist\n"); // station that user inputted does not exist
        } while (1);

        // check to make sure that departure and destination station does not match
        if (strcmp(departure_station, destination_station) != 0)
            break; // break the loop if departure and destination station does not match

        printf("Error: Departure station and destinatino station is same\n");
    } while (1);

    int departure_index = get_station_index(departure_station);     // get and set departure station index
    int destination_index = get_station_index(destination_station); // get and set destination station index

    int method; // store user's preferred method which is either shortest path or minimum transfer path

    do
    {
        printf("방식? 1. 최단경로 2. 최소환승\n:");
        scanf("%d", &method); // read user input and set as the user's preferred method

        if (method == 1) // shortest path
        {
            shortest_path(&g, departure_index); // get shortest path

            // print the shortest path for the specific travel
            printf("\n최단경로\n<출발>\n");
            print_path(&g, departure_index, destination_index);
            printf("-><%s> %s <도착>\n", subway_name[destination_index], station_name[destination_index]);
            total_station += 1;

            // print time for the specific travel
            printf("\n소요시간 : %d (%d + 환승소요시간 : %d) 분\n", total_subway_time + total_transfer_time, total_subway_time, total_transfer_time);

            // print the total number of station travell for the specific travel
            printf("정거장 수 : %d 개\n", total_station);

            break;
        }
        else if (method == 2) // minimum transfer path
        {
            minimum_transfer_path(&g, departure_index); // get minimum transfer path

            // print the minimum transfer path for the specific travel
            printf("\n최소환승\n<출발>\n");
            print_path(&g, departure_index, destination_index);
            printf("-><%s> %s <도착>\n", subway_name[destination_index], station_name[destination_index]);
            total_station += 1;

            // print time for the specific travel
            printf("\n소요시간 : %d (%d + 환승소요시간 : %d) 분\n", total_subway_time + total_transfer_time, total_subway_time, total_transfer_time);

            // print the total number of station travelled for the specific travel
            printf("정거장 수 : %d 개\n", total_station);

            break;
        }
        else // unavailable method
        {
            printf("Error: Given method is unavailable\n");
        }
    } while (1);

    return 0;
}