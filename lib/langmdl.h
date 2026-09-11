#ifndef SHAMAANI_LANGMDL_H
#define SHAMAANI_LANGMDL_H

#include <stddef.h>

typedef struct {
    char *prompt;
    char *answer;
} LangEntry;

typedef struct {
    LangEntry *entries;
    size_t count;
} LangModel;

/* Initialize before loading. The caller owns the model and must free it. */
int langmdl_load(LangModel *model, const char *path);
/* Returned text belongs to the model (or is static); do not free it. */
const char *langmdl_answer(const LangModel *model, const char *input);
void langmdl_free(LangModel *model);

#endif
