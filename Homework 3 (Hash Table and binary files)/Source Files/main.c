/*
This program will make a hash table binary file from a given text file and will allow a user to enter in a new person,
delete a person, or display all the records of the binary file.

Created by: David Nguyen
Date: 5/18/14

CIS 26B Advanced C
Delia Garbacea
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "verify_student_records.h"
#define  MAXTABLESIZE 50
#define BUFFERSIZE 100

typedef struct
{
    char lastname[20];
    char firstname[20];
    float amount;
    int student_id;
}STUDENTREC;

typedef struct
{
    int num_elements;
    STUDENTREC students[3];
}BUCKET;

FILE* Open (char* FileName);
FILE* Build_HashTable_File (void);
void Populate_HashTable_File (FILE* input, FILE* database);
int hashing (char* input);
void Add_To_HashTable (FILE* database);
void Delete_From_HashTable (FILE* database);
void Display_HashTable (FILE* database);

int main (int argc, char* argv[])
{
    //Local Declarations
    char choice;
    FILE* input;
    FILE* output;
    BUCKET test;

    //Statements
    printf("This program will read in a text file with a student's 4 digit ID number, their\n"
            "first and last name and the amount of money awarded two them to two decimal places.\n\n");
    input = Open(argv[1]);
    output =  Build_HashTable_File();
    Populate_HashTable_File(input, output);
    fclose(input);
    do
    {
        printf("Menu\n"
                "a. Add a student\n"
                "d. Delete a student\n"
                "l. List all records in the hashed table file\n"
                "q. Quit\nChoice: ");
        scanf("%c%*c", &choice);
        //while (getchar !='\n'); //FLUSH the buffer
        switch (choice)
        {
            case 'a': Add_To_HashTable(output); break;
            case 'd': Delete_From_HashTable(output); break;
            case 'l': Display_HashTable(output); break;
            case 'q': break;
            default: printf("That choice was not understood.\n");
        }
        printf("\n\n");
    } while (choice !='q');
    fclose (output);
    return 0;
}

/*
Open will open a file with the argv[1] parameter or failing that will try to open input.txt to
open a text file with student records/
Pre: The file name
Post: A valid file pointer will be returned
*/
FILE* Open (char* FileName)
{
    FILE* return_ptr;
    if ( !(return_ptr = fopen(FileName, "r")) )
    {
        if ( !(return_ptr = fopen("input.txt", "r")) )
        {
            printf("The file given through the argv parameter or \"input.txt\" could not be opened.\n");
            exit (1);
        }
    }
    return return_ptr;
}

/*
Build_HashTable_File will open up a binary file and write to it a hash table of buckets with the size of MAXTABLESIZE
Pre:
Post: A binary file will be opened and written to it a hash table
*/
FILE* Build_HashTable_File (void)
{
    FILE* return_ptr;
    BUCKET HashTable[MAXTABLESIZE]; //empty hash table
    int i;

    for (i = 0; i < MAXTABLESIZE; i++)
    {
        HashTable[i].num_elements = 0;
    }

    if ( !(return_ptr = fopen("database.dat", "w+b")) )
    {
        printf("The file binary file cannot be opened.\n");
        exit (1);
    }
    fwrite (HashTable, sizeof(BUCKET), MAXTABLESIZE, return_ptr); //write hash table into empty file
    rewind (return_ptr);
    return return_ptr;
}

/*
hashing will hash a character string to a unique index by summing the square of each character's ASCII value.
If the index is bigger than the last element, the index will be modded with the size of the array.
Pre: the character string
*/
int hashing (char* input)
{
    int index = 0;
    int string_len = strlen (input);
    int i;

    for (i = 0; i < string_len; i++)
    {
        index += (*input) * (*input);
        input++;
    }
    if (index >= MAXTABLESIZE)
        index %= MAXTABLESIZE;
    return index;
}

/*
Populate_HashTable_File will populate a hash table file by getting and validating a user's input, hashing the entry by
last name (it had more randomness than using the student ID), grabbing the corresponding bucket from the binary file,
updating the bucket, and rewriting it back to the file.
Pre: The file pointer to the text file containing student information and the file pointer to the binary file that will
hold the hash table
Post: The binary file will be populated with student data from the text file
*/
void Populate_HashTable_File (FILE* input, FILE* database)
{
    int student_id;
    int index;
    char firstname[20];
    char lastname[20];
    float amount;
    BUCKET read;

    while (fscanf(input, "%d %s %s %f", &student_id, lastname, firstname, &amount) !=EOF)
    {
        STUDENTREC temp = {.amount = amount, .student_id = student_id};         //initalize a temporary struct
        strcpy (temp.firstname, firstname); strcpy (temp.lastname, lastname);   //part of the initalization
        index = hashing(temp.lastname);

        //read the bucket file if bucket < 3 add into the buceket else discard
        fseek (database, index*sizeof (BUCKET), SEEK_SET);  //pointer is at the correct location
        fread (&read, sizeof (BUCKET), 1, database);        //pointer will move one record forward after this
        if (read.num_elements < 3)
        {
            read.students[read.num_elements] = temp;
            read.num_elements++;
            fseek (database, -1*sizeof (BUCKET), SEEK_CUR); //move back one record to the original record
            fwrite (&read, sizeof(BUCKET), 1, database);    //overwrite the old record
        }
        else
        {
            printf("Could not add %s %s", firstname, lastname );
        }
    }
    rewind(database);
    rewind(input);
    return;
}

