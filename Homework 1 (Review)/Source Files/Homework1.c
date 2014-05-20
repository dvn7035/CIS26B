/*
This program reads an input file for ASCII art and will save two files.  One file has a list of all artists, the number of entries they put,
and their art.  The second file has a list of the art with the highest rating and their artist.

David Nguyen
04/17/14

Delia Garbacea
CIS 26B Advanced C

Note to professor: I took out the extra white space on one of the names manually in the file. I assumed it was a mistake.

This is an updated source file that solves the heap corruption problem I had originally in my homework.  I could not free my art and name pointers because
when I allocated them with the strlen function in conjunction with the calloc function, I did not add one for the null terminator.  This successfully compiles
in Visual Studio 2012.
*/

//Preprocessor Director
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<stdbool.h>  (This is a change from the original because Visual Studio 2012 apparently does not have the stdbool library

#define FLUSH while (getchar() != '\n')
#define MAXARTSIZE 1600

typedef struct
{
    char* name;
    char* art;
    int rating;
}ART;

//Function declarations
FILE* Open (void);
void Fill_Array (ART** art_array, FILE* input_file, int* number_of_pieces);
void Sort_Array (ART** art_array, int number_of_pieces);
void Write_To_File_Highest_Rated (ART** art_array, int number_of_pieces);
void Write_To_File_Sorted_Art(ART** art_array, int number_of_pieces);
void Clean_Up(ART** art_array, int number_of_pieces);

int main (void)
{
    //Local Declarations
    FILE* input_file;
    ART** art_array = calloc(1000, sizeof(ART*));
    int number_of_pieces = 0;

    //Statements
    printf("This program reads an input file for ASCII art and will save two files.\n"
		   "One file has a list of all artists, the number of entries they put, and\n"
		   "their art. The second file has a list of the art with the highest rating\n"
		   "and their artist.\n");
    input_file = Open ();
	Fill_Array (art_array, input_file, &number_of_pieces);
	fclose(input_file);
	art_array = realloc (art_array, number_of_pieces*sizeof(ART*));
	Sort_Array(art_array, number_of_pieces);
	Write_To_File_Highest_Rated(art_array, number_of_pieces);
	Write_To_File_Sorted_Art(art_array, number_of_pieces);
	Clean_Up(art_array, number_of_pieces);
    return 0;
}

/*
Open will open up a file with the specified name and validate if in fact the file exists
Pre: void
Post: File will be opened and file pointer will be passed back by value
*/
FILE* Open (void)
{
    char file_name[FILENAME_MAX];
    FILE* art_input;

    printf("Please enter the file containing the ASCII art:  ");
    scanf("%s", file_name);
	FLUSH;
	//Confirm the input is correct
    if ( !(art_input = fopen (file_name, "r")) )
    {
        printf("Could not open the file you specified. The program will now close.\n");
        exit(101);
    }
    return art_input;
}

/*
Fill_Array will populate the ART** array with pointers to ART strctures and will fill in the ART structures based on
the file given.
Pre: The array of art pointers, the file pointer for the input file, and a pointer to hold the number of ART structures
Post: the array of art pointers will be initialized and the number of artworks will be returned
*/
void Fill_Array (ART** art_array, FILE* input_file, int* number_of_pieces)
{
	char temporary_buffer[MAXARTSIZE];

    while ( fscanf(input_file, "%[^\n]", temporary_buffer) != EOF )
    {
        //Allocate a new ART item
        ART* new_art = malloc (sizeof(ART));

		//Allocate fields and fill them in with strcpy
        new_art->name = calloc (strlen (temporary_buffer)+1, sizeof(char));  //(This is a change from the orignal source file)
		strcpy(new_art->name, temporary_buffer);
        fscanf (input_file, "%[^#]", temporary_buffer);
		new_art->art = calloc(strlen (temporary_buffer)+1, sizeof(char));    //(This is a change form the original source file)
		strcpy ( new_art->art, temporary_buffer);
        fscanf (input_file, "# %d\n", &(new_art->rating));
        *(art_array) = new_art;
		art_array++;
		++*(number_of_pieces);
    }
    return;
}

/*
Sort_Array will sort the array of art pointers by the artist's name in alpahabetical order.
Pre: The array of art pointers and the number of art pieces
Post:
*/
void Sort_Array (ART** art_array, int number_of_pieces)
{
	ART** current;
	ART** last_element = art_array + (number_of_pieces-1);
	ART** smallest;
	ART** walk;
	ART* tempData;

	//Selection Sort Algorithm
	for (current = art_array; current < last_element; current++)
	{
		smallest = current;
		for (walk = current + 1; walk <= last_element; walk++)
		{
			if ( strcmp((*walk)->name, (*smallest)->name) < 0 )
				smallest = walk;
		}
		tempData = *current;
		*current = *smallest;
		*smallest = tempData;
	}
	return;
}

/*
Write_To_File_Highest_Rated will write to a file the art work with the highest rating and its author.
Pre: The array of art pointers, the number of art pieces
Post: A file will be written with the art work with the higest rating and its author
*/
void Write_To_File_Highest_Rated (ART** art_array, int number_of_pieces)
{
	char FileNameOut[FILENAME_MAX];
	FILE* FileOut;
	ART** walk;
	ART** last_element = art_array + (number_of_pieces-1);
	int highest_rating = 0;

	//Make the output file
	printf("Please enter a name to save a file with the highest rated art pieces:  ");
	scanf("%s", FileNameOut);
	FLUSH;
	FileOut = fopen(FileNameOut, "w");
	//Find the highest rating number
	for (walk = art_array; walk <= last_element; walk++)
	{
		if (highest_rating < (*walk)->rating)
			highest_rating = (*walk)->rating;
	}
	//Print all pieces with the highest rating
	for (walk = art_array; walk <= last_element; walk++)
	{
		if ((*walk)->rating == highest_rating)
		{
			fprintf(FileOut, "%s", (*walk)->name);
			fprintf(FileOut, "%s\n", (*walk)->art);
		}
	}
	fclose(FileOut);
	return;
}

/*
Write_To_File_Sorted_Art will write to a file with all the art work that were sorted in alphabetical order.
Pre: Array of art pointers and the number of pieces
Post: A file will have all the sorted artwork
*/
void Write_To_File_Sorted_Art (ART** art_array, int number_of_pieces)
{
	char FileNameOut[FILENAME_MAX];
	FILE* FileOut;
	ART** walker = art_array;
	ART** beginning = art_array;
	ART** last_element = art_array + (number_of_pieces-1);
	int number_of_submissions = 0;
	int i = 0;

	printf("Please enter a name to save a file with art that has been sorted in alpahabetical order:  ");
	scanf("%s", FileNameOut);
	FLUSH;
	FileOut = fopen(FileNameOut, "w");

	while (beginning <= last_element)
	{
		walker = beginning;
		number_of_submissions = 0;
		while (walker <= last_element)
		{
			if (strcmp((*walker)->name, (*beginning)->name) == 0)
			{
				number_of_submissions++;
			}
			walker++;
		}
		fprintf(FileOut, "%d %s\n", number_of_submissions, (*beginning)->name);
		for (i = 0; i < number_of_submissions; i++)
		{
			fprintf(FileOut, "%s", (*beginning)->art);
			fprintf(FileOut, "# %d\n\n", (*beginning)->rating);
			beginning++;
		}
	}
	fclose(FileOut);
	return;
}

/*
Clean_Up will clean all dynamically allocated objects
Pre: The array of art pointers and the number of pieces
Post: All allocated memory will be freed
*/
void Clean_Up(ART** art_array, int number_of_pieces)
{
	ART** first_element = art_array;
	ART** last_element = art_array + (number_of_pieces-1);

	for (art_array; art_array <= last_element; art_array++)
	{
		free ((*art_array)->art);		//free dynamically allocated art   (This is a change from the original source file)
		free ((*art_array)->name);		//free dynamically allocated name  (This is a change from the original source file)
		free ((*art_array));			//free ART* pointers
	}
	free(first_element);	//free ART** pointer
	return;
}
/*
Output:

This program reads an input file for ASCII art and will save two files.
One file has a list of all artists, the number of entries they put, and
their art. The second file has a list of the art with the highest rating
and their artist.
Please enter the file containing the ASCII art:  art.txt
Please enter a name to save a file with the highest rated art pieces:  output1.t
xt
Please enter a name to save a file with art that has been sorted in alpahabetica
l order:  output2.txt
Press any key to continue . . .
*/
