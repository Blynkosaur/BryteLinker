#include "../../include/compiler/compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../include/compiler/scanner.h"

typedef struct
{
    Token current;
    Token previous;
    bool cooked;
    bool hadError;

} Parser;
Chunk *compilingChunk;

static Chunk *currentChunk()
{
    return compilingChunk;
}

Parser parser;



typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR,         // or
    PREC_AND,        // and
    PREC_EQUALITY,   // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * /
    PREC_UNARY,      // ! -
    PREC_CALL,       // . ()
    PREC_PRIMARY
} Precedence;
typedef void (*Parsefunc)(bool canAssign);
typedef struct
{
    Parsefunc prefix;
    Parsefunc infix;
    Precedence precedence;

}ParseRule;

static void expression();
static void statement();
static void declaration();
static bool check(TokenType type);
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);
static u_int8_t makeConstant(Value value);
static bool match(TokenType type);
static void consume(TokenType type, const char *message);
static void advance();
static void error(const char *message);
static void writeByte(uint8_t byte);

static void writeBytes(uint8_t byte1, uint8_t byte2);
static void parsePrecedence(Precedence precedence){
    advance();
    Parsefunc prefixRule = getRule(parser.previous.type)->prefix;
    if(prefixRule == NULL){
        error("Expression expected.");
        return;
    } 
    bool canAssign = precedence <= PREC_ASSIGNMENT;

    prefixRule(canAssign);
    while (precedence <= getRule(parser.current.type)->precedence){
        advance(); // --> parses the expression until finds something with lower precedence
        Parsefunc infixRule = getRule(parser.previous.type)->infix;
        infixRule(canAssign);
    }
    // if(canAssign && match(TOKEN_EQUAL)){
    //     error("invalid assignment target.");
    // }
}
// parser scans the tokens and transforms it into the tree/ syntax bullshit, just keep reading
//  do not read these comments please
static void errorAt(Token *token, const char *message)
{
    if (parser.cooked)
        return;
    fprintf(stderr, "[line %d] Error ", token->line);
    if (token->type == TOKEN_EOF)
    {
        fprintf(stderr, " at end");
    }
    else if (token->type == TOKEN_ERROR)
    {
        // Why is this nothing?? idk
    }
    else
    {
        fprintf(stderr, "at '%.*s'", token->length, token->start);
    }
    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
    parser.cooked = true;
}
static void error(const char *message)
{
    errorAt(&parser.previous, message);
}
static void errorAtCurrent(const char *message)
{
    errorAt(&parser.current, message);
}
static void advance()
{
    parser.previous = parser.current;
    while (1)
    {
        parser.current = scanToken(); // moves the current token forward
        if (parser.current.type != TOKEN_ERROR)// lwk retarded, only loops if there is an error
            
            break;

        errorAtCurrent(parser.current.start);
    }
}
static void literal(bool canAssign){
    switch (parser.previous.type){
        case TOKEN_FALSE:{
            writeByte(OP_FALSE);
            break;
        }case TOKEN_TRUE:{
            writeByte(OP_TRUE);
            break;
            
        }case TOKEN_NULL:{
            writeByte(OP_NULL);
            break;
        }default:
            return; 
    }// no need to advance anywhere since the prefix is automatically
    // consumed in "parsePrecendence"

}
static void expression()
{
    parsePrecedence(PREC_ASSIGNMENT);
}
static uint8_t identifierConstant(Token* token){
       return makeConstant(OBJ_VAL(copyString(token->start,token->length))); // stores the variable name to the chunk value array
}
static void defineVariable(uint8_t global){
    writeBytes(OP_DEFINE_GLOBAL, global);// OP_DEFINE_GLOBAL
}
static uint8_t parseVariable(const char* errorMessage){ //stores the variable name
       consume(TOKEN_IDENTIFIER, errorMessage);
        return identifierConstant(&parser.previous);//returns the index of the of the variable name on the value array
}
static void varDeclaration(){
    uint8_t global = parseVariable("Expected variable name");//index of name on value array
    if (match(TOKEN_EQUAL)){
        expression();//stores the variable value
    }
    else{
        writeByte(OP_NULL);
    }//allows simple variable declaration or declaration with assignment
    consume(TOKEN_SEMICOLON, "Expected ';' after variable declaration");

    defineVariable(global);
}
static void expressionStatement(){
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' after expression.");
    writeByte(OP_POP);
}
static void printStatement(){
    expression();
    consume(TOKEN_SEMICOLON,"Expect ';' after value.");
    writeByte(OP_PRINT);

}
static void synchronize(){
    parser.cooked = false;
    while (parser.current.type!= TOKEN_EOF){
        if (parser.previous.type == TOKEN_SEMICOLON) return;
        switch(parser.current.type){
            //basically safe to start scanning at the beginning of a statement
            case TOKEN_PRINT:
            case TOKEN_CLASS:
            case TOKEN_VAR:
            case TOKEN_FOR:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_RETURN:
            case TOKEN_FUN:
            return;
            
            default: ;

        }
    }

}
static void declaration(){
    if (match(TOKEN_VAR)){
        varDeclaration();
    }else{

    statement();
    }
    if (parser.cooked) synchronize();
}
static void statement(){
    if(match(TOKEN_PRINT)){
        printStatement();
    } else{
        expressionStatement();
    }
}
static void consume(TokenType type, const char *message)
{
    if (parser.current.type == type)
    {
        advance();
        return;
    }
    errorAtCurrent(message);
}
static bool match(TokenType type){
    if(!check(type)) return false;
    advance();
    return true;
}
static bool check(TokenType type){
   return parser.current.type == type; 
}

