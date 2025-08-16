#include "../../include/compiler/compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../include/compiler/scanner.h"




typedef struct{
  Token current;
  Token previous;
  bool cooked;
  bool hadError;

} Parser;
Chunk * compilingChunk;

static Chunk* currentChunk(){
    return compilingChunk;
}

Parser parser;
//parser scans the tokens and transforms it into the tree/ syntax bullshit, just keep reading
// do not read these comments please
static void errorAt (Token * token, const char* message){
    if (parser.cooked) return;
    fprintf(stderr, "[line %d] Error", token->line);
    if (token->type == TOKEN_EOF){
        fprintf (stderr, " at end");
        
    }else if (token->type == TOKEN_ERROR){
        // Why is this nothing?? idk
    }else{
        fprintf(stderr, "at '%.*s", token->length, token->start);
    }
    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
    parser.cooked = true;

}
static void error(const char* message){
    errorAt(&parser.previous, message);
}
static void errorAtCurrent(const char* message){
    errorAt(&parser.current, message);
}
static void advance(){
    parser.previous = parser.current;
    while (1){
        parser.current = scanToken();
        if(parser.current.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.current.start);
    }
}
static void expression(){

}
static void consume(TokenType type, const char* message){
    if (parser.current.type == type){
        advance();
        return;
    }
    errorAtCurrent(message);
}

static void writeByte(uint8_t byte){
    writeChunk(currentChunk(), byte, parser.previous.line);
}
static void emitReturn(){
    writeByte(OP_RETURN);
}
static void writeBytes(uint8_t byte1, uint8_t byte2){
    writeByte(byte1);
    writeByte(byte2);
}
static void endCompiler(){
    emitReturn();
}



bool compile (const char* source, Chunk* chunk){
    initScanner(source);
    compilingChunk = chunk;
    parser.hadError = false;
    parser.hadError = false;
    advance();
    expression();
    consume(TOKEN_EOF, "Expected end of expression.");
    endCompiler();
    return !parser.hadError;



}

