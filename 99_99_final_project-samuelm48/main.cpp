#include <iostream>
#include <iomanip>
#include "./includes/sql/sql.h"
//#include "./inclu"

using namespace std;

void readcommands(const std::string &filename, SQL &sql);
void handle_error(int e);

int main(int argv, char **argc)
{
    cout << "\n\n"
         << endl;
    SQL sql;
    Table t;
    string command;
    string filename = "commands.txt";
    sql._write = false;
    cout << "------ SQL Tables: --------" << endl;
    sql.read_tables("tables.txt");
    cout << "-------To print tables(cout << tables)-------" << endl;

    readcommands(filename, sql);
    for (int i = 1; i < 15; i++)
    {
        try
        {

            sql.run_batch(i);
        }
        catch (int e)
        {
            handle_error(e);
        }
        catch (...)
        {
            cerr << "\nAn unknown error occurred\n"
                 << endl;
        }
    }

    sql._write = true;
    sql.run_throw = true;

    while (true)
    {
        cout << "\n\nEnter a SQL command (or 'q' to quit): ";
        getline(cin, command);
        if (command == "q")
        {
            cout << "\n\nQUITTING SQL" << endl;
            break;
        }
        else if (command == "cout << tables")
        {
            sql.read_tables("tables.txt");
            continue;
        }

        try
        {
            t = sql.command(command);
            cout << t << endl;
        }
        catch (int e)
        {
            handle_error(e);
        }
        catch (...)
        {
            cerr << "\nAn unknown error occurred\n"
                 << endl;
        }

        /*         if (sql.is_valid() == true)
                {
                    cout << t << endl;
                }
                else
                {
                    cout << "\nINVALID COMMAND\nPlease try again\n" << endl;
                } */
    }

    cout << "\n=====================" << endl;
    return 0;
}

void readcommands(const std::string &filename, SQL &sql)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Unable to open file: " << filename << endl;
        return;
    }

    string line;
    while (std::getline(file, line))
    {
        // Check if the line starts with "//"
        if (line.find("//") != 0)
        {
            sql.command(line);
        }
    }

    file.close();
}

void handle_error(int e)
{
    if (e == ERROR_COMMA)
    {
        cout << "\nExpected: comma\nPlease try again\n"
             << endl;
    }
    else if (e == ERROR_FIELD_NAME)
    {
        cout << "\nExpected: field name\nPlease try again\n"
             << endl;
    }
    else if (e == 4)
    {
        cout << "\nExpected from\nPlease try again\n"
             << endl;
    }
    else if (e == 5)
    {
        cout << "\nNo table specified\nPlease try again\n"
             << endl;
    }
    else if (e == 6)
    {
        cout << "\nMissing Conditions\nPlease try again\n"
             << endl;
    }
    else if (e == 7)
    {
        cout << "\nMissing left parenthesis\nPlease try again\n"
             << endl;
    }
    else if (e == 8)
    {
        cout << "\nMissing right parenthesis\nPlease try again\n"
             << endl;
    }
    else if (e == 9)
    {
        cout << "\nExpected: value\nPlease try again\n"
             << endl;
    }
    else if (e == 10)
    {
        cout << "\nNo Such Table Found\n\nPlease try again\n"
             << endl;
    }
    else if(e == 11){
        cout << "\nMISSING VALUE IN CONDITION\n\nPlease try again\n" << endl;
    }else if(e == 12){
        cout << "\nMISSING LOGICAL\n\nPlease try again\n" << endl;
    }
    else
    {
        cout << "\nAn unknown integer error occurred: " << e << "\n"
             << endl;
    }
}