static void writeByte(uint8_t byte)
{
    writeChunk(currentChunk(), byte, parser.previous.line);
}
static void writeReturn()
{
    writeByte(OP_RETURN);
}
static u_int8_t makeConstant(Value value)
{
    int constant_index = addConstant(currentChunk(), value);
    // gives back the index on the constant array for the chunk
    if (constant_index > UINT8_MAX)
    {
        // since all bytecode are all 8 bits, max is 256 soo..... yeah the max array size is also 256 bytes
        // all bytecode is 8/bits one byte --> so the maximum index for the value array is also 256-1
        error("Too many constants in one chunk.");
        return 0;
    }
    return (uint8_t)constant_index;
}


static void writeBytes(uint8_t byte1, uint8_t byte2)
{
    writeByte(byte1);
    writeByte(byte2);
}

static void writeConstant(Value value)
{
    writeBytes(OP_CONSTANT, makeConstant(value));
}
static void endCompiler()
{
    writeReturn();
}

static void grouping(bool canAssign)
{
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}
static void unary(bool canAssign)
{
    TokenType operatorType = parser.previous.type;
    // expression();
    parsePrecedence(PREC_UNARY); // maybe take this off idk what this is
    switch (operatorType)
    {
    case TOKEN_MINUS:
        writeByte(OP_NEGATE);
        break;
    case TOKEN_BANG:
        writeByte(OP_NOT);
        break;
    default:
        return;
    }
}

static void binary(bool canAssign)
{
    TokenType operatorType = parser.previous.type;
    ParseRule *rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));
    switch (operatorType)
    {
    case TOKEN_PLUS:
        writeByte(OP_ADD);
        break;
    case TOKEN_MINUS:
        writeByte(OP_SUBSTRACT);
        break;
    case TOKEN_STAR:
        writeByte(OP_MULITPLY);
        break;
    case TOKEN_SLASH:
        writeByte(OP_DIVIDE);
        break;
    case TOKEN_BANG_EQUAL:
        writeBytes(OP_EQUAL, OP_NOT); break;
    case TOKEN_EQUAL_EQUAL: 
        writeByte(OP_EQUAL); break;
    case TOKEN_GREATER: 
        writeByte(OP_GREATER); break;
    case TOKEN_GREATER_EQUAL:
         writeBytes(OP_LESS,OP_NOT); break;
    case TOKEN_LESS: 
        writeByte(OP_LESS); break;
    case TOKEN_LESS_EQUAL: 
        writeBytes(OP_GREATER, OP_NOT); break;

    /* Just a small comment here, the reason why there are only 3 instructions: GREATER
    LESS AND EQUAL is cuz we can make all the other logic operators with them
    >= is the same as. !(<), >= the same as !(<) and != same as !(=)
    WHAAAAAT BLOWS MY MIND */
    default:
        return;
    }
}
static void number(bool canAssign)
{
    double value = strtod(parser.previous.start, NULL);
    writeConstant(NUMBER_VAL(value));
}

static void string(bool canAssign){
    writeConstant(OBJ_VAL(copyString(parser.previous.start+1, parser.previous.length-2)));
    // parser.previous is a token
    /*" a b c "
        |   |
    yeah idk "a" is start +1 and "c" is lenght -2 since not 0 indexed*/


}
static void namedVariable(Token name, bool canAssign){
    uint8_t arg = identifierConstant(&name);
    if(match(TOKEN_EQUAL)&& canAssign ){
        expression();
        writeBytes(OP_SET_GLOBAL,arg);
    }else{
        writeBytes(OP_GET_GLOBAL, arg);
    }
}
static void variable(bool canAssign){
    namedVariable(parser.previous,canAssign);
}
ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE}, 
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
  [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
  [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
  [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
  [TOKEN_BANG]          = {unary,     NULL,   PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,     binary,   PREC_EQUALITY},
  [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL_EQUAL]   = {NULL,     binary,   PREC_EQUALITY},
  [TOKEN_GREATER]       = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_GREATER_EQUAL] = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_LESS]          = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_LESS_EQUAL]    = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_IDENTIFIER]    = {variable,     NULL,   PREC_NONE},
  [TOKEN_STRING]        = {string,     NULL,   PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
  [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {literal,     NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NULL]           = {literal,     NULL,   PREC_NONE},
  [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_TRUE]          = {literal,     NULL,   PREC_NONE},
  [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};
static ParseRule* getRule(TokenType type){
    return &rules[type];
}


bool compile(const char *source, Chunk *chunk)
{
    initScanner(source);
    compilingChunk = chunk;
    parser.hadError = false;
    parser.hadError = false;
    advance();
    while (!match(TOKEN_EOF)){
        declaration();
    }
        endCompiler();
        
    #ifdef DEBUG_PRINT_CODE
    #include "../../include/debug.h"
    if(!parser.hadError){
        disassembleChunk(currentChunk(),"code");
    }
    #endif

    return !parser.hadError;
}
