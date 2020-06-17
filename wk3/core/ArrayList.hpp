// ArrayList.hpp
//
// ICS 45C Spring 2020
// Code Example
//
// This is a class declaration for our ArrayList class, which implements an
// array-based list of strings.  Our ArrayList has three member variables:
//
// * A pointer to a dynamically-allocated array of strings
// * An unsigned integer that keeps track of the list's size (i.e., how many
//   strings are currently stored in it)
// * Another unsigned integer that keeps track of the list's capacity (i.e.,
//   how big the current array is)
//
// It provides a modest amount of additional functionality, as well.  In a
// practical ArrayList class, we'd need more than this, but these demonstrate
// the most interesting parts of what you might realistically build.
//
// * Read values given an index
// * Assign to values given an index and a new value
// * Add a new string to the end of the list
// * Find out the list's size
// * Find out the list's capacity
//
// Our ArrayList is well-behaved, as well, which requires the Big Three,
// because we'll need to control what happens when an ArrayList is destroyed,
// copied, or assigned.  It's best not to write these if you don't explicitly
// need them -- for classes whose member variables are already well-behaved,
// writing them simply introduces risk with no benefit -- but we definitely
// need them here, since we have at least one member variable that is a
// pointer to dynamically-allocated storage that must be managed manually.

#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP

#include <string>
#include <gconv.h>


class ArrayList
{
public:
    // Initializing an ArrayList makes it be empty and have a smallish
    // initial capacity.
    ArrayList();


    // This is the declaration for a "copy constructor".  C++ automaticaly
    // invokes this whenever an ArrayList object is constructed as a copy of
    // an existing one (e.g., when a ArrayList is passed by value, when a
    // ArrayList variable is defined as a copy of an existing one).
    //
    // If you don't write a copy constructor, one is generated automatically,
    // which simply does a member-by-member copy of the member variables, in
    // the order that they're declared.  In our case, that's problematic,
    // because we don't want to copy the pointer to the array of strings; we
    // want to copy the array itself, so that the new ArrayList has a separate
    // array from the original one.  So we need our own copy constructor.
    ArrayList(const ArrayList& a);


    // This is the declaration for a "destructor", recognizable because its
    // name is the name of the class preceded by a "~" character (and because
    // there is no return type).  C++ automatically invokes this just before
    // an ArrayList object dies.  After the destructor runs, the member
    // variables are themselves destroyed, in the reverse of the order they're
    // declared in the class.  But that's not enough in our case, because we
    // don't just want the pointer to the dynamically-allocated array to be 
    // destroyed; we want the array itself to be destroyed.  The destructor
    // will do that.
    ~ArrayList();


    // This is the declaration for an "assignment operator".  In general, we
    // can redefine almost any operator in the C++ language by writing
    // functions whose names are "operator@" for any operator "@" (e.g.,
    // "operator=" for assignment, "operator+" for addition, "operator[]"
    // for indexing, etc.).
    //
    // Overloading operators requires understanding their underlying behavior
    // in more detail than you might be accustomed to.  In the case of
    // assignment, it's important to understand two things:
    //
    // (1) It's possible to do an assignment without modifying the object
    //     on the right-hand side, so we declare the parameter here to be
    //     const.
    // (2) Assignment is an expression that returns a value; specifically,
    //     assignment generally returns a reference to the object on the
    //     left-hand side (which we know will be an lvalue and, thus, can
    //     have a reference pointed to it).  While we could define our
    //     particular operator overload to return something different, it's
    //     best if we overload operators so they behave like their built-in
    //     counterparts, which follows the design "principle of least
    //     surprise".
    ArrayList& operator=(const ArrayList& a);


    // Unlike the version we wrote in lecture, the at() function is declared
    // here to return a reference to an actual string stored in the vector,
    // as opposed to returning a copy.  This has two effects:
    //
    // (1) It is a performance optimization, because it avoids copying the
    //     string in order to return it.
    // (2) It means we need different versions of the function for const
    //     ArrayList than for non-const ArrayList.  If the ArrayList is const,
    //     we have to return a const reference to one of its strings, because
    //     returning a non-const reference to the string would allow us to
    //     change it; if the ArrayList is non-const, we can return a non-const
    //     reference to the string and allow it to be changed.  C++ will
    //     automatically call the appropriate overload of the at() function
    //     depending on whether we call it on a const or non-const ArrayList.
    //
    // Given that at() returns a reference, we can then use it as either an
    // lvalue or an rvalue:
    //
    //     cout << a.at(3) << endl;
    //     a.at(3) = "Boo";
    //
    // However, the latter of the two examples will only be permitted if the
    // ArrayList is non-const, because it requires the returned string reference
    // not to be const (or else we won't be allowed to assign to it).
    std::string& at(unsigned int index);
    const std::string& at(unsigned int index) const;


    // add() adds a new string to the end of the ArrayList.  If the underlying
    // array is full, it is replaced by a new, larger array and the capacity
    // will grow as a result.
    void add(const std::string& s);


    // size() returns the size of the ArrayList.
    unsigned int size() const;


    // capacity() returns the capacity of the ArrayList.
    unsigned int capacity() const;

    //Move Constructor:
    ArrayList(ArrayList && a) noexcept;

    //Move Assignment Operator:
    ArrayList& operator=(ArrayList&& a) noexcept;

private:
    std::string* items;
    unsigned int sz;
    unsigned int cap;
};



#endif // ARRAYLIST_HPP

