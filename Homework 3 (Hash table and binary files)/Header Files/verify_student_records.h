#ifndef VERIFY_STUDENT_RECORDS_INCLUDED
#define VERIFY_STUDENT_RECORDS_INCLUDED

#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define BUFFERSIZE 100

/*
This header file contains the functions that will manipulate a given string to make sure it matches the specifications
of the student record fields. Namelly, first and last name must be less than or equal to 20 characters (each), the
student ID is 4 digits, and the amount lies within 10.00 and 99999.99 while having exactly 2 decimal numbers.
*/

void verify_wrapper (bool (*actual_verify_function)(char* to_validate), char* error_message, char* validated);
bool verify_4_digit (char* input);
bool verify_name (char* name);
bool verify_amount (char* amount);

bool verify_4_digit (char* input)
{
    bool verify = false;
    bool escape = false;
    int string_length = strlen(input);

    if (string_length == 4)
    {
        while (*input && !escape)
        {
            if (isdigit(*input))
                input++;
            else
                escape = true;
        }
        if ( !strcmp(input, "\0") )
            verify = true;
    }
    return verify;
}

bool verify_name (char* name)
{
    bool verify = false;
    bool escape = false;
    int string_length = strlen(name);
    if (string_length <= 20)
    {
        while (*name && !escape)
        {
            if (isalpha(*name))
                name++;
            else
                escape = true;
        }
        if (!strcmp(name, "\0"))
            verify = true;
    }
    return verify;
}

bool verify_amount (char* amount)
{
    bool verify = false;
    bool escape = false;
    char local_copy[BUFFERSIZE];
    strcpy (local_copy, amount);    //make a local copy to validate (string tokenization will modify the string)
    char* top = strtok (local_copy, "." );      //top holds the numbers up to the decimal
    char* decimal = strtok (NULL, "\0");        //decimal holds the decimal numbers

    if (decimal && strlen(decimal) == 2 && isdigit(decimal[0]) && isdigit(decimal[1])) //if there was a decimal point with two characters and the characters are numbers
    {
        char* walk = top;
        while (*walk && !escape)
        {
            if (isdigit(*walk))
                walk++;
            else
                escape = true;
        }
        if (atol(top) >= 10 && atol(top) <= 99999 && !strcmp(walk, "\0") ) //if the top part is within range and the walker got to the end of number without escaping
            verify = true;
    }
    return verify;
}

//I had to use a function pointer becuase validation got repetitive otherwise
void verify_wrapper (bool (*actual_verify_function)(char* to_validate), char* error_message, char* validated)
{
    bool verified;
    char buffer[BUFFERSIZE];
    do
    {
        fgets (buffer, BUFFERSIZE, stdin);
        strtok (buffer, "\n");  //i need to get rid of the \n that fgets reads
        if ( !(verified = (*actual_verify_function)(buffer)) )
            printf(error_message);
    } while (!verified);
    strcpy (validated, buffer);
}

#endif // VERIFY_STUDENT_RECORDS_INCLUDED
