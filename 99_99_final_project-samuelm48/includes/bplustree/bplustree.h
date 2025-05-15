#ifndef B_TREE_PLUS_H
#define B_TREE_PLUS_H
#include <iostream>
#include <iomanip>
#include <cassert>
#include "../../includes/bplustree/btree_array_funcs.h"

using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator
    {
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T> *_it = NULL, int _key_ptr = 0) : it(_it), key_ptr(_key_ptr) {}

        T operator*()
        {
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used)
        {
            Iterator temp = *this;
            key_ptr++;
            if (key_ptr == it->data_count)
            {
                key_ptr = 0;
                it = it->next;
            }
            return temp;
        }
        Iterator operator++()
        {
            key_ptr++;
            if (key_ptr == it->data_count)
            {
                key_ptr = 0;
                it = it->next;
            }
            return *this;
        }
        friend bool operator==(const Iterator &lhs, const Iterator &rhs)
        {
            return lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr;
        }
        friend bool operator!=(const Iterator &lhs, const Iterator &rhs)
        {
            return !(lhs == rhs);
        }
        void print_Iterator()
        {
            if (it)
            {
                cout << "iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else
            {
                cout << "iterator: NULL, key_ptr: " << key_ptr << endl;
            }
        }
        bool is_null() { return !it; }
        void info()
        {
            cout << endl
                 << "Iterator info:" << endl;
            cout << "key_ptr: " << key_ptr << endl;
            cout << "it: " << *it << endl;
        }

    private:
        BPlusTree<T> *it;
        int key_ptr;
    };

    BPlusTree(bool dups = false);
    BPlusTree(T *a, int size, bool dups = false);

    // big three:
    BPlusTree(const BPlusTree<T> &other);
    ~BPlusTree();
    BPlusTree<T> &operator=(const BPlusTree<T> &RHS);

    void copy_tree(const BPlusTree<T> &other); // copy other into this object
    void copy_tree(const BPlusTree<T> &other, BPlusTree<T> *&last_node);

    void insert(const T &entry); // insert entry into the tree
    void remove(const T &entry); // remove entry from the tree
    void clear_tree();           // clear this object (delete all nodes etc.)

    bool contains(const T &entry) const; // true if entry can be found
    T &get(const T &entry);              // return a reference to entry
    const T &get(const T &entry) const;  // return a reference to entry
    T &get_existing(const T &entry);     // return a reference to entry
    Iterator find(const T &key);         // return an iterator to this key.
                                         //      NULL if not there.
    Iterator lower_bound(const T &key);  // return first that goes NOT BEFORE
                                         //  key entry or next if does not
                                         //  exist: >= entry
    Iterator upper_bound(const T &key);  // return first that goes AFTER key
                                         // exist or not, the next entry  >entry

    int size() const // count the number of elements
    {
        int size = 0;
        if (!is_leaf())
        {
            for (int i = 0; i < child_count; i++)
            {
                size += subset[i]->size();
            }
            size += data_count;
        }
        else
        {
            return data_count;
        }
        return size;
    }
    bool empty() const // true if the tree is empty
    {
        return data_count == 0;
    }
    void print_tree(int level = 0,
                    ostream &outs = cout) const;
    friend ostream &operator<<(ostream &outs,
                               const BPlusTree<T> &print_me)
    {
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid()
    {
        return true;
    }
    string in_order();
    string pre_order();
    string post_order();

    Iterator begin() { return Iterator(get_smallest_node()); }
    Iterator end() { return Iterator(NULL); }
    ostream &list_keys(Iterator from = NULL, Iterator to = NULL)
    {
        if (from == NULL)
            from = begin();
        if (to == NULL)
            to = end();
        for (Iterator it = from; it != to; it++)
            cout << *it << " ";
        return cout;
    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                   // true if duplicate keys are allowed
    int data_count;                 // number of data elements
    T data[MAXIMUM + 1];            // holds the keys
    int child_count;                // number of children
    BPlusTree *subset[MAXIMUM + 2]; // subtrees
    BPlusTree *next;
    bool is_leaf() const
    {
        return child_count == 0;
    } // true if this is a leaf node

    T *find_ptr(const T &entry); // return a pointer to this key.
                                 //  NULL if not there.

    // insert element functions
    void loose_insert(const T &entry); // allows MAXIMUM+1 data elements in
                                       //    the root
    void fix_excess(int i);            // fix excess in child i

    // remove element functions:
    void loose_remove(const T &entry); // allows MINIMUM-1 data elements
                                       //   in the root

    BPlusTree<T> *fix_shortage(int i); // fix shortage in child i
    // and return the smallest key in this subtree
    BPlusTree<T> *get_smallest_node();
    void get_smallest(T &entry);                 // entry := leftmost leaf
    void get_biggest(T &entry);                  // entry := rightmost leaf
    void remove_biggest(T &entry);               // remove the biggest child of tree->entry
    void transfer_left(int i);                   // transfer one element LEFT from child i
    void transfer_right(int i);                  // transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i); // merge subset i with  i+1
};

// CTOR
template <class T>
BPlusTree<T>::BPlusTree(bool dups)
{
    dups_ok = dups;
    data_count = 0;
    child_count = 0;
    next = NULL;
}

template <class T>
BPlusTree<T>::BPlusTree(T *a, int size, bool dups)
{
    dups_ok = dups;
    data_count = 0;
    child_count = 0;
    next = NULL;
    for (int i = 0; i < size; i++)
    {
        insert(a[i]);
    }
}

// big three:
template <class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T> &other)
{
    copy_tree(other);
}

