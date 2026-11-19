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


        /* printf("\nLoading AST from output\n");
        mpc_ast_t* a = r.output;
        printf("Tag: %s\n", a->tag); //field access of pointer types always uses ->
        printf("Contents %s\n", a->contents);
        printf("No. of children: %d\n", a->children_num);

        printf("\nGetting first child\n");

        mpc_ast_t* c1 = a->children[1];
        printf("Tag: %s\n", c1->tag); //field access of pointer types always uses ->, tag is 
        printf("Contents %s\n", c1->contents);
        printf("No. of children: %d\n", c1->children_num);

        */

        /*

        Because initial output will look like this.

        lispy> + 1 1
        > 
        regex 
        operator|char:1:1 '+'
        expr|number|regex:1:3 '1'
        expr|number|regex:1:5 '1'
        regex 
        */

        mpc_ast_t* result = r.output;

        
        for (int i = 0; i < result->children_num; i++)
        {
            printf("%ith item %s \n", i, result->children[i]->contents);
        }

        printf("The result of this output is %li \n" , eval(result));
        printf("Number of nodes in this tree: %d", number_of_nodes(result));
        mpc_ast_delete(result);
    }
    
    mpc_cleanup(4, Number, Operator, Expr, Lispy); /* Undefines and deletes parsers, takes n (4) parameters and delete them */    
    
    return 0;
}

