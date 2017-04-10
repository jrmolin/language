
#include "_types.h"

ssize_t __read(int fd, char *buffer, size_t count)
{
    size_t cbRead = 0;
    ssize_t remaining = count;
    ssize_t result = 0;

    if (0 == count || InvalidPtr(buffer))
    {
        goto Cleanup;
    }

    while (remaining > 0)
    {
        ssize_t numRead = read(fd, buffer + cbRead, remaining);

        if (numRead <= 0)
        {
            break;
        }

        remaining -= numRead;
        cbRead += numRead;
    }

    result = cbRead;

Cleanup:
    return result;
}

const char *
getOperatorName(operator_kind_t op)
{
    const char *result = "UNKNOWN";

    switch(op)
    {
        case PLUS:
            result = "PLUS";
            break;

        case TIMES:
            result = "TIMES";
            break;

        case MINUS:
            result = "MINUS";
            break;

        case DIVIDE:
            result = "DIVIDE";
            break;

        default:
            result = "INVALID";
            break;
    }

    return result;
}
const char *
getTokenName(token_type_t tokenType)
{
    const char *result = "UNKNOWN";

    switch(tokenType)
    {
        case NUMBER:
            result = "NUMBER";
            break;

        case OPERATOR:
            result = "OPERATOR";
            break;

        case LCURLY:
            result = "LCURLY";
            break;

        case RCURLY:
            result = "RCURLY";
            break;

        case LSQUARE:
            result = "LSQUARE";
            break;

        case RSQUARE:
            result = "RSQUARE";
            break;

        case LPAREN:
            result = "LPAREN";
            break;

        case RPAREN:
            result = "RPAREN";
            break;

        case IDENTIFIER:
            result = "IDENTIFIER";
            break;

        case DEF:
            result = "DEF";
            break;

        case QUOTEDSTRING:
            result = "QUOTEDSTRING";
            break;

        case ASSIGN:
            result = "ASSIGN";
            break;

        default:
            result = "INVALID";
            break;
    }

    return result;
}
