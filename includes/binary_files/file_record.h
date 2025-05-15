#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;


class FileRecord {
public:
    FileRecord();
    FileRecord(char str[]);
    FileRecord(string s);
    FileRecord(vector<string> fields);

    long write(fstream& outs);
    long read(fstream& ins, long recno);

    friend ostream& operator<<(std::ostream& outs, const FileRecord& r);

//private:
    static const int MAX = 100;
    int recno;
    char _record[MAX + 1][MAX + 1];
};

bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]);
void open_fileW(fstream& f, const char filename[]);

#endif
