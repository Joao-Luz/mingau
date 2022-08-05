#pragma once

// tokens
#define NULL_TOKEN 0
#define INTEGER    1
#define STRING     2
#define REAL       3
#define FUNCTION   4
#define IF         5
#define ELSE       6
#define WHITE_SPACE 7
#define IDENTIFIER 8

typedef struct tokenizer tokenizer;
typedef struct token {
    int type;
    char* value;
} token;

token* token_init();
token* token_delete(token* t);
char* token_to_str(token* t);

tokenizer* tokenizer_init(const char* string);
tokenizer* tokenizer_delete(tokenizer* tk);
int tokenizer_has_tokens(tokenizer* tk);
token* tokenizer_next(tokenizer* tk);