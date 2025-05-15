#include "sql.h"
// CTOR
SQL::SQL() : batch_name("")
{
}
// CTOR
SQL::SQL(const string &batch)
{
    batch_name = batch;
}

const vector<string> command_list = {
    "",
    "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    "select lname fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    "select lname, , major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    "select lname, fname, major  student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    "select lname, fname, major from  where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    "select lname, fname, major from student where",
    "select lname, fname, major from student where (lname=Yang or major=CS) and age<23 )or lname=Jackson",
    "select lname, fname, major from student where ((lname=Yang or major=CS and age<23 )or lname=Jackson",
    "select lname, fname, major from student where ((lname= or major=CS) and age<23 )or lname=Jackson",
    "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    "select lname, fname, major from student where ((lname=Yang  major=CS) and age<23 )or lname=Jackson",
    "select lname, fname, major from student where ((lname=Yang or ) and age<23 )or lname=Jackson",
    "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or ",
    "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",

};

const int SELECT_COMMANDS = 15;
//runs the batch for testing
void SQL::run_batch(int command_num)
{
    SQL sql;
    Table t;

    cout << endl;

    cout << command_num << ") " << command_list[command_num] << endl;
    t = sql.command(command_list[command_num]);
    cout << "\n//valid command:" << endl;
}
//gets the command and changes it to ptree 
//based on the respective command it runs table 
Table SQL::command(const string &command)
{  
    // gets ptree from parsere tree()
    char s[500];
    strcpy(s, command.c_str());
    Parser parser(s);
    string filename = "commands.txt";
    ptree = parser.parse_tree();

    if (ptree.empty() && run_throw)
    {
        throw 0;
    }
    //gets everything from ptree
    vectorstr _command = ptree.at("command");
    string _table_name = ptree.at("table_name").front();
    vectorstr _col = ptree.at("col");
    vectorstr _fields = ptree.at("fields");
    vectorstr _values = ptree.at("values");
    vectorstr _condition = ptree.at("condition");
    vectorstr _where = ptree.at("where");

    if (find(t_names.begin(), t_names.end(), _table_name) == t_names.end() && run_throw)
    {
        throw 10;
    }
    // Executing make command
    if (_command[0] == "make" || _command[0] == "create")
    {
        if (_write)
        {
            write_file(command, filename);
            if (find(t_names.begin(), t_names.end(), _table_name) == t_names.end())
            {
                write_file(_table_name, "tables.txt");
            }
        }

        table = Table(_table_name, _col);
        return table;
    }
    else if (_command[0] == "insert")
    {
        // Executing insert command
        if (_write)
        {
            write_file(command, filename);
        }
        table = Table(_table_name);
        table.insert_into(_values);
        recnos.clear();
    }
    else if (_command[0] == "select")
    {
        // Executing select command
        table = Table(_table_name);
        if (_fields.front() == "*")
        {
            if (_where.empty())
            {
                Table temp = table.select_all();
                recnos = temp.select_recnos();

                return temp;
            }
            else
            {
                Table temp = table.select(_fields, _condition);
                recnos = temp.select_recnos();
                return temp;
            }

            // "Selecting all fields
        }
        else if (!ptree.at("condition").empty())
        {
            // Selecting with condition;
            Table temp = table.select(_fields, _condition);
            recnos = temp.select_recnos();
            return temp;
        }
        else
        {
            // Selecting specific fields
            Table temp = table.select(_fields);
            recnos = temp.select_recnos();
            return temp;
        }
    }
    else
    {
        throw runtime_error("Unknown command");
    }
    return table;
}

vectorlong SQL::select_recnos()
{
    return recnos;
}

void SQL::read_tables(const string &filename)
{
    // Create an ifstream object to open the file
    ifstream file(filename);

    // Check if the file was opened successfully
    if (!file.is_open())
    {
        cout << "Could not open the file: " << filename << endl;
        return;
    }

    string line;
    // Read the file line by line
    while (getline(file, line))
    {
        cout << line << endl;
        t_names.push_back(line);
    }

    // Close the file
    file.close();
}

void SQL::write_file(const string &comm, const string &filename)
{
    // Create an ofstream object to write to the file
    cout << "IM IN WRITE:" << endl;
    ofstream outFile(filename, ios::app);

    // Check if the file was successfully opened
    if (outFile.is_open())
    {
        outFile << "\n"
                << comm;
        // Write the content to the file

        // Close the file
        outFile.close();
    }
    else
    {
        cout << "Unable to open file " << filename << std::endl;
    }
}