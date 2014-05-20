/*
Project: Basketball Players
This program will prompt a user for a file with a list of basketball players and their scores for that month.
This information will be put into a circularly doubly linked list with a sentinel node. The program will display
the players and one of their scores from the top of the stack from A-Z and backwards from Z-A.  Then it will allow
a user to enter a player's name to get their average points per game. All the players, scores, and averages will be
put into an output file.

Created by: David Nguyen
Date: 04/27/14

Lab2 CIS 26B
Delia Garbacea
*/

//Preprocessor Directives
//#include "MemTracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "STACKNODE.h"
#include "LISTNODE.h"
#define FLUSH while (getchar() != '\n')
#define MAXNAMESIZE 30
#define DUMMY_TRAILER "~"

//Function Declarations
FILE* Open (char FileLocation[FILENAME_MAX]);
LISTNODE* Build_List_Players_From_File (FILE* input);
void Display_Alphabetical_And_StackTop_Score (LISTNODE* players);
void Display_Reverse_Alphabetical_And_StackTop_Score (LISTNODE* players);
void Search_Player_And_Find_Average (LISTNODE* players);
void Create_Ouput_File (LISTNODE* players, char FileLocation[FILENAME_MAX]);
void Clean_Up (LISTNODE* players);

int main(void)
{
    LISTNODE* players;
    char FileLocation[FILENAME_MAX] = "0";
    FILE* input;

    input = Open (FileLocation);
    players = Build_List_Players_From_File (input);
    fclose (input);
    Display_Alphabetical_And_StackTop_Score (players);
    Display_Reverse_Alphabetical_And_StackTop_Score (players);
    Search_Player_And_Find_Average (players);
    Create_Ouput_File (players, FileLocation);
    Clean_Up (players);
    return 0;
}

/*
Open will open an input file for reading based on the user's input. If the user did not enter anything, the
default is "players.txt"
Pre: An empty array to hold the file name
Post: A file pointer will be passed back by value and the array will have the file's name.
*/
FILE* Open (char FileLocation[FILENAME_MAX])
{
    FILE* input;

    printf("Please enter the file containing the basketball players\n"
           "and their scores:  ");
    scanf("%[^\n]", FileLocation);
    FLUSH;
    if ( !strcmp (FileLocation, "0") )
        strcpy (FileLocation, "players.txt");
    if ( !(input = fopen (FileLocation, "r")) )
    {
        printf("The file you specified does not exist. This program will now terminate.\n");
        exit(100);
    }
    return input;
}

/*
Build_List_Players_From_File will create a doubly circular linked list from the file that has the format:
LASTNAME, FIRSTNAME score (int)

and if there are multiple scores for one player, it will push the score in the player's stack. The list
will be in alphabetical order.
Pre: File pointer for the input file
Post: A doubly circular linked list pointer will be passed back.
*/
LISTNODE* Build_List_Players_From_File (FILE* input)
{
    int points;
    bool duplicate;
    char name[MAXNAMESIZE];
    char lastname[MAXNAMESIZE];
    LISTNODE* players;
    LISTNODE* find_duplicate;

    players = init_list();
    while ( fscanf(input, "%s %s %d", name, lastname, &points) != EOF )
    {
        strcat (name, " ");
        strcat (name, lastname);
        duplicate = insert(players, name, points);
        if (duplicate)
        {
            find_duplicate = players->forw; //make the pointer be the first non-sentinel element
            while ( strcmp (find_duplicate->name, name) ) //search for the pointer with the same name
            {
                find_duplicate = find_duplicate->forw;
            }
            find_duplicate->scores = push (find_duplicate->scores, points);
        }
    }
    return players;
}

/*
Display_Alphabetical_And_StackTop_Score will display all the players alphabetically and their latest score on the stack.
Pre: A pointer to the list's sentinel node
Post: All players and their latest score will be displayed on screen
*/
void Display_Alphabetical_And_StackTop_Score (LISTNODE* players)
{
    printf("Here is the list of players in alpahabetical order and their most recent score.\n\n");
    players = players->forw;
    while ( strcmp (players->name, DUMMY_TRAILER) )
    {

        printf("%s  %d\n", players->name, players->scores->points);
        players = players->forw;
    }
    printf("\n");
    return;
}

/*
Display_Reverse_Alphabetical_And_StackTop_Score will display all the players in reverse
alphabetical order and their latest score.
Pre: A pointer to the list's sentinel node
Post: All players will be printed with their latest score (in reverse alphabetical order)
*/
void Display_Reverse_Alphabetical_And_StackTop_Score (LISTNODE* players)
{
    printf("Here is the list of players in reverse alphabetical order and their most recent score.\n\n");
    players = players->back;
    while ( strcmp (players->name, DUMMY_TRAILER) )
    {

        printf("%s  %d\n", players->name, players->scores->points);
        players = players->back;
    }
    printf("\n");
    return;
}

