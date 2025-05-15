#include "table.h"
//CTOR
Table::Table()
{
    last_record = 0;
}

Table::Table(const string &name)
{
    last_record = 0;
    serial_number = 0;
    table_name = name;
    //
    fstream table_file;
    open_fileRW(table_file, (name + "_fields.txt").c_str());

    string line;
    vectorstr temp;
    //gets info from he file
    if (table_file.is_open())
    {
        while (getline(table_file, line))
        {
            temp.push_back(line);
        }
    }
    table_file.close();

    for (int i = 0; i < temp.size(); i++)
    {
        field_names.push_back(temp[i]); // add the field name to field_names 
        field_map[temp[i]] = i; // map the field name to its index in field_map
        indices.push_back(MMap<string, long>()); // initialize an empty index for each field
    }
     // rebuild the indices based on the current data
    reindex();
}

Table::Table(const string &name, vectorstr fields)
{
    table_name = name;
    serial_number = 0;
    last_record = 0;

    // set field names
    for (int i = 0; i < fields.size(); i++)
    {
        field_names.push_back(fields[i]);
        field_map[fields[i]] = i;
        indices.push_back(MMap<string, long>());
    }

    // create binary file
    ofstream binary_file(name + ".bin", ios::binary);

    // write field on txt
    ofstream text_file(name + "_fields.txt");
    if (text_file.is_open())
    {
        for (const string &field : fields)
        {
            text_file << field << endl;
        }
        text_file.close();
    }
    binary_file.close();
}

void Table::insert_into(const vectorstr field_name)
{
    fstream file(table_name + ".bin", ios::binary | ios::out | ios::in | ios::app);
    if (!file.is_open())
    {
        cout << "Error: Could not open file for writing." << endl;
        return;
    }

    // Create a FileRecord object with field values and write it to the file
    FileRecord record(field_name);
    last_record = record.write(file);

    // Update indices after writing the record
    for (int i = 0; i < field_name.size(); ++i)
    {
        indices[i][field_name[i]] += last_record;
    }
    file.close();
}
// i don't use this
Table Table::select(const vectorstr field_name, const string spc_field, const string condition, const string field_value)
{
    Table result_table(table_name + "_selected_" + to_string(++serial_number), field_name);

    int field_index = field_map[spc_field];
    vectorlong rec_num; 

    if (condition == "=")
    {
        rec_num = indices[field_index].at(field_value);
    }
    else if (condition == "<")
    {
        MMap<string, long>::Iterator it = indices[field_index].begin();
        for (; it != indices[field_index].end(); it++)
        {
            if ((*it).key < field_value)
            {
                rec_num += (*it).value_list;
            }
            else
            {
                break;
            }
        }
    }
    else if (condition == ">")
    {
        MMap<string, long>::Iterator it = indices[field_index].upper_bound(field_value);

        for (; it != indices[field_index].end(); it++)
        {
            rec_num += (*it).value_list;
        }
    }
    else if (condition == ">=")
    {
        MMap<string, long>::Iterator it = indices[field_index].lower_bound(field_value);
        for (; it != indices[field_index].end(); it++)
        {
            rec_num += (*it).value_list;
        }
    }
    else if (condition == "<=")
    {
        MMap<string, long>::Iterator it = indices[field_index].begin();
        for (; it != indices[field_index].upper_bound(field_value); it++)
        {
            rec_num += (*it).value_list;
        }
    }

    recons = rec_num;
    result_table.recons = recons;

    fstream file;
    FileRecord record;
    open_fileRW(file, (table_name + ".bin").c_str());
    long record_number = 0;
    long bytes = record.read(file, record_number);
    vectorstr result;

    while (bytes > 0)
    {
        if (find(rec_num.begin(), rec_num.end(), record_number) != rec_num.end())
        {
            for (int i = 0; i < field_name.size(); i++)
            {
                result.push_back(record._record[field_map[field_name[i]]]);
            }
            result_table.insert_into(result);
            result.clear();
        }
        record_number++;
        bytes = record.read(file, record_number);
    }
    file.close();
    return result_table;
}

Table Table::select(const vectorstr field_name, Queue<Token *> postfix)
{
    Table result_table(table_name + "_selected_" + to_string(++serial_number), field_names);
    // runs rpn and gets the reccurd numbers
    RPN rpn(postfix, indices, field_map);

    vectorlong rec_num = rpn();

    recons = rec_num;
    result_table.recons = recons;

    // changes the record numbers into a table
    fstream file;
    FileRecord record;
    open_fileRW(file, (table_name + ".bin").c_str());
    long record_number = 0;
    long bytes = record.read(file, record_number);
    vectorstr result;
    vectorstr field = field_name;
    if (field_name[0] == "*")
    {
        field = field_names;
    }
    

    while (bytes > 0)
    {

        if (find(rec_num.begin(), rec_num.end(), record_number) != rec_num.end())
        {
            for (int i = 0; i < field.size(); i++)
            {
                result.push_back(record._record[field_map[field[i]]]);
            }
            result_table.insert_into(result);
            result.clear();
        }
        record_number++;
        bytes = record.read(file, record_number);
    }
    file.close();
    return result_table;
}

