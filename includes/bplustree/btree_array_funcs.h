#ifndef B_TREE_ARRAY_FUNCTIONS_H
#define B_TREE_ARRAY_FUNCTIONS_H
#include <cassert>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
using namespace std;

template <class T>
T maximal(const T &a, const T &b) // return the larger of the two items
{
    return max(a, b);
}

template <class T>
void swap(T &a, T &b) // swap the two items
{
    // swap(a,b);
    T temp = a;
    a = b;
    b = temp;
}

template <class T>
void print_array(const T data[], int n, int pos = -1) // print array data
{
    for (int i = 0; i < n; i++)
    {
        cout << data[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < n + 1; i++)
    {
        if (i == pos)
            cout << "^";
        cout << "  ";
    }
}

template <class T>
int index_of_maximal(T data[], int n) // return index of the largest item in data
{
    assert(n > 0);
    int max = 0;

    for (int i = 0; i < n; i++)
    {
        if (data[i] > data[max])
        {
            max = i;
        }
    }
    return max;
}

template <class T>                             // insert entry into the sorted array
void ordered_insert(T data[], int &n, T entry) // data with length n
{
    int i = n - 1;
    while (i >= 0 && data[i] > entry)
    {
        data[i + 1] = data[i];
        i--;
    }
    data[i + 1] = entry;
    n++;
}

template <class T>                                  // return the first element in data that is
int first_ge(const T data[], int n, const T &entry) // not less than entry
{
    for (int i = 0; i < n; i++)
    {
        if (entry <= data[i])
        {
            return i;
        }
    }
    return n;
}

template <class T>
void attach_item(T data[], int &n, const T &entry) // append entry to the right of data
{
    data[n] = entry;
    n++;
}

template <class T>
void insert_item(T data[], int i, int &n, T entry) // insert entry at index i in data
{
    assert(i >= 0 && i <= n);
    for (int j = n; j >= i; j--)
    {
        data[j + 1] = data[j];
    }
    data[i] = entry;
    n++;
}

template <class T>
void detach_item(T data[], int &n, T &entry) // remove the last element in data and place                                             // it in entry
{
    n--;
    entry = data[n];
}

template <class T>
void clear_list(T data[], int &n)
{
    for (int i = 0; i < n; i++)
    {
        data[i] = T();
    }
    n = 0;
}

template <class T>
void delete_item(T data[], int i, int &n, T &entry) // delete item at index i and place it in entry
{
    entry = data[i];
    n--;
    for (int j = i; j < n; j++)
    {
        data[j] = data[j + 1];
    }
}

template <class T>
void merge(T data1[], int &n1, T data2[], int &n2) // append data2 to the right of data1
{
    int j = 0;
    for (int i = n1; i < n1 + n2; i++)
    {
        data1[i] = data2[j];
        j++;
    }
    n1 += j;
    n2 = 0;
}

template <class T>
void split(T data1[], int &n1, T data2[], int &n2) // move n/2 elements from the right of data1
                                                   // and move to data2
{
    int mid = ceil(n1 / 2.0);
    int k = 0;
    for (int i = mid; i < n1; i++)
    {
        // insert_item(data2, k, n2, data1[i]);
        for (int j = n2; j >= k; j--)
        {
            data2[j + 1] = data2[j];
        }
        data2[k] = data1[i];
        n2++;
        k++;
    }
    n1 = mid;
}

template <class T>
void copy_array(T dest[], const T src[],
                int &dest_size, int src_size) // copy src[] into dest[]
{
    for (int i = 0; i < src_size; i++)
    {
        dest[i] = src[i];
    }
    dest_size = src_size;
}

template <class T>
bool is_gt(const T data[], int n, const T &item) // item > all data[i]
{
    for (int i = 0; i < n; i++)
    {
        if (item <= data[i])
        {
            return false;
        }
    }
    return true;
}

template <class T>
bool is_le(const T data[], int n, const T &item) // item <= all data[i]
{
    for (int i = 0; i < n; i++)
    {
        if (data[i] < item)
        {
            return false;
        }
    }
    return true;
}

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream &operator<<(ostream &outs, const vector<T> &list) // print vector list
{
    for (int i = 0; i < list.size(); i++)
    {
        cout << " " << list[i];
    }
    return outs;
}

template <typename T>
vector<T> &operator+=(vector<T> &list, const T &addme) // list.push_back addme
{
    list.push_back(addme);
    return list;
}

template <typename T>
vector<T> &operator+=(vector<T> &list, const vector<T> &rhs)
{
    for (int i = 0; i < rhs.size(); i++)
    {
        list.push_back(rhs[i]);
    }
    return list;
}

#endif
