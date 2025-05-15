#ifndef STOKENIZER_H
#define STOKENIZER_H
#include <iostream>

#include "../../includes/token/constants_stk.h"
#include "../../includes/token/tokenizer.h"

using namespace std;

class STokenizer
{
public:
    STokenizer();           // default ctor
    STokenizer(char str[]); // takes char array
    bool done();            // true: there are no more tokens
    bool more();            // true: there are more tokens
    //

    //---------------
    // extract one token (very similar to the way cin >> works)
    friend STokenizer &operator>>(STokenizer &s, TOKENIZER &t)
    {
        string str = "";
        int type = 0;
        s.get_token(type, str);
        t = TOKENIZER(str, type);
        return s;
    }

    // set a new string as the input string
    void set_string(char str[]);

private:
    // create table for all the tokens we will recognize
    //                       (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    // extract the longest string that match
    //      one of the acceptable token types
    bool get_token(int &start_state, string &token);
    //---------------------------------
    char _buffer[MAX_BUFFER]; // input string
    int _pos;                 // current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};

#endif