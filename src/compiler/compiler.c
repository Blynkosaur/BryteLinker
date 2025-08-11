#include "../../include/compiler/compiler.h"
#include <stdio.h>
#include "../../include/common.h"
#include "../../include/compiler/scanner.h"

void compile (const char* source){
    initScanner(source);
    int line = -1;

while (1){
    Token token = scanToken();
    if(token.line != line){
        printf("%4d ", token.line);
        line = token.line;
    }else{
        printf("   | ");
    }
    printf("%2d '%.s'\n", token.type, token.length, token.start);
    ir(token.type == TOKEN_EOF) break;
}
}

