#include "argo.h"

int peek(FILE *stream) {
    int c = getc(stream);
    ungetc(c, stream);
    return c;
}

void unexpected(FILE *stream) {
    if (peek(stream) != EOF)
        printf("Unexpected token '%c'\n", peek(stream));
    else
        printf("Unexpected end of input\n");
}

int accept(FILE *stream, char c) {
    if (peek(stream) == c) {
        (void) getc(stream);
        return 1;
    }
    return 0;
}

int expect(FILE *stream, char c) {
    if (accept(stream, c)) {
        return 1;
    }
    unexpected(stream);
    return 0;
}



//add your code here
