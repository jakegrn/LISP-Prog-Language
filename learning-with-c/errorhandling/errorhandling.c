/* Compilation instructions
    
    gcc -std=c99 -Wall parsing.c mpc.c -o lispy

*/

#include <stdio.h> /* input output library for the command line */
#include <stdlib.h>
#include "mpc.h"

static char input[2048];

long eval_op(long x, char* op, long y)
{
    if (strcmp(op, "+") == 0) { return x + y; } // strcmp returns 0 if they are both true
    if (strcmp(op, "-") == 0) { return x - y; } // unlike traditional boolean conditions
    if (strcmp(op, "*") == 0) { return x * y; }
    if (strcmp(op, "/") == 0) { return x / y; }
    if (strcmp(op, "%") == 0) { return x % y; }
    return 0;
}

long eval(mpc_ast_t* t)
{
    if (strstr(t->tag,"number")) // not 100% but assumes returns true when char* substring found in first char* 
    {
        return atoi(t->contents); // atoi(char*) converts string to integer where possible
    }

    // operator is always the second child after '>'
    char* op = t->children[1]->contents;

    // store remaining children in x to evaluate left tree
    long x = eval(t->children[2]);

    // iterate remaining children and combine recursively
    
    int i = 3;
    while (strstr(t->children[i]->tag, "expr")) {
        x = eval_op(x, op, eval(t->children[i])); // 5 + (next expression)
        i++;
    }

    return x;
}

int number_of_nodes(mpc_ast_t* t) // for a tree, every parent must have at least 2 or more child nodes
{
    if ((t->children_num) == 0) {   return 1;   }
    if ((t->children_num) >= 1)
    {
        int total = 1;
        for (int i = 0; i < t->children_num; i++)
        {
            total = total + number_of_nodes(t->children[i]);
        }
        return total;
    }
    return 0;
}

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

        // we will be creating a data structure for lisp values

        struct {
            int type; // either 0 (struct is a num) or 1 (struct is an error)
            long num;
            int err;
        } lval;

        // since type is of int, we create enum of possible lisp value types (0 or 1)
        enum { LVAL_NUM, LVAL_ERR };

        // do similarly for the many error returns we may get
        // either division by 0, operator given is invalid or num has invalid input 
        enum { DIV_BY_ZERO, INVALID_NUM, INVALID_OP };

        mpc_ast_t* result = r.output;

        printf("The result of this output is %li \n" , eval(result));
        mpc_ast_delete(result);
    }
    
    mpc_cleanup(4, Number, Operator, Expr, Lispy); /* Undefines and deletes parsers, takes n (4) parameters and delete them */    
    
    return 0;
}

