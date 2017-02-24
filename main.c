
#include <stdio.h>

#include "lexer.h"
#include "utils.h"

#include "macros.h"

int main(int argc, char **argv)
{
    int result = 0;
    scanner_t *scanner = NULL;

    if (2 > argc)
    {
        printf("usage: %s <file>\n", *argv);
        printf(EVAL(JOIN_STRINGS("\n",
            "usage: %s <file>",
            "this is used to do things that are neat",
            "last line here")) "\n", *argv);
        goto Cleanup;
    }

    scanner = ReadFile(argv[1]);

    if (NULL != scanner)
    {
        printf("input: [%s]\n", scanner->input);
        for (;;)
        {
            token_t *token = scanner->getToken(scanner);

            if (ValidPtr(token))
            {
                printf("[%s] [%lu] [%.*s]\n", getTokenName(token->type), token->value.u64,
                    token->length, &(scanner->input[token->offset]));
            }
            else
            {
                break;
            }
        }

        free(scanner);
    }

Cleanup:
    printf(EVAL(GREET(JOIN_STRINGS(", ", "Mum", "Dad", "Adam", "Joe"))));
    return result;
}