template <class T>
BPlusTree<T>::~BPlusTree()
{
    clear_tree();
}

template <class T>
BPlusTree<T> &BPlusTree<T>::operator=(const BPlusTree<T> &RHS)
{
    if (this == &RHS)
    {
        return *this;
    }

    clear_tree();
    copy_tree(RHS);

    return *this;
}

template <class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T> &other) // copy other into this object
{
    // assert(empty() && "Copy Tree BPlusTree is empty");
    BPlusTree<T> *temp = NULL;
    copy_tree(other, temp);
}

// Copy the contents of another tree into this tree, keeping track of the last node
// works recursivvely as long as it is not a leaf copy
// if it is a leaf link the previous link node to this node
template <class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T> &other, BPlusTree<T> *&last_node)
{
    dups_ok = other.dups_ok;
    child_count = other.child_count;
    copy_array(data, other.data, data_count, other.data_count);
    if (!other.is_leaf())
    {
        for (int i = 0; i < other.child_count; i++)
        {
            subset[i] = new BPlusTree<T>;
            subset[i]->copy_tree(*other.subset[i], last_node);
        }
    }
    else
    {
        if (last_node != NULL)
        {
            last_node->next = this;
        }
        last_node = this;
    }
}

template <class T>
void BPlusTree<T>::insert(const T &entry) // insert entry into the tree
{
    loose_insert(entry);
    if (data_count > MAXIMUM)
    {
        // in this case (we have excess in the root)
        // create new node copy the call fix_excess
        BPlusTree<T> *new_root = new BPlusTree<T>;
        new_root->copy_tree(*this);
        clear_tree();
        subset[0] = new_root;
        data_count = 0;
        child_count = 1;
        fix_excess(0);
    }
}

