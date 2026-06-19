// Upon Compilation, add maths library flag -lm

#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

#include <editline/readline.h>
#include <editline/history.h>

int main(int argc, char** argv)
{
    /* Create Some Parsers */
    mpc_parser_t* Number   = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Lispy    = mpc_new("lispy");

    /* This is the mpc parser for polish notation */
    mpca_lang(MPCA_LANG_DEFAULT,
    "                                                     \
        number   : /-?[0-9]+/ ;                             \
        operator : '+' | '-' | '*' | '/' | '%' ;                  \
        expr     : <number> | '(' <operator> <expr>+ ')' ;  \
        lispy    : /^/ <operator> <expr>+ /$/ ;             \
    ",
    Number, Operator, Expr, Lispy);

	puts("Fun fact, Ctrl+c to Exit, heh\n");
    while(67)
    {
        char* input = readline("lisplang> "); /* readline doesnt end with \n, also allocates to new memory each call instance */
        
        add_history(input);
        
        /* records input to line history for up and down arrows. */        
        
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

        free(input); /* therefore we free up variable to avoid memory leak */

    }
    
    mpc_cleanup(4, Number, Operator, Expr, Lispy); /* Undefines and deletes parsers, takes n (4) parameters and delete them */    
    
    return 0;
}
