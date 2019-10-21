// Exam Number: Y3857872
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    main.c:
        - Main source file to demonstrate the control of a weighted network and Dijkstra's algorithm.
        - Fully dynamic to the number of lines in each data input file.
        - Built in exit points and error flags embedded within function and displayed in terminal.

        - User Network Functions:
            - networkConstructor / networkDestructor
            - addCity / removeCity
            - addPath / removePath
            - displayNetwork
            - displayConnections
            - dijkstra

        - Scans two tab-delimited data input files in the text format:
            - 'ukcitypaths.txt'        -> 'city1Name    city2Name   distanceBetweenCities'
                - Scans and adds both unique cities and paths between respective cities.
                - Unique cities and connections are displayed within the terminal.

            - 'ukdijkstrapairs.txt'    -> 'startCityName   endCityName'
                - Applies Dijkstra's algorithm between two cities that are present within 'citypaths.txt'.
                - Results are displayed within the terminal.

            - (.txt files MUST end with a NEW BLANK LINE and be in the same directory as the project,
                or with a directory manually set below)

        - A copy of the results are printed to the terminal and written to a 'dijkstraresults.txt text file
            in the project directory.

        - Stress test data input .txt files, including the global city network are included with the project and
            can be used and tested by replacing the data file directory names to:
            - 'stress_test_ukdijkstrapairs.txt'     -> 1,000 test iterations for Dijkstra using the UK city network.
            - 'stress_test_globalcitypaths.txt'     -> Global city network, 1,376 unique cities and 14,615 connections.
            - 'stress_test_globaldijkstrapairs.txt' -> 1,000 test iterations for Dijkstra using the global city network.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// Include network header file:
#include "network.h"


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                                    MAIN STRUCTURES
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    'ukCities' Structure Typedef:
        - Custom array to scan data into from file directories.
*/
typedef struct {
    char city1Name[127];
    char city2Name[127];
    int distance;
} ukCities;

/*
    'dijkstraPairs' Structure Typedef:
        - Custom array to scan data into from file directories.
*/
typedef struct {
    char cityStartName[127];
    char cityEndName[127];
} dijkstraPairs;


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                                    MAIN FUNCTION
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