template <class T>
void BPlusTree<T>::loose_insert(const T &entry) // allows MAXIMUM+1 data elements in the root
{
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count && data[i] == entry);

    // inserts element into the data
    if (is_leaf())
    {
        if (found)
        {
            data[i] = entry;
            return;
        }
        else
        {
            ordered_insert(data, data_count, entry); // not sure how to insert
        }
    }
    else
    {
        if (found)
        {
            subset[i + 1]->loose_insert(entry);
            if (subset[i + 1]->data_count > MAXIMUM)
            {
                fix_excess(i + 1);
            }
        }
        else
        {
            subset[i]->loose_insert(entry);
            if (subset[i]->data_count > MAXIMUM)
            {
                fix_excess(i);
            }
        }
    }
}
template <class T>
void BPlusTree<T>::fix_excess(int i) // fix excess in child i
{
    //1. add a new subset at location i+1 of this node
    //2. split subset[i] (both the subset array and the data array) and move half into
    //      subset[i+1] (this is the subset we created in step 1.)
    //3. detach the last data item of subset[i] and bring it and insert
    //      it into this node's data[]
    BPlusTree<T> *new_subset = new BPlusTree<T>;
    insert_item(subset, i + 1, child_count, new_subset);
    split(subset[i]->data, subset[i]->data_count, new_subset->data, new_subset->data_count);
    split(subset[i]->subset, subset[i]->child_count, new_subset->subset, new_subset->child_count);
    T item;
    detach_item(subset[i]->data, subset[i]->data_count, item);
    ordered_insert(data, data_count, item);
    if (subset[i]->is_leaf())
    {
        //  transfer the middle entry to the right and...
        //  Deal with next pointers. just like linked list insert
        insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, item);
        subset[i + 1]->next = subset[i]->next;
        subset[i]->next = subset[i + 1];
    }
}

template <class T>
void BPlusTree<T>::remove(const T &entry) // remove entry from the tree
{
    //Loose_remove the entry from this tree.
    //Shrink if you have to
    loose_remove(entry);
    if (data_count == 0 && child_count == 1)
    {
        BPlusTree<T> *shrink_ptr = subset[0];
        copy_tree(*shrink_ptr);
        shrink_ptr->child_count = 0;
        shrink_ptr->data_count = 0;
        delete shrink_ptr;
    }
}

template <class T>
void BPlusTree<T>::loose_remove(const T &entry)
{
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count && data[i] == entry);
    //basiclly searches for the element and delets it
    //if node is not in the leaf but found go to net subset
    //when comming back fix any issues 
    if (is_leaf())
    {
        if (!found)
        {
            return;
        }
        else
        {
            T item;
            delete_item(data, i, data_count, item);
        }
    }
    else
    {
        if (!found)
        {
            subset[i]->loose_remove(entry);
            if (subset[i]->data_count < MINIMUM)
            {
                fix_shortage(i);
            }
        }
        else
        {
            // assert(i < child_count-1);
            subset[i + 1]->loose_remove(entry);
            if (subset[i + 1]->data_count < MINIMUM)
            {
                fix_shortage(i + 1);
            }
        }
    }
}

// not fully done
template <class T>
BPlusTree<T> *BPlusTree<T>::fix_shortage(int i)
{

    if (i + 1 < child_count && subset[i + 1]->data_count > MINIMUM)
    {
        transfer_left(i + 1);
    }
    else if (i > 0 && subset[i - 1]->data_count > MINIMUM)
    {
        transfer_right(i - 1);
    }
    else if (i < child_count - 1)
    {
        merge_with_next_subset(i - 1);
    }
    else
    {
        merge_with_next_subset(i);
    }

    if ((i < child_count - 1) && (subset[i + 1]->data_count > MINIMUM))
    {
        // borrow from right subtree if you can

        transfer_left(i + 1);
        return subset[i];
    }

    return subset[i];
}
// 2 functions below need work
template <class T>
void BPlusTree<T>::transfer_left(int i)
{
}
template <class T>
void BPlusTree<T>::transfer_right(int i)
{
}

//Merge subset[i] with subset [i+1] REMOVE data[i];
//leaves  : delete but do not bring down data[i]
template <class T>
BPlusTree<T> *BPlusTree<T>::merge_with_next_subset(int i)
{
    // 1. remove data[i] from this object
    //   2. if not a leaf, append it to child[i]->data:
    //   3. Move all data items from subset[i+1]->data to right of subset[i]->data
    //   4. Move all subset pointers from subset[i+1]->subset to
    //          right of subset[i]->subset
    //   5. delete subset[i+1] (store in a temp ptr)
    //   6. if a leaf, point subset[i]->next to temp_ptr->next
    //  6. delete temp ptr
    BPlusTree<T> *temp_tree;
    T entry;
    delete_item(data, i, data_count, entry);
    ordered_insert(subset[i]->data, subset[i]->data_count, entry);
    merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
    merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);

    if (is_leaf())
    {
        delete_item(subset, i + 1, child_count, temp_tree);
        delete temp_tree;
    }
    return subset[i];
}

