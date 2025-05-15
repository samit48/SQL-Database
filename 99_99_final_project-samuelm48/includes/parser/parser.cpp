#include "parser.h"
#include <algorithm>
#include <cctype>
//table
int Parser::state_table[MAX_ROWS][MAX_COLUMNS];
// CTOR
Parser::Parser()
{
}
// CTOR
Parser::Parser(char *s)
{
    input_q = s;
    make_table();
    build_keyword_list();
    set_string(s);
}
// tokenizes the command the rins get_parser_tree
void Parser::set_string(char s[])
{
    input.clear();
    ptree.clear();

    input_q = s;
    comma = 0;

    STokenizer stk(s);
    TOKENIZER t;
    stk >> t;

    while (stk.more())
    {
        if (t.token_str() == ",")
        {
            comma++;
        }

        if (t.type() != TOKEN_SPACE && t.type() != TOKEN_PUNC &&
            t.token_str() != "table" && t.token_str() != "into")
        {
            input.push_back(t.token_str());
        }
        t = TOKENIZER();
        stk >> t;
    }
    _fail = !get_parse_tree();
}

// gets what the type of token command it is
int Parser::get_column(string token)
{
    if (keyword.contains(token))
    {
        return keyword[token];
    }
    else
    {
        return SYMBOL;
    }
}
// this is the state machine that does basically everyhting
bool Parser::get_parse_tree()
{
    int current_state = 0;
    string token;

    for (int i = 0; i < input.size(); ++i)
    {
        token = input[i];
        if (current_state < 0)
        {
            ptree.clear();
            return false;
        }

        int col = get_column(token);
        //gets current state
        current_state = state_table[current_state][col];

        if (current_state < 0)
        {
            // this is a fail State
            if (ptree.at("from").empty())
            {
                throw 4;
            }
            if (ptree.at("table_name").empty())
            {
                throw 5;
            }
            if (ptree.at("condition").empty())
            {
                throw 6;
            }
            ptree.clear();
            return false;
        }

        //calls update_parse_tree which adds the tokens to the ptree
        if (!update_parse_tree(current_state, token))
        {
            ptree.clear();
            return false;
        }
    }

    if (state_table[current_state][0] == 1)
    {
        return true;
    }
    else
    {
        // this is a fail State
        if (ptree.at("from").empty())
        {
            throw 4;
        }
        if (ptree.at("table_name").empty())
        {
            throw 5;
        }
        if (ptree.at("condition").empty())
        {
            throw 6;
        }
        cout << "WY" << endl;
        ptree.clear();
        return false;
    }
}

//adds the token into the ptree respectivly
bool Parser::update_parse_tree(int state, const string &token)
{
    switch (state)
    {
    case 1:
        ptree["command"].push_back(token);
        break;
    case 2:
        ptree["fields"].push_back(token);
        break;
    case 3:
        ptree["from"].push_back(token);
        return true;
    case 4:
        ptree["table_name"].push_back(token);
        break;
    case 5:
        ptree["where"].push_back("yes");
        break;
    case 6:
        ptree["condition"].push_back(token);
        break;
    case 7:
        ptree["command"].push_back(token);
        break;
    case 8:
        ptree["table_name"].push_back(token);
        break;
    case 9:
        return true;
    case 10:
        ptree["col"].push_back(token);
        break;
    case 11:
        return true;
    case 12:
        ptree["values"].push_back(token);
        break;
    default:
        return false;
    }
    return true;
}
// makes a map of the keywords
void Parser::build_keyword_list()
{
    keyword.insert("select", 1);
    keyword.insert("from", 2);
    keyword.insert("where", 3);
    keyword.insert("*", 4);
    keyword.insert("values", 5);
    keyword.insert("fields", 6);
    keyword.insert("insert", 7);
    keyword.insert("make", 8);
    keyword.insert("symbol", 9);
}

//creates the table with all the states and row to jump to
void Parser::make_table()
{
    init_table(state_table);

    const int transitions[][3] = {
        {0, SELECT, 1}, {1, STAR, 2}, {2, FROM, 3}, {4, WHERE, 5}, {8, FIELDS, 9}, {0, MAKE, 7}, {0, INSERT, 7}, {8, VALUES, 11}, {1, SYMBOL, 2}, {2, SYMBOL, 2}, {3, SYMBOL, 4}, {5, SYMBOL, 6}, {6, SYMBOL, 6}, {7, SYMBOL, 8}, {9, SYMBOL, 10}, {10, SYMBOL, 10}, {11, SYMBOL, 12}, {12, SYMBOL, 12}};

    for (int i = 0; i < sizeof(transitions) / sizeof(transitions[0]); ++i)
    {
        int state = transitions[i][0];
        int input = transitions[i][1];
        int next_state = transitions[i][2];
        mark_cell(state, state_table, input, next_state);
    }

    const int success_states[] = {4, 6, 10, 12};
    const int fail_states[] = {0, 1, 2, 3, 5, 7, 8, 9, 11};

    for (int state : success_states)
    {
        mark_success(state_table, state);
    }

    for (int state : fail_states)
    {
        mark_fail(state_table, state);
    }
}

//returns ptree
mmap_ss Parser::parse_tree()
{
    if (!ptree.at("fields").empty() && (ptree.at("fields").size() - 1 != comma))
    {
        if (ptree.at("fields").size() == comma)
        {
            throw FIELD_NAME;
        }
        throw COMMA;
    }
    else if (!ptree.at("values").empty() && (ptree.at("values").size() - 1 != comma))
    {
        if (ptree.at("values").size() == comma)
        {
            throw 9;
        }
        throw COMMA;
    }

    return ptree;
}

bool Parser::fail()
{
    return _fail;
}