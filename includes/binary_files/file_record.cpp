#include "file_record.h"

FileRecord::FileRecord()
{
    _record[0][0] = '\0';
    recno = -1;
}

FileRecord::FileRecord(char str[])
{
    strncpy(_record[0], str, MAX);
    _record[1][0] = '\0';
}

FileRecord::FileRecord(string s)
{
    strncpy(_record[0], s.c_str(), MAX);
    _record[0][s.size() + 1] = '\0';
}

FileRecord::FileRecord(vector<string> fields)
{
    int i = 0;
    for (; i < fields.size() && i <= MAX; ++i)
    {
        strncpy(_record[i], fields[i].c_str(), MAX);
        _record[i][MAX] = '\0';
    }
    for (; i <= MAX; ++i)
    {
        _record[i][0] = '\0';
    }
}

long FileRecord::write(fstream &outs)
{
    // r.write(f); //take the Record r and write it into file f
    //   and return the record number of this Record

    // write to the end of the file.
    outs.seekg(0, outs.end);

    long pos = outs.tellp(); // retrieves the current position of the
                             //       file pointer

    // outs.write(&record[0], sizeof(record));
    outs.write(_record[0], sizeof(_record));

    return pos / sizeof(_record); // record number
}

long FileRecord::read(fstream &ins, long recno)
{
    // returns the number of bytes read.
    //     r.read(f, 6);
    //     cout<<r<<endl;
    long pos = recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    // ins.read(&_record[0], sizeof(_record))
    ins.read(_record[0], sizeof(_record));
    // don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    // don't need the null character, but for those of us with OCD and PTSD:
    _record[0][ins.gcount()] = '\0';
    return ins.gcount();
}

ostream &operator<<(ostream &outs, const FileRecord &r)
{
    for (int i = 0; r._record[i][0] != '\0'; i++)
    {
        outs << setw(20) << r._record[i];
        outs << " ";
    }
    return outs;
}

bool file_exists(const char filename[])
{
    const bool debug = false;
    fstream ff;
    ff.open(filename,
            std::fstream::in | std::fstream::binary);
    if (ff.fail())
    {
        if (debug)
            cout << "file_exists(): File does NOT exist: " << filename << endl;
        return false;
    }
    if (debug)
        cout << "file_exists(): File DOES exist: " << filename << endl;
    ff.close();
    return true;
}

void open_fileRW(fstream &f, const char filename[])
{
    const bool debug = false;
    // opening a nonexistent file for in|out|app causes a fail()
    //   so, if the file does not exist, create it by openning it for
    //   output:
    if (!file_exists(filename))
    {
        // create the file
        f.open(filename, std::fstream::out | std::fstream::binary);
        if (f.fail())
        {
            cout << "file open (RW) failed: with out|" << filename << "]" << endl;
            throw("file RW failed  ");
        }
        else
        {
            if (debug)
                cout << "open_fileRW: file created successfully: " << filename << endl;
        }
    }
    else
    {
        f.open(filename,
               std::fstream::in | std::fstream::out | std::fstream::binary);
        if (f.fail())
        {
            cout << "file open (RW) failed. [" << filename << "]" << endl;
            throw("file failed to open.");
        }
    }
}

void open_fileW(fstream &f, const char filename[])
{
    f.open(filename,
           std::fstream::out | std::fstream::binary);
    if (f.fail())
    {
        cout << "file open failed: " << filename << endl;
        throw("file failed to open.");
    }
}
