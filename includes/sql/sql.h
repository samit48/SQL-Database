#ifndef SQL_H
#define SQL_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include "../../includes/parser/parser.h"
#include "../../includes/table/table.h"


class SQL
{    
public:
    SQL();
    SQL(const string& batch);
    Table command(const string& command);
    vectorlong select_recnos();
    void run_batch(int command_num);
    void read_tables(const string &filename);
    bool _write = false;
    bool run_throw = false;
    vectorstr t_names;
    void handle_error(int e);

private:
    void write_file(const string& comm, const string& filename);
    vectorlong recnos;
    Table table;
    mmap_ss ptree;  
    string batch_name;
    fstream f_batch;
    
    

};

#endif