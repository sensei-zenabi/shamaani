#define _POSIX_C_SOURCE 200809L
#include "parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parser_run(const LangModel *model) {
    char *line = NULL;
    size_t capacity = 0;
    int result = 0;
    puts("Keskustele Shamaanin kanssa. Näet komennot kirjoittamalla /ohje.");
    for (;;) {
        fputs("Sinä > ", stdout);
        fflush(stdout);
        ssize_t length = getline(&line, &capacity, stdin);
        if (length == -1) {
            if (!feof(stdin)) {
                fputs("Syötteen lukeminen epäonnistui.\n", stderr);
                result = 1;
            }
            break;
        }
        if (memchr(line, '\0', (size_t)length)) {
            puts("Kirjoita tekstiä ilman nollatavuja.");
            continue;
        }
        while (length && isspace((unsigned char)line[length - 1])) line[--length] = '\0';
        char *input = line;
        while (isspace((unsigned char)*input)) ++input;
        if (!*input) continue;
        if (!strcmp(input, "/lopeta") || !strcmp(input, "/poistu") ||
            !strcmp(input, "/quit") || !strcmp(input, "/exit")) break;
        if (!strcmp(input, "/ohje") || !strcmp(input, "/help")) {
            puts("Kirjoita viesti, niin etsin vastauksen aineistosta. /lopeta tai /poistu päättää keskustelun. Myös Ctrl-D lopettaa.");
        } else if (*input == '/') {
            puts("Tuntematon komento. Näet komennot kirjoittamalla /ohje.");
        } else {
            printf("Shamaani > %s\n", langmdl_answer(model, input));
        }
    }
    free(line);
    puts("\nShamaani > Näkemiin!");
    return result;
}