int main(void)
{
    // Initialise variables.
    int i = 0;
    int ch = 0;
    int citypaths_lines = 0;
    int dijkstrapairs_lines = 0;
    double algorithmTime = 0;
    double dijkstraTotalTime = 0;

    // Define file pointers and cities array to scan and write to.
    FILE *citypaths_file;
    FILE *dijkstrapairs_file;
    FILE *dijkstraresults_file;


    /* OPEN DATA FILES */

        printf("\n*** OPENING DATA FILES ***\n");

        // Open 'citypaths.txt' for network construction:

        citypaths_file = fopen("ukcitypaths.txt", "r");

            // Flag error at exit point '1' if failure to open file 'citypaths.txt'.
            if (citypaths_file == NULL)
            {
                printf("Error: cannot open file 'citypaths.txt'! - (main.c)\n");
                exit(1); // Program exits if the file pointer returns NULL.

            } else printf("\n'citypaths.txt' successfully loaded!\n");

            // Scans and counts how many lines there are in 'citypaths.txt'.
            //  (Code referenced and adapted from Jerry Coffin - Reference [11])
            while (EOF != (ch = getc(citypaths_file)))
                if ('\n' == ch)
                    ++citypaths_lines;

            rewind(citypaths_file);

            // Flag error at exit point '2' if no data found in 'citypaths.txt'.
            if (citypaths_lines == 0)
            {
                printf("Error: No data found in 'citypaths.txt'! - (main.c)\n");
                exit(2);

            } else printf("\tLines of data = %d\n", citypaths_lines);

            // Create 'cityPaths' array of correct size.
            ukCities cityPaths[citypaths_lines - 1];

        // Open 'dijkstrapairs.txt' for Dijkstra routes to test:

        dijkstrapairs_file = fopen("ukdijkstrapairs.txt", "r");

            // Flag error at exit point '3' if failure to open file 'dijkstrapairs.txt'.
            if (dijkstrapairs_file == NULL)
            {
                printf("Error: cannot open file 'dijkstrapairs.txt'! - (main.c)\n");
                exit(3); // Program exits if the file pointer returns NULL.

            } else printf("\n'dijkstrapairs.txt' successfully loaded!\n");

            // Scans and counts how many lines there are in 'dijkstrapairs.txt'.
            //  (Code referenced and adapted from Jerry Coffin - Reference [11])
            while (EOF != (ch = getc(dijkstrapairs_file)))
                if ('\n' == ch)
                    ++dijkstrapairs_lines;

            rewind(dijkstrapairs_file);

            // Flag error at exit point '4' if no data found in 'dijkstrapairs.txt'.
            if (dijkstrapairs_lines == 0)
            {
                printf("Error: No data found in 'dijkstrapairs.txt'! - (main.c)\n");
                exit(4);

            } else printf("\tLines of data = %d\n", dijkstrapairs_lines);

            // Create 'dijkstraPairs' array of correct size.
            dijkstraPairs dijkstraPairs[dijkstrapairs_lines - 1];

        // Open 'dijkstraresults.txt' for Dijkstra results to be written:

        dijkstraresults_file = fopen("dijkstraresults.txt", "w");

            // Flag error at exit point '5' if failure to open file 'dijkstraresults.txt'.
            if (dijkstraresults_file == NULL)
            {
                printf("Error: cannot open file 'dijkstraresults.txt'! - (main.c)\n");
                exit(5); // Program exits if the file pointer returns NULL.

            } else printf("\n'dijkstraresults.txt' successfully loaded!\n");


    /* CONSTRUCT NETWORK */

        // Construct network 'cityNetwork'.
        struct Network* cityNetwork = networkConstructor();

        for (i = 0; i < citypaths_lines; i++)
        {
            // Scan each line of 'citypaths.txt' to cityPaths array.
            while (3 == fscanf(citypaths_file, " %s %s %d", cityPaths[i].city1Name, cityPaths[i].city2Name, &cityPaths[i].distance))
            {
                // Add cities from 'citynetwork' array to network.
                addCity(cityNetwork, cityPaths[i].city1Name);
                addCity(cityNetwork, cityPaths[i].city2Name);

                // Add edge between cities to network.
                addPath(cityNetwork, cityPaths[i].city1Name, cityPaths[i].city2Name, cityPaths[i].distance);
            }
        }

    /* SELF CHECKING ERROR FLAGS */

        printf("\n*** TESTING ERROR FLAGS - START ***\n");

        // Testing 'addCity' error flags.
        printf("\nTesting 'addCity' function error path:\n");
        addCity(cityNetwork, "York");                               // City already present.

        // Testing 'addPath' error flags.
        printf("\nTesting 'addPath' function error paths:\n");
        addPath(cityNetwork, "Winchester", "York", 100);            // City1 not present.
        addPath(cityNetwork, "Edinburgh", "Falmouth", 100);         // City2 not present.
        addPath(cityNetwork, "Exeter", "Bath", 100);                // Both cities not present.
        addPath(cityNetwork, "Manchester", "Glasgow", -14);         // Invalid distance.

        // Testing 'removeCity' error flags.
        printf("\nTesting 'removeCity' function error paths:\n");
        removeCity(cityNetwork, "Brighton");                        // City not present.

        // Testing 'removePath' error flags.
        printf("\nTesting 'removePath' function error paths:\n");
        removePath(cityNetwork, "Winchester", "York");              // City1 not present.
        removePath(cityNetwork, "Edinburgh", "Aberdeen");           // City2 not present.
        removePath(cityNetwork, "Exeter", "Bath");                  // Both cities not present.
        removePath(cityNetwork, "Reading", "Glasgow");              // Path not present.

        // Testing 'displayConnections' error flags.
        printf("\nTesting 'displayConnections' function:\n");
        displayConnections(cityNetwork, "Aberdeen");                // City not present.
        displayConnections(cityNetwork, "Birmingham");              // Test display 1.
        displayConnections(cityNetwork, "York");                    // Test display 2.

        // Testing 'dijkstra' error flags.
        printf("\nTesting 'dijkstra' function error paths:\n");
        fprintf(dijkstraresults_file, "Testing 'dijkstra' function error paths:");
        dijkstra(dijkstraresults_file, &algorithmTime, cityNetwork, "Winchester", "York");                // City1 not present.
        dijkstra(dijkstraresults_file, &algorithmTime, cityNetwork, "Edinburgh", "Aberdeen");             // City2 not present.
        dijkstra(dijkstraresults_file, &algorithmTime, cityNetwork, "Southampton", "London");             // Both cities not present.
        printf("\n");

        // Add a city with no paths to test an unreachable path scenario.
        addCity(cityNetwork, "Coventry");
        displayConnections(cityNetwork, "Coventry");                                      // Test display city with no connections.
        dijkstra(dijkstraresults_file, &algorithmTime, cityNetwork, "Edinburgh", "Coventry");             // Test unreachable path.
        printf("\n");

        removeCity(cityNetwork, "Coventry");

        printf("\n*** TESTING ERROR FLAGS - COMPLETE ***\n");


    /* USER CODE (OPTIONAL) - START */

        printf("\n*** USER CODE ***\n");



    /* USER CODE (OPTIONAL) - END */


    /* DIJKSTRA'S ALORITHM */

        printf("\n*** DIJKSTRA'S ALGORITHM - START ***");
        fprintf(dijkstraresults_file, "\n*** DIJKSTRA'S ALGORITHM - START ***");

        // Print complete network.
        displayNetwork(cityNetwork);

        for (i = 0; i < dijkstrapairs_lines; i++)
        {
            // Scan each line of 'dijkstrapairs.txt' to dijkstraPairs array.
            while (2 == fscanf(dijkstrapairs_file, " %s %s", dijkstraPairs[i].cityStartName, dijkstraPairs[i].cityEndName))
            {
                // Find shortest paths between two cities from dijkstraPairs array
                dijkstra(dijkstraresults_file, &algorithmTime, cityNetwork, dijkstraPairs[i].cityStartName, dijkstraPairs[i].cityEndName);

                // Add algorithm computation time to the total time (used to find mean average computation time).
                dijkstraTotalTime = dijkstraTotalTime + algorithmTime;
            }
        }

        printf("\n(%d Iterations - Time Duration %fs)\n", dijkstrapairs_lines, dijkstraTotalTime);
        fprintf(dijkstraresults_file, "\n(%d Iterations - Time Duration %fs)\n", dijkstrapairs_lines, dijkstraTotalTime);
        printf("\n*** DIJKSTRA'S ALGORITHM - COMPLETE ***\n");
        fprintf(dijkstraresults_file, "\n*** DIJKSTRA'S ALGORITHM - COMPLETE ***\n");


    /* END MAIN.C */

        // Close data and result files.
        fclose(citypaths_file);
        fclose(dijkstrapairs_file);
        fclose(dijkstraresults_file);

        // Free network memory.
        networkDestructor(cityNetwork);

        // Display empty network.
        printf("\nTest Display - Empty Network:");
        displayNetwork(cityNetwork);

        printf("\n*** PROGRAM FINISHED ***\n");

        // Terminate main function.
        return 0;
}
