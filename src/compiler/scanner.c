#include <stdio.h>
#include <string.h>
#include "../../include/common.h"
#include "../../include/compiler/scanner.h"

typedef struct{
    const char* start;
    const char* current;
    int line;


}Scanner;

Scanner scanner;

void initScanner(const char* source){
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isAtEnd(){
    return *(scanner.current) == '\0';
}
static Token makeToken(TokenType type){
    Token token;
    token.type = type;
    token.length = (int) (scanner.current-scanner.start);
    token.start = scanner.start;
    token.line = scanner.line;
    return token;

}
static Token errorToken(const char* message){
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int) strlen(message);
    token.line = scanner.line;
    return token;
}
static char advance(){
    char current_char = scanner.current[0];
    scanner.current++;
    return current_char;
}
static bool match_next (char expected){
    if (isAtEnd()) return false;
    if (*(scanner.current) == expected) {
        scanner.current ++;
        return true;
    }
    return false;
}
static char peek(){
    return *(scanner.current);
}
static char peekNext(){
    if (isAtEnd()) return '\0'; // in case already at the end of file so can't peek forward anymore
    return *(scanner.current +1);
}
static void skipWhitespace(){
    while (true){
        char c = peek();
        switch (c){
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '#': // for comments
                while (peekNext != '\n' && !isAtEnd()){
                    advance();
                }
                break;

            default:
                return;

        }
    }
}
static Token string(){
    while (peek()!= '"'&& !isAtEnd()){
        if(peek() == '\n' )scanner.line ++;
        advance();
    }
    if  (isAtEnd) return errorToken("Unterminated string.");
    // for the closing quote.
    advance();
    return makeToken(TOKEN_STRING);
}
static Token number(){
    while (isDigit(peek())) advance();

    //fractions fuck me why is that even a thing

    if (peek() =='.' && isDigit(peekNext())){
        advance();

        while (isDigit(peek())) advance();

    }
    return makeToken(TOKEN_NUMBER);


}
static bool isDigit(char c){
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c){
    return ((c >= 'a' && c <='z') || (c >= 'A' && c <='Z') || c == '_');
}
static TokenType identifierType(){
    return TOKEN_IDENTIFIER;
}
static Token identifier(){
    while (isAlpha(peek()) || isDigit(peek())) advance;
    return makeToken(identifierType());
    // => what the fuck idk why not just return TOKEN_IDENTIFIER
    // maybe for reserved words or user made identifiers(i.e. variables)
}


Token scanToken(){
    skipWhitespace();// before even getting to a token just skip all ts
    //cooked
    scanner.start= scanner.current;
    if(isAtEnd()) return makeToken(TOKEN_EOF);
    
    char c = advance();
    if (isDigit(c)){
        return number();
    }
    if (isAlpha(c)){
        return identifier();
    }
    switch (c) {
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ';': return makeToken(TOKEN_SEMICOLON);
    case ',': return makeToken(TOKEN_COMMA);
    case '.': return makeToken(TOKEN_DOT);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);
    case '!':
      return makeToken(
          match_next('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return makeToken(
          match_next('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
      return makeToken(
          match_next('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
      return makeToken(
          match_next('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

    case '"': return string();
  }
    
    return errorToken("Unexpected character.");
}

