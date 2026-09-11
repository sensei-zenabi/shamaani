#define _POSIX_C_SOURCE 200809L
#include "langmdl.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *trim(char *text) {
    while (isspace((unsigned char)*text)) ++text;
    size_t length = strlen(text);
    while (length && isspace((unsigned char)text[length - 1]))
        text[--length] = '\0';
    return text;
}

static char *normalize(const char *text) {
    char *copy = strdup(text);
    if (copy) {
        for (char *p = copy; *p; ++p)
            if ((unsigned char)*p < 128) *p = (char)tolower((unsigned char)*p);
    }
    return copy;
}

void langmdl_free(LangModel *model) {
    for (size_t i = 0; i < model->count; ++i) {
        free(model->entries[i].prompt);
        free(model->entries[i].answer);
    }
    free(model->entries);
    *model = (LangModel){0};
}

int langmdl_load(LangModel *model, const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Aineistotiedoston avaaminen epäonnistui: %s\n", path);
        return -1;
    }
    LangModel loaded = {0};
    char *line = NULL;
    size_t capacity = 0, line_number = 0;
    ssize_t length;
    int result = -1;
    while ((length = getline(&line, &capacity, file)) != -1) {
        ++line_number;
        if (memchr(line, '\0', (size_t)length)) goto malformed;
        char *prompt = trim(line);
        if (!*prompt || *prompt == '#') continue;
        char *answer = strchr(prompt, '\t');
        if (!answer) goto malformed;
        *answer++ = '\0';
        prompt = trim(prompt);
        answer = trim(answer);
        if (!*prompt || !*answer) goto malformed;
        LangEntry entry = {normalize(prompt), strdup(answer)};
        LangEntry *entries = NULL;
        if (entry.prompt && entry.answer)
            entries = realloc(loaded.entries, (loaded.count + 1) * sizeof(*entries));
        if (!entries) {
            free(entry.prompt);
            free(entry.answer);
            fprintf(stderr, "Muistin varaaminen aineistolle epäonnistui.\n");
            goto done;
        }
        loaded.entries = entries;
        loaded.entries[loaded.count++] = entry;
    }
    if (!feof(file)) {
        fprintf(stderr, "Aineiston lukeminen epäonnistui: %s\n", path);
        goto done;
    }
    if (!loaded.count) {
        fprintf(stderr, "Aineisto ei sisällä kysymys–vastauspareja: %s\n", path);
        goto done;
    }
    langmdl_free(model);
    *model = loaded;
    loaded = (LangModel){0};
    result = 0;
    goto done;
malformed:
    fprintf(stderr, "%s:%zu: odotettu muoto on kysymys<SARKAIN>vastaus.\n", path, line_number);
done:
    free(line);
    fclose(file);
    langmdl_free(&loaded);
    return result;
}

static int word_byte(unsigned char c) {
    return c >= 128 || isalnum(c) || c == '_';
}

const char *langmdl_answer(const LangModel *model, const char *input) {
    char *query = normalize(input);
    if (!query) return "Viestin käsittely epäonnistui: muisti loppui.";
    const char *answer = "Aineistossani ei vielä ole vastausta tähän.";
    size_t best = 0;
    for (size_t i = 0; i < model->count; ++i) {
        const char *prompt = model->entries[i].prompt;
        size_t length = strlen(prompt);
        const char *match = query;
        while ((match = strstr(match, prompt))) {
            if ((match == query || !word_byte((unsigned char)match[-1])) &&
                !word_byte((unsigned char)match[length]) && length > best) {
                best = length;
                answer = model->entries[i].answer;
            }
            ++match;
        }
    }
    free(query);
    return answer;
}
