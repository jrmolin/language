#pragma once

#include "_types.h"
#include "utils.h"

scanner_t *ReadFile(const char *file);
void printToken(int fd, token_t *token);
const char *
getOperatorName(operator_kind_t op);
