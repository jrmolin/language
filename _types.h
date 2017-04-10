#pragma once

#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static inline int InvalidPtr(void *ptr)
{
    int result = 0;
    size_t val = (size_t)ptr;

    if (val < 4096)
    {
        result = 1;
    }
    return result;
}

static inline int ValidPtr(void *ptr)
{
    return !InvalidPtr(ptr);
}

#define ZeroBytes(ptr, _sz) memset((ptr), 0, (_sz))

struct _scanner_t;
typedef struct _scanner_t scanner_t;

struct _scanner_api_t;
typedef struct _scanner_api_t scanner_api_t;

struct _token_t;
typedef struct _token_t token_t;

typedef token_t * (*NextTokenFunction)(scanner_t *);

typedef enum
{
   NUMBER = 0,
   OPERATOR,
   LPAREN,
   RPAREN,
   LCURLY,
   RCURLY,
   LSQUARE,
   RSQUARE,
   IDENTIFIER,
   DEF,
   QUOTEDSTRING,
   ASSIGN,
} token_type_t;

typedef enum
{
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
} operator_kind_t;

struct _token_t
{
    token_type_t type;

    union
    {
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        uint8_t  u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
        const char *cstr;
        void *vptr;
        operator_kind_t kind;
    } value;

    const char *string;

    size_t lineno;  // line number of this token
    size_t offset;  // byte offset into the file (ascii only for now)
    size_t length;  // length of the string that made this
    size_t hoffset; // horizontal offset / column number
};

struct _scanner_api_t
{
    char placeholder;
};

struct _scanner_t
{
    // for now, just grab the whole thing
    char *input;
    size_t length;
    size_t index;
    size_t line;

    token_t token;
    token_t lastToken; // for rewinding

    NextTokenFunction getToken;

    scanner_api_t api;
};


