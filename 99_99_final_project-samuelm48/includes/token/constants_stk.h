#ifndef CONSTANTS_STK_H
#define CONSTANTS_STK_H

const int MAX_COLUMNS = 128;
const int MAX_ROWS = 100;
const int MAX_BUFFER = 200;

const char ALPHA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "><=!+-%&|^()/*";
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
const int TOKEN_END = -2;

#endif