template <class T>
void BPlusTree<T>::clear_tree() // clear this object (delete all nodes etc.)
{
    // goes to each leaf and clears it
    for (int i = 0; i < child_count; i++)
    {
        if (!is_leaf())
        {
            subset[i]->clear_tree();
        }
        else
        {
            delete subset[i];
            subset[i] = NULL;
        }
    }
    child_count = 0;
    data_count = 0;
}

template <class T>
bool BPlusTree<T>::contains(const T &entry) const // true if entry can be found
{
    int index = first_ge(data, data_count, entry);
    bool found = (index < data_count && data[index] == entry);
    // goes through the tree if found in leaf return
    // found but not in leaf go to the next subset
    // if not found in leaf return null
    if (found && is_leaf())
    {
        return true;
    }
    else if (found && !is_leaf())
    {
        return subset[index + 1]->contains(entry);
    }
    else if (!found && !is_leaf())
    {
        return subset[index]->contains(entry);
    }
    else
    {
        return false;
    }
}
template <class T>
T &BPlusTree<T>::get(const T &entry) // return a reference to entry
{
    if (!contains(entry))
    {
        insert(entry);
    }
    return get_existing(entry);
}
template <class T>
const T &BPlusTree<T>::get(const T &entry) const // return a reference to entry
{
    if (!contains(entry))
    {
        insert(entry);
    }
    return *find(entry);
}
template <class T>
T &BPlusTree<T>::get_existing(const T &entry) // return a reference to entry
{
    int index = first_ge(data, data_count, entry);
    bool found = (index < data_count && data[index] == entry);
    // goes through the tree if found in leaf return
    // found but not in leaf go to the next subset
    // if not found in leaf return null
    if (found && is_leaf())
    {
        return data[index];
    }

    else if (found && !is_leaf())
    {
        return subset[index + 1]->get_existing(entry);
    }

    else if (!found && is_leaf())
    {
        assert(found);
    }

    return subset[index]->get_existing(entry);
}
template <class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T &key) // return an iterator to this key.
                                                                 //      NULL if not there.
{
    int index = first_ge(data, data_count, key);
    bool found = (index < data_count && data[index] == key);
    // goes through the tree if found in leaf return
    // found but not in leaf go to the next subset
    // if not found in leaf return null
    if (found && !is_leaf())
    {
        return subset[index + 1]->find(key);
    }
    else if (!found && !is_leaf())
    {
        return subset[index]->find(key);
    }
    else if (!found && is_leaf())
    {
        return Iterator(NULL, 0);
    }

    return Iterator(this, index);
}

