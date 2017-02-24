#pragma once

#include "_types.h"

// *****************************************************************************
//         functions
// *****************************************************************************
const char *
getTokenName(token_type_t tokenType);

ssize_t __read(int fd, char *buffer, size_t count);
