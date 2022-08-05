#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

int main() {
    const char* str = " 42 32.18 foo function function_name if else \"joao\"";

    tokenizer* tk = tokenizer_init(str);
    while (tokenizer_has_tokens(tk)) {
        token* t = tokenizer_next(tk);

        if (t->type == NULL_TOKEN) {
            printf("Lexical error!\n");
            // return 0;
        }

        char* s = token_to_str(t);
        printf("%s\n", s);

        free(s);
        token_delete(t);
    }
    tokenizer_delete(tk);
}