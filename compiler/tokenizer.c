#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>

#include "tokenizer.h"

struct tokenizer {
    size_t cursor;
    const char* string;
    char specs[9][128];
    int spec_size;
};

void slice_string(char* dest, const char* src, int so, int eo) {
    strncpy(dest, src+so, eo-so);
    dest[eo] = '\0';
}

char* regex_first_match(const char* str, const char* regex_exp) {
    regex_t regex;
    int ret = regcomp(&regex, regex_exp, REG_EXTENDED);
    if (ret) {
        fprintf(stderr, "Could not compile regex \"%s\"\n", regex_exp);
        exit(1);
    }
    regmatch_t match[2];
    ret = regexec(&regex, str, 2, match, 0);
    regfree(&regex);

    if (ret == REG_NOMATCH) {
        return NULL;
    }

    int so = match[0].rm_so;
    int eo = match[0].rm_eo;

    char* matched = malloc((eo-so+1)*sizeof(char));
    slice_string(matched, str, so, eo);

    return matched;
}


token* token_init() {
    token* t = malloc(sizeof(token));
    t->type = NULL_TOKEN;
    t->value = NULL;

    return t;
}

token* token_delete(token* t) {
    free(t->value);
    free(t);
    return NULL;
}

char* token_to_str(token* t) {
    char str[256];

    if (t->type == INTEGER) {
        int value = atoi(t->value);
        sprintf(str, "{ type: INTEGER, value: %d }", value);
    }
    else if (t->type == REAL) {
        float value = atof(t->value);
        sprintf(str, "{ type: REAL, value: %f }", value);
    }
    else if (t->type == STRING) {
        char* value = t->value;
        sprintf(str, "{ type: STRING, value: %s }", value);
    }
    else if (t->type == IDENTIFIER) {
        char* value = t->value;
        sprintf(str, "{ type: IDENTIFIER, value: %s }", value);
    }
    else if (t->type == FUNCTION) {
        char* value = t->value;
        sprintf(str, "{ type: FUNCTION, value: %s }", value);
    }
    else if (t->type == IF) {
        char* value = t->value;
        sprintf(str, "{ type: IF, value: %s }", value);
    }
    else if (t->type == ELSE) {
        char* value = t->value;
        sprintf(str, "{ type: ELSE, value: %s }", value);
    }
    else {
        sprintf(str, "{ type: NULL, value: }");
    }

    return strdup(str);
}

tokenizer* tokenizer_init(const char* string) {
    tokenizer* tk = malloc(sizeof(tokenizer));
    tk->cursor = 0;
    tk->string = string;

    tk->spec_size = 9;

    strcpy(tk->specs[INTEGER], "^([0-9]+)");
    strcpy(tk->specs[STRING], "^\"[^\"]*\"");
    strcpy(tk->specs[REAL], "^([0-9]+[.][0-9]+|[.][0-9]+)");
    strcpy(tk->specs[IDENTIFIER], "^([[:alpha:]]|_)([[:alnum:]]|_)*");
    strcpy(tk->specs[FUNCTION], "^function");
    strcpy(tk->specs[IF], "^if");
    strcpy(tk->specs[ELSE], "^else");
    strcpy(tk->specs[WHITE_SPACE], "^([[:space:]]+)");

    return tk;
}

tokenizer* tokenizer_delete(tokenizer* tk) {
    free(tk);

    return NULL;
}

int tokenizer_has_tokens(tokenizer* tk) {
    return tk->cursor < strlen(tk->string);
}

int tokenizer_is_eof(tokenizer* tk) {
    return tk->cursor == strlen(tk->string);
}

const char* check_reserved_word(const char* str, const char* word) {
    if (!strcmp(str, word)) {
        return strdup(word);
    } else {
        return NULL;
    }
}

token* tokenizer_next(tokenizer* tk) {
    token* t = token_init();

    if (!tokenizer_has_tokens(tk)) {
        return NULL_TOKEN;
    }

    char* match = NULL;
    char* last_match = NULL;
    int n = 0;
    for (int type = 1; type < tk->spec_size; type++) {
        char* regex = tk->specs[type];

        match = regex_first_match(tk->string+tk->cursor, regex);
        if (match == NULL) {
            continue;
        }

        if (type == WHITE_SPACE) {
            tk->cursor += strlen(match);
            type = 0;
            free(match);
            continue;
        }

        if (last_match == NULL || strlen(match) > strlen(last_match)) {
            t->type = type;
            t->value = match;
            n = strlen(match);
            free(last_match);
            last_match = match;
        } else {
            free(match);
        }
    }
    tk->cursor += n;
    return t;
}