/*
Add_To_HashTable will add a student to the text file from the console.
Pre: The binary file's file pointer
Post: The student will be added to the binary file or the console will
display that it had failed
*/
void Add_To_HashTable (FILE* database)
{
    STUDENTREC add;
    BUCKET update;
    char validated[BUFFERSIZE];
    int index;

    printf("Please enter a 4 digit ID for the student you are going to add: ");
    verify_wrapper(&verify_4_digit, "That is not a 4 digit id. Try again: ", validated);
    add.student_id = atoi(validated);

    printf("Please enter the first name for the student you are going to add (max 20 char): ");
    verify_wrapper(&verify_name, "That is not a valid first name. Try again: ", validated);
    strcpy (add.firstname, validated);

    printf("Please enter the last name for the student you are going to add (max 20 char):");
    verify_wrapper(&verify_name, "That is not a valid last name. Try again: ", validated);
    strcpy (add.lastname, validated);

    printf("Please enter the amount of aid for the student\n"
           "(minimium 10.00 and max 99999.99 and the number must have two decimal numbers): ");
    verify_wrapper(&verify_amount, "That is not a valid amount. Try again: ", validated);
    add.amount = atof(validated);

    index = hashing(add.lastname);
    fseek (database, index*sizeof (BUCKET), SEEK_SET);
    fread (&update, sizeof (BUCKET), 1, database);
    if (update.num_elements < 3)
    {
        update.students[update.num_elements] = add;
        update.num_elements++;
        fseek(database, -1*sizeof (BUCKET), SEEK_CUR);
        fwrite (&update, sizeof(BUCKET), 1, database);
        printf("%s %s is inserted at index %d\n", add.firstname, add.lastname, index);
    }
    else
    {
        printf("Could not add %s %s", add.firstname, add.lastname);
    }
    rewind(database);
    return;
}

/*
Delete_From_HashTable will search for a student matching all the fields provided and delete the person from the hash table
Pre: The binary file's file pointer
Post: The user specified will be deleted or it will be stated that the person does not exist
*/
void Delete_From_HashTable (FILE* database)
{
    BUCKET deleting;
    int index;
    int i = 0;
    int shift;
    char validated[BUFFERSIZE];
    int delete_student_id;
    char delete_firstname[20];
    char delete_lastname[20];

    printf("Please the 4 digit ID of the student you are going to delete: ");
    verify_wrapper(&verify_4_digit, "That is not a 4 digit id. Try again: ", validated);
    delete_student_id = atoi(validated);

    printf("Please enter the first name for the student you are going to delete\n(max 20 char): ");
    verify_wrapper(&verify_name, "That is not a valid first name. Try again: ", validated);
    strcpy (delete_firstname, validated);

    printf("Please enter the last name for the student you are going to delete\n(max 20 char): ");
    verify_wrapper(&verify_name, "That is not a valid last name. Try again: ", validated);
    strcpy (delete_lastname, validated);

    index = hashing(delete_lastname);
    fseek (database, index*sizeof (BUCKET), SEEK_SET);
    fread (&deleting, sizeof (BUCKET), 1, database);

    //find the index at which the student matching all the given fields given is at
    //until i goes out of bounds
    while ( i < deleting.num_elements &&
            (deleting.students[i].student_id != delete_student_id ||
            strcmp (deleting.students[i].lastname, delete_lastname) ||
            strcmp (deleting.students[i].firstname, delete_firstname)) )
    {
        i++;
    }
    if (i >= deleting.num_elements) //never found the name because the greatest index must always be one less than the size
    {
            printf("%s %s does not exist in the hash table.\n", delete_firstname, delete_lastname);
    }
    else //shift the array of STUDENTRECS to the left
    {
        for (; i < deleting.num_elements-1; i++)
            deleting.students[i] = deleting.students[i+1];
        deleting.num_elements--; //decrement the counter
        printf("%s %s was deleted", delete_firstname, delete_lastname);
        fseek(database, -1*sizeof (BUCKET), SEEK_CUR);  //go back one bucket to the original bucket
        fwrite (&deleting, sizeof(BUCKET), 1, database); //overwrite the altered bucket
    }
    rewind(database);
    return;
}

/*
Display_HashTable will display the entire contents of the hash table.
Pre: The file pointer to the binary file
Post: The table will be printed onto screen
*/
void Display_HashTable (FILE* database)
{
    BUCKET HashTable[MAXTABLESIZE];
    int i;
    int j;

    fread (HashTable, sizeof (BUCKET), MAXTABLESIZE, database);
    for (i = 0; i < MAXTABLESIZE; i++)
    {
        if (HashTable[i].num_elements > 0)
        for (j=0; j < HashTable[i].num_elements; j++)
            printf("%d %s %s %.2f\n", HashTable[i].students[j].student_id, HashTable[i].students[j].lastname,
                                      HashTable[i].students[j].firstname, HashTable[i].students[j].amount);
    }
    rewind (database);
    return;
}
