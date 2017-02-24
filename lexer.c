
#include <ctype.h>

#include "_types.h"
#include "utils.h"

const char *__input = "(defn hello () (str \"hello, \" \"world!\"))";

/*
 * write the state machine for the lexer
 *
 * - number (integer, float, octal, hex)
 * - string
 * - bareword/identifier
 * - symbol thing
 * - single-character token
 *
 */

typedef enum
{
    STATE_INVALID = -1,
    STATE_WHITESPACE = 0,
    STATE_STRING,
    STATE_OCTAL,
    STATE_HEXADECIMAL,
    STATE_DECIMAL,
    STATE_IDENTIFIER,
    STATE_EOT,
} eState;

void __skipWhiteSpace(scanner_t *scanner)
{
    if (ValidPtr(scanner))
    {
        for (size_t i = scanner->index; i < scanner->length; ++i)
        {
            char c = scanner->input[i];

            if (' ' == c || '\t' == c || '\r' == c)
            {
                scanner->index++;
            }
            else if ('\n' == c)
            {
                scanner->index++;
                scanner->line++;
            }
            else
            {
                break;
            }
        }
    }
}

void __getNumber(
    const char *string,
    size_t length,
    token_t *token)
{
    char *ptr = NULL;
    uint64_t value = strtoull(string, &ptr, 0);
    token->length = (size_t)ptr - (size_t)string;;
    token->value.u64 = value;
}


token_t * __getNextToken(scanner_t *scanner)
{
    token_t *result = NULL;
    token_t tmp = {0,};
    eState state = STATE_INVALID;
    size_t index = 0;
    size_t remaining = 0;

    if (InvalidPtr(scanner))
    {
        goto Cleanup;
    }

    // skip white space
    __skipWhiteSpace(scanner);

    index = scanner->index;
    remaining = scanner->length - index;

    // while loop
    while (index < scanner->length)
    {
        int saveAndBreak = 0;
        char c = scanner->input[index];
        tmp.offset = index;
        tmp.lineno = scanner->line;

        // does this start a number?
        if (isdigit(c))
        {
            tmp.type = NUMBER;
            __getNumber(&(scanner->input[index]), remaining, &tmp);
            saveAndBreak = 1;
        }
        else if('+' == c || '-' == c || '*' == c || '/' == c)
        {
            tmp.length = 1;
            tmp.value.cstr = &(scanner->input[index]);
            switch(c)
            {
                case '+':
                    tmp.type = PLUS;
                    break;

                case '-':
                    tmp.type = MINUS;
                    break;

                case '*':
                    tmp.type = TIMES;
                    break;

                case '/':
                    tmp.type = DIVIDE;
                    break;
                default:
                    break;
            }

            saveAndBreak = 1;
        }
        else if('"' == c)
        {
            tmp.type = QUOTEDSTRING;
            int escapeToggle = 0;
            tmp.value.cstr = &(scanner->input[index]);

            for (size_t i = index + 1; i < scanner->length; ++i)
            {
                c = scanner->input[i];

                if (escapeToggle && '"' != c)
                {
                    escapeToggle = 0;
                }

                if ('"' == c && !(escapeToggle))
                {
                    tmp.length = i - index + 1;
                    printf("string [%.*s]\n", tmp.length, tmp.value.cstr);
                    break;
                }

                if ('\\' == c)
                {
                    escapeToggle ^= 1;
                }
            }

            saveAndBreak = 1;
        }
        else
        {
            index++;
        }

        if (saveAndBreak)
        {
            memcpy(&(scanner->lastToken), &(scanner->token), sizeof(scanner->token));
            memcpy(&(scanner->token), &(tmp), sizeof(tmp));;
            result = &(scanner->token);
            break;
        }
    }

    scanner->index += scanner->token.length;

Cleanup:
    return result;
}

scanner_t *ReadFile(const char *file)
{
    scanner_t *tmpResult, *result = NULL;
    struct stat sb = {0,};
    size_t cbRead = 0;
    int fd = -1;

    int res = stat(file, &sb);

    if (0 == res)
    {
        size_t size = sizeof(*result) + sb.st_size;
        tmpResult = (scanner_t*)malloc(size);

        if (NULL == tmpResult)
        {
            goto Cleanup;
        }

        ZeroBytes(tmpResult, size);

        tmpResult->length = sb.st_size;
        tmpResult->input = (char *)((size_t)tmpResult + sizeof(*tmpResult));
        tmpResult->getToken = __getNextToken;
    }
    else
    {
        printf("failed\n");
        goto Cleanup;
    }

    // open file and read it
    // int open(const char *pathname, int flags);
    fd = open(file, O_RDONLY);

    if (fd > -1)
    {
        ssize_t cbRead = __read(fd, tmpResult->input, sb.st_size);

        if (cbRead < sb.st_size)
        {
            goto Cleanup;
        }
    }
    else
    {
        printf("failed to open [%s]\n", file);
        goto Cleanup;
    }

    result = tmpResult;
    tmpResult = NULL;

Cleanup:
    if (NULL != tmpResult)
    {
        free(tmpResult);
        tmpResult = NULL;
    }

    return result;
}
