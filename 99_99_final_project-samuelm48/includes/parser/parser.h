#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "typedefs.h"
#include "../../includes/token/stokenize.h"
#include "../../includes/token/state_machine_functions.h"


enum KeyWords
{
    ZERO,
    SELECT,
    FROM,
    WHERE,
    STAR,
    VALUES,
    FIELDS,
    INSERT,
    MAKE,
    SYMBOL,
    COMMA
};

class Parser
{
public:
    Parser();
    Parser(char* s);
    void set_string(char s[]);
    int get_column(string token);
    bool get_parse_tree();
    void build_keyword_list();
    mmap_ss parse_tree();
    void make_table();
    bool fail();
    bool update_parse_tree(int state, const string& token);


private:
    map_sl keyword;
    vectorstr input;
    static int state_table[MAX_ROWS][MAX_COLUMNS];
    string input_q;
    bool _fail;
    int comma;
    mmap_ss ptree;
};

#endif