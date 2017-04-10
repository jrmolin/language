
#include <stdio.h>

#include "lexer.h"
#include "utils.h"

int main(int argc, char **argv)
{
    int result = 0;
    scanner_t *scanner = NULL;

    if (2 > argc)
    {
        printf("usage: %s <file>\n", *argv);
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
                printToken(1, token);
            }
            else
            {
                break;
            }
        }

        free(scanner);
    }

Cleanup:
    return result;
}