/*
Search_Player_And_Find_Average will allow the user to search if a player exists in the list and give the user
the average of that player.  It will end when the player types "quit please".
Pre: Poiner to the list's sentinel node
Post: The status of the player will be displayed and their average score.
*/
void Search_Player_And_Find_Average (LISTNODE* players)
{
    char name[MAXNAMESIZE];
    char lastname[MAXNAMESIZE];
    int number_of_scores;
    int sum;
    float average;
    LISTNODE* walk;
    STACKNODE* walker;

    printf("Please enter a player's name in the format LASTNAME, FIRSTNAME\n"
           "to get his average or type \"quit please\" to exit:  ");
    scanf("%s %s", name, lastname);
    FLUSH;
    strcat (name, " ");
    strcat (name, lastname);
    while (strncmp (name, "quit", 4)) //user must enter two strings and the first string must be "quit"
    {
        walk = players->forw; //set walk to the first non-Sentinel element in the list
        while (strcmp (walk->name, name) < 0)
        {
                walk = walk->forw;
        }
        if (!strcmp (walk->name, name))  //go through the stack with the STACKNODE* walk and average the scores
        {
            number_of_scores = 0;
            sum = 0;
            walker = walk->scores;
            while (walker)
            {
                sum += walker->points;
                number_of_scores++;
                walker = walker->next;
            }
            average = (float)sum/number_of_scores;
            printf("Average is %f\n", average);
        }
        else
            printf("This person does not exist on the list\n");
        scanf("%s %s", name, lastname);
        FLUSH;
        strcat (name, " ");
        strcat (name, lastname);
    }
    return;
}

/*
Create_Ouput_File will make an output file that prints the player, all of their scores, and their average
score in a file with the same name as the input with "_out.txt" appended to it.
Pre: The pointer to the list's sentinel node and the string with the original file location
Post: An output file will have all hte players, scores, and averages
*/
void Create_Ouput_File (LISTNODE* players, char FileLocation[FILENAME_MAX])
{
    FILE* output;
    char concatenate[]= "_out.txt";
    int number_of_scores;
    int sum;
    float average;
    LISTNODE* walk = players->forw;
    STACKNODE* walker;

    strtok (FileLocation,".");
    strcat (FileLocation, concatenate);
    output = fopen (FileLocation, "w");
    while ( strcmp (walk->name, DUMMY_TRAILER) )
    {
        sum = 0;
        number_of_scores = 0;
        fprintf(output, "%s ", walk->name);
        walker = walk->scores;
        while (walker)
        {
            fprintf(output, "%d ", walker->points);
            sum += walker->points;
            number_of_scores++;
            walker = walker->next;
        }
        average = (float) sum / number_of_scores;
        fprintf(output, "%f\n", average);
        walk = walk->forw;
    }
    fclose (output);
    return;
}

/*
Clean_Up will free all dynamically allocated memory from the list and the stack in each of the list's node.
Pre: The pointer to the list's sentinel node.
Post: All dynamically allocated memory from the list and each of its node's stack is freed.
*/
void Clean_Up (LISTNODE* players)
{
    LISTNODE* delete;
    STACKNODE* walker;
    STACKNODE* delete_1;

    players = players->forw; //set players to its first non-sentinel node

    while (strcmp (players->name, DUMMY_TRAILER)) //while players != the sentinel node
    {
        walker = pop (&(players->scores)); // players->scores;
        while (walker)      //delete the stack for that LISTNODE
        {
            delete_1 = walker;
            walker = pop (&(players->scores));
            free (delete_1);
        }
        delete = players;
        players = players->forw;
        free (delete);
    }
    free (players);     //delete the sentinel node (it has no stack pointer)
    return;
}

/*
Output:

Please enter the file containing the basketball players
and their scores:  players.txt
Here is the list of players in alpahabetical order and their most recent score.

BIRD, KIM  22
JABBAR, ROB  0
MULLIN, JOHN  23
ROBINSON, JAKE  32

Here is the list of players in reverse alphabetical order and their most recent score.

ROBINSON, JAKE  32
MULLIN, JOHN  23
JABBAR, ROB  0
BIRD, KIM  22

Please enter a player's name in the format LASTNAME, FIRSTNAME
to get his average or type "quit please" to exit:  JABBAR, ROB
Average is 31.750000
quit please

Process returned 0 (0x0)   execution time : 8.609 s
Press ENTER to continue.
*/
