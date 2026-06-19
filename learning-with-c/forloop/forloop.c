#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

/*static char input[2048];*/

/* Declaring buffer for user input of size 2048,
    A buffer is just a space of memory
    
    Global array of 2048 CHARACTERS.
    
    static KEYWORD makes variable local to the file.
  */

int main(int* argc, char** argv)
{
    puts("Fun fact, Ctrl+c to Exit, heh\n");
    while(67)
    {
        /*fputs("lisplang> ", stdout);*/
        
        /*fgets(input, 2048, stdin);*/

        /*printf("You have entered the string: %s", input);*/
        
        /* 
        different to puts because does not append new line before. lisplang> 'input'
        fgets function reads at most n-1 characters + null terminator.
        if this is cut short, function will iterate length(input) / n-1 times since fgets reads what is left until the null terminator. 
        */
        
        char* input = readline("lisplang> "); /* readline doesnt end with \n, also allocates to new memory each call instance */
        
        add_history(input);
        
        /* records input to line history for up and down arrows. */        
        
        printf("You have entered the string: %s\n", input);
        
        free(input); /* therefore we free up variable to avoid memory leak */
    }
    
    return 0;
}
