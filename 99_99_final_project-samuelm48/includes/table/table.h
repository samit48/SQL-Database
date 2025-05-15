#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

#include "typedefs.h"
#include "../../includes/binary_files/file_record.h"
#include "../../includes/tokenizer/token.h"
#include "../../includes/rpn/rpn.h"
#include "../../includes/shunting_yard/shunting_yard.h"

using namespace std;

class Table
{
public:
    Table();
    Table(const string &name);
    Table(const string &name, vectorstr field_name);

    void insert_into(const vectorstr field_name);

    // select({"fname", "lname", "age"}, "lname", "=", "Yao");
    // select({fields}, specific field, equation(token?), value of specific field to search);
    Table select(const vectorstr field_name, const string spc_field, const string condition, const string field_value);

    // select({"lname", "age"}, post);
    // Queue<Token *> post;
    Table select(const vectorstr field_name, Queue<Token *> postfix);

    // select({"lname", "age"}, condition);
    // vector<string> condition = {"(", "age", "<", "17", "or", "age", ">", "20", ")", "and", "lname", "=", "Jackson"};
    Table select(const vectorstr field_name, const vector<string> condition);

    Table select(const vectorstr field_name);


    void reindex();

    vectorlong select_recnos();
    Table select_all();

    friend ostream &operator<<(ostream &outs, const Table &t);

    /* vectorstr get_field_names()
    {
        return field_names;
    } */
    string table_name;
private:
    Queue<Token*> convert_to_tokens(const vectorstr& condition);
    //string table_name;
    vector<mmap_sl> indices;
    vectorstr field_names;
    map_sl field_map;
    bool empty;
    vectorlong recons;
    // static
    long serial_number;
    long last_record;
};

#endif