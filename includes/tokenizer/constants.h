#ifndef CONSTANTS_H
#define CONSTANTS_H

/* const int MAX_COLUMNS = 128;
const int MAX_ROWS = 100;
const int MAX_BUFFER = 200;

const char ALPHA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "><=!+-%&|^()/";
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char PUNC[] = "?.,:;'`~!";
const int START_DOUBLE = 0;
const int START_SPACES = 4;
const int START_ALPHA = 6;
const int START_OPERATOR = 20;
const int START_PUNC = 10;

//token types:
const int TOKEN_NUMBER = 3;
const int TOKEN_DIGITS = 2;
const int TOKEN_ALPHA = 4;
const int TOKEN_SPACE = 7;
const int TOKEN_OPERATOR = 6;
const int TOKEN_PUNC = 5;

const int TOKEN_UNKNOWN = -1;
const int TOKEN_END = -2; */

//-------------------------------------------
enum TokenType{
    TOKEN,
    LPAREN,
    RPAREN,
    TOKEN_STR,
    OPERATOR,
    RELATIONAL,
    LOGICAL,
    RESULT_SET,
    _TOKEN_NUMBER,
    _TOKEN_ALPHA,
    _TOKEN_SPACE,
    _TOKEN_OPERATOR,
    _TOKEN_PUNC,

    _TOKEN_UNKNOWN,
    _TOKEN_END
};

// error vaiable
enum Error{
    ERROR_ZERO,
    ERROR_ONE,
    ERROR_COMMA,
    ERROR_FIELD_NAME,
    ERROR_FROM,
    ERROR_TABLE_NAME,
    ERROR_CONDITIONS,
    ERROR_LEFT_PAREN,
    ERROR_RIGHT_PAREN,
    ERROR_VALUE,
    ERROR_NO_TABLE,
    ERROR_MISS_VALUE,
    ERROR_MISS_LOGICAL,
};

#endif