#ifndef MULTI_MAP_H
#define MULTI_MAP_H
#include <iostream>
#include <iomanip>
#include <vector>
#include "../../includes/bplustree/btree_array_funcs.h"
#include "../../includes/bplustree/bplustree.h"
using namespace std;

template <typename K, typename V>
struct MPair
{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K &k = K())
    {
        key = k;
    }
    MPair(const K &k, const V &v)
    {
        key = k;
        value_list += v;
    }
    MPair(const K &k, const vector<V> &vlist) : key(k)
    {
        value_list.clear();
        value_list += vlist;
    }
    //--------------------------------------------------------------------------------

    // You'll need to overlod << for your vector:
    friend ostream &operator<<(ostream &outs, const MPair<K, V> &print_me)
    {
        outs << print_me.key << ":" << print_me.value_list;
        return outs;
    }

    friend bool operator==(const MPair<K, V> &lhs, const MPair<K, V> &rhs)
    {
        return lhs.key == rhs.key;
    }
    friend bool operator<(const MPair<K, V> &lhs, const MPair<K, V> &rhs)
    {
        return lhs.key < rhs.key;
    }
    friend bool operator<=(const MPair<K, V> &lhs, const MPair<K, V> &rhs)
    {
        return lhs.key <= rhs.key;
    }
    friend bool operator>(const MPair<K, V> &lhs, const MPair<K, V> &rhs)
    {
        return lhs.key > rhs.key;
    }
    friend MPair<K, V> operator+(const MPair<K, V> &lhs, const MPair<K, V> &rhs)
    {
        return MPair<K, V>(lhs.key, lhs.value_list + rhs.value_list);
    }
};

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator
    {
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it = NULL) : _it(it)
        {
        }
        Iterator operator++(int unused)
        {
            return _it++;
        }
        Iterator operator++()
        {
            return ++_it;
        }
        MPair<K, V> operator*()
        {
            return *_it;
        }
        friend bool operator==(const Iterator &lhs, const Iterator &rhs)
        {
            return (lhs._it == rhs._it);
        }
        friend bool operator!=(const Iterator &lhs, const Iterator &rhs)
        {
            return !(lhs._it == rhs._it);
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap();

    //  Iterators
    Iterator begin();
    Iterator end();

    //  Capacity
    int size() const;
    bool empty() const;

    //  Element Access
    const vector<V> &operator[](const K &key) const;
    vector<V> &operator[](const K &key);
    vector<V> &at(const K &key);

    //  Modifiers
    void insert(const K &k, const V &v);
    void erase(const K &key);
    void clear();

    //  Operations:
    bool contains(const K &key) const;
    vector<V> &get(const K &key);

    Iterator find(const K &key);
    int count(const K &key);
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
    Iterator lower_bound(const K &key);
    Iterator upper_bound(const K &key);

    void print_lookup();

    bool is_valid();

    friend ostream &operator<<(ostream &outs, const MMap<K, V> &print_me)
    {
        outs << print_me.mmap << endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};

template <typename K, typename V>
MMap<K, V>::MMap() : mmap(map_base())
{
}

//  Iterators
template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::begin()
{
    return mmap.begin();
}
template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::end()
{
    return mmap.end();
}

//  Capacity

template <typename K, typename V>
int MMap<K, V>::size() const
{
    return mmap.size();
}

template <typename K, typename V>
bool MMap<K, V>::empty() const
{
    return mmap.empty();
}

//  Element Access
template <typename K, typename V>
const vector<V> &MMap<K, V>::operator[](const K &key) const
{
    return (mmap.get(key)).value_list;
}

template <typename K, typename V>
vector<V> &MMap<K, V>::operator[](const K &key)
{
    return (mmap.get(key)).value_list;
}

//  Modifiers
template <typename K, typename V>
void MMap<K, V>::insert(const K &k, const V &v)
{
    mmap.insert(MPair<K, V>(k, v));
}
template <typename K, typename V>
void MMap<K, V>::erase(const K &key)
{
    mmap.remove(key);
}
template <typename K, typename V>
void MMap<K, V>::clear()
{
    mmap.clear_tree();
}

//  Operations:
template <typename K, typename V>
bool MMap<K, V>::contains(const K &key) const
{
    return mmap.contains(key);
}
template <typename K, typename V>
vector<V> &MMap<K, V>::get(const K &key)
{
    return mmap.get(key);
}

template <typename K, typename V>
vector<V> &MMap<K, V>::at(const K &key)
{
    return (mmap.get(key)).value_list;
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::find(const K &key)
{
    return mmap.find(key);
}

template <typename K, typename V>
int MMap<K, V>::count(const K &key)
{
    return 0;
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::lower_bound(const K &key)
{
    return mmap.lower_bound(key);
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::upper_bound(const K &key)
{
    return mmap.upper_bound(key);
}

template <typename K, typename V>
bool MMap<K, V>::is_valid()
{
    return true;
}

template <typename K, typename V>
void MMap<K, V>::print_lookup()
{
    for (MMap<K, V>::Iterator it = begin(); it != end(); ++it)
    {
        cout << setw(10) << (*it).key << " :" << setw(4) << " " << (*it).value_list << endl;
    }
}

#endif