#ifndef SHAMAANI_PARSER_H
#define SHAMAANI_PARSER_H

#include "langmdl.h"

/* Run the terminal chat until /quit, /exit, or EOF. */
int parser_run(const LangModel *model);

#endif