template <class T>
T *BPlusTree<T>::find_ptr(const T &entry)
{
    int index = first_ge(data, data_count, entry);
    bool found = (index < data_count && data[index] == entry);
    // goes through the tree if found in leaf return
    // found but not in leaf go to the next subset
    // if not found in leaf return null
    if (is_leaf())
    {
        if (found)
        {
            return &data[index];
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        if (found)
        {
            return subset[index + 1]->find_ptr(entry);
        }
        else
        {
            return subset[index]->find_ptr(entry);
        }
    }
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T &key)
{
    int i = first_ge(data, data_count, key);
    bool found = (i < data_count && data[i] == key);
    // When fond return iterator
    if (found && is_leaf())
    {
        return Iterator(this, i);
    }
    // If the key is found but not in leaf search in the right child
    else if (found && !is_leaf())
    {
        return subset[i + 1]->lower_bound(key);
    }
    // If the key is not found and we're in a leaf node, find the appropriate position
    else if (!found && is_leaf())
    { // If the key is not found and we're in a leaf node, find the appropriate position
        if (is_le(data, data_count, key))
        {
            Iterator it = Iterator(this, i);
            {
                return it;
            }
        }
        else
        { // If the key is greater, return the iterator to the previous position
            return Iterator(this, i - 1);
        }
    }
    else if (!found && !is_leaf())
    {
        // recurse into the appropriate subtree
        Iterator it = subset[i]->lower_bound(key);
        if (it != Iterator(NULL) && key > *it)
        {
            it++;
        }
        return it;
    }
    else
    {
        cout << "UNKOWN ERROR " << endl;
        return Iterator(this, 0);
    }
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T &key)
{
    // goes through the tree trying to find element using breadcrumbs
    int i = first_ge(data, data_count, key);
    bool found = (i < data_count && data[i] == key);
    // When fond return iterator
    if (found && is_leaf())
    {
        return Iterator(this, i);
    }
    // If the key is found but not in leaf search in the right child
    else if (found && !is_leaf())
    {
        Iterator it = subset[i + 1]->upper_bound(key);
        if (it != Iterator(NULL) && *it <= key)
        {
            it++;
        }
        return it;
    }
    // If the key is not found and we're in a leaf node, find the appropriate position
    else if (!found && is_leaf())
    { // If the key is greater than all elements in the node Otherwise, return the iterator to the current positio
        if (is_gt(data, data_count, key))
        {
            return Iterator(this, i - 1);
        }
        else
        {
            return Iterator(this, i);
        }
    }
    else if (!found && !is_leaf())
    {
        // recurse into the appropriate subtree
        Iterator it = subset[i]->upper_bound(key);
        // Ensure the iterator is correctly positioned
        if (it != Iterator(NULL) && *it <= key)
        {
            it++;
        }
        return it;
    }
    else
    {
        cout << "UNKOWN ERROR " << endl;
        return Iterator(this, 0);
    }
}
// returns a string in in_order
template <class T>
string BPlusTree<T>::in_order()
{
    string str;
    if (!is_leaf())
    {
        for (int i = 0; i < child_count; i++)
        {
            str += subset[i]->in_order();
            if (i < data_count)
            {
                str += to_string(data[i]) + "|";
            }
        }
    }
    else
    {
        for (int i = 0; i < data_count; i++)
        {
            str += to_string(data[i]) + "|";
        }
    }
    return str;
}
// returns a string in pre_order
template <class T>
string BPlusTree<T>::pre_order()
{
    string str = "";
    for (int i = 0; i < data_count; i++)
    {
        str += to_string(data[i]) + "|";
        if (!is_leaf())
        {
            if (i == 0)
            {
                str += subset[i]->pre_order();
            }
            str += subset[i + 1]->pre_order();
        }
    }
    return str;
}
// returns a string in post_order
template <class T>
string BPlusTree<T>::post_order()
{
    string str = "";
    for (int i = 0; i < data_count; i++)
    {
        if (!is_leaf())
        {
            if (i == 0)
                str += subset[i]->post_order();
            str += subset[i + 1]->post_order();
        }
        str += to_string(data[i]) + "|";
    }
    return str;
}

// returns smallest node
// goes to the first index element tell it hits a leaf
template <class T>
BPlusTree<T> *BPlusTree<T>::get_smallest_node()
{
    if (!is_leaf())
    {
        return subset[0]->get_smallest_node();
    }
    else
    {
        return this;
    }
}

// prints the btree
template <class T>
void BPlusTree<T>::print_tree(int level, ostream &outs) const
{
    if (!is_leaf())
    {
        subset[child_count - 1]->print_tree(level + 1, outs);
    }

    for (int i = data_count - 1; i >= 0; i--)
    {
        /* if (data[i] != nullptr)
        {
             cout << setw(4 * level) << "^" << endl;
        } */

        cout << setw(4 * level) << "[" << data[i] << "]" << endl
             << endl;
        if (!is_leaf())
        {
            subset[i]->print_tree(level + 1, outs);
        }
    }
}

#endif