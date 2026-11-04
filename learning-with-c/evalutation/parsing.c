/* Compilation instructions
    
    gcc -std=c99 -Wall parsing.c mpc.c -o lispy

*/

#include <stdio.h> /* input output library for the command line */
#include <stdlib.h>
#include "mpc.h"

static char input[2048];

int main(int argc, char** argv)
{
    /* Create Some Parsers */
    mpc_parser_t* Number   = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Lispy    = mpc_new("lispy");

    /* This is the regex mpc parser definition for polish notation */
    mpca_lang(MPCA_LANG_DEFAULT,
    "                                                     \
        number   : /-?[0-9]+/ ;                             \
        operator : '+' | '-' | '*' | '/' | '%' ;                  \
        expr     : <number> | '(' <operator> <expr>+ ')' ;  \
        lispy    : /^/ <operator> <expr>+ /$/ ;             \
    ",
    Number, Operator, Expr, Lispy);

	puts("Fun fact, Ctrl+c to Exit, heh"); /* Appends new line character, as compared to fputs */

    while(1) /* Any +tive no. */
    {
        fputs("lispy> ", stdout); 

        /* Reads line of user input of max size 2048 */
        fgets(input, 2048, stdin);
        /* Both fgets and fputs require some file to write or read from (e.g stdin declared in stdio.h)        
        
        /* Parsing user input */
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)) {
            /* On Success print the AST */
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        }
        else {
            /* otherwise print the error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
        
        // printf("You have entered the string: %s\n", input);

    }
    
    mpc_cleanup(4, Number, Operator, Expr, Lispy); /* Undefines and deletes parsers, takes n (4) parameters and delete them */    
    
    return 0;
}