Table Table::select(const vectorstr field_name, const vectorstr condition)
{
    //chnges the string into a token
    // does shunting yard to get postfix
    // then calles select
    Queue<Token *> converted = convert_to_tokens(condition);
    ShuntingYard sy(converted);
    Queue<Token *> postfix = sy.postfix();
    return select(field_name, postfix);
}
// need rec_num
Table Table::select(const vectorstr field_name)
{
    // makes a table of all elements with spcific field_names
    Table result_table(table_name + "_selected_" + to_string(++serial_number), field_name);
    fstream file;
    FileRecord record;
    open_fileRW(file, (table_name + ".bin").c_str());
    long record_number = 0;
    long bytes = record.read(file, record_number);
    vectorstr result;
    vectorlong rec_num;

    while (bytes > 0)
    {
        for (int i = 0; i < field_name.size(); i++)
        {
            result.push_back(record._record[field_map[field_name[i]]]);
        }
        result_table.insert_into(result);
        result.clear();
        rec_num.push_back(record_number);
        record_number++;

        bytes = record.read(file, record_number);
    }
    recons = rec_num;
    result_table.recons = recons;
    file.close();
    return result_table;
}

// converts string to token
Queue<Token *> Table::convert_to_tokens(const vectorstr &condition)
{
    Queue<Token *> tokens;

    for (int i = 0; i < condition.size(); i++)
    {
        string str = condition[i];
        if (str == "and" || str == "or")
        {
            tokens.push(new Logical(str));
        }
        else if (str == "=" || str == ">" || str == "<" || str == ">=" || str == "<=")
        {
            tokens.push(new Relational(str));
        }
        else if (str == ")")
        {
            tokens.push(new Rparen);
        }
        else if (str == "(")
        {
            tokens.push(new LParen);
        }
        else
        {
            tokens.push(new TokenStr(str));
        }
    }
    return tokens;
}

vectorlong Table::select_recnos()
{
    return recons;
}

// slect all elements and makes then retuens a tree
Table Table::select_all()
{
    Table result_table(table_name + "_selected_" + to_string(++serial_number), field_names);

    fstream file;
    FileRecord record;
    open_fileRW(file, (table_name + ".bin").c_str());
    long record_number = 0;
    long bytes = record.read(file, record_number);
    vectorstr result;
    recons.clear();

    while (bytes > 0)
    {
        for (int i = 0; i < field_names.size(); i++)
        {
            result.push_back(record._record[i]);
        }
        result_table.insert_into(result);
        result.clear();
        recons.push_back(record_number);
        record_number++;
        bytes = record.read(file, record_number);
    }
    file.close();
    result_table.recons = recons;
    return result_table;
}
//rebuilds the indecies
void Table::reindex()
{
    fstream data_file;
    FileRecord record;
    open_fileRW(data_file, (table_name + ".bin").c_str());

    long record_num = 0;
    long bytes_read = record.read(data_file, record_num);

    while (bytes_read > 0)
    {
        for (int field_index = 0; field_index < field_names.size(); field_index++)
        {
            // Get the value of the current field
            string field_value = record._record[field_index];
            // Update the index for the current field with the record number
            indices[field_index][field_value] += record_num;
        }

        record_num++;
        bytes_read = record.read(data_file, record_num);
    }
    last_record = record_num - 1;
    data_file.close();
}
//prints table
ostream &operator<<(ostream &outs, const Table &t)
{

    outs << endl
         << "Table name: " << t.table_name << ", " << "records: " << t.last_record + 1 << endl;

    vectorstr field_text;
    fstream file((t.table_name + "_fields.txt").c_str());
    string temp;
    if (file.is_open())
    {
        while (getline(file, temp))
        {
            field_text.push_back(temp);
        }
    }
    file.close();

    outs << setw(10) << "record";
    for (int i = 0; i < field_text.size(); i++)
    {
        outs << setw(20) << field_text[i];
    }
    outs << endl
         << endl;

    fstream binary_file;
    FileRecord record;
    open_fileRW(binary_file, (t.table_name + ".bin").c_str());
    int record_number = 0;
    long recno = record.read(binary_file, record_number);
    while (recno > 0)
    {
        outs << setw(10) << record_number << setw(20) << record << endl;
        record_number++;
        recno = record.read(binary_file, record_number);
    }
    binary_file.close();
    return outs;
}