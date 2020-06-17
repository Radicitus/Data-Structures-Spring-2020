// ArrayList.cpp
//
// ICS 45C Spring 2020
// Code Example
//
// This is an implementation of our ArrayList class.

#include "ArrayList.hpp"



// This notation is how you introduce definitions in a source file but hide
// them from other source files.  It puts the definitions into what's called
// the "unnamed namespace", which limits their availability only to this
// source file.  Even if another source file attempts to declare them, the
// other source file's declarations will be considered different from these;
// the linker will not be able to see anything we define in the unnamed
// namespace, which is a nice way to ensure that we don't have name collisions
// between small, local utilities like these.
namespace
{
    // The default size of our ArrayList when they're first created
    const unsigned int initialCapacity = 10;


    // arrayCopy() is a utility function that copies "size" elements from
    // the "source" array into the "target" array.  It'll have use in a
    // few places throughout our implementation, so we'll pull it out and
    // make a function out of it.
    //
    // Note that this function could have been a private member function
    // of the ArrayList class, but my general goal is to reduce the number
    // of member functions -- and, hence, the apparent complexity of a
    // class, from the perspective of code that uses the class -- so I
    // only make a function into a member function if I need to.
    void arrayCopy(std::string* target, std::string* source, unsigned int size)
    {
        for (unsigned int i = 0; i < size; i++)
        {
            target[i] = source[i];
        }
    }
}



// Note that I've added commented-out lines that write output to std::cout that
// display a message when the constructor, copy constructor, destructor, and
// assignment operator are called.  Uncomment these and experiment with using
// the ArrayList class in various ways, so you can see which ones are called in
// which circumstances.



// The constructor need only allocate an array, set the size to 0, and set the
// capacity to whatever the length of the array is.  We can do all of that in
// the initializers, so there's no need for any code in the body (though I
// included console output, commented-out, for you to experiment with).

ArrayList::ArrayList()
    : items{new std::string[initialCapacity]}, sz{0}, cap{initialCapacity}
{
//    std::cout << "ArrayList::ArrayList()" << std::endl;
}


// The copy constructor initializes a new ArrayList to be a copy of an existing
// one, which necessitates a few things: (1) allocating a new array with the
// same capacity as the original, (2) copying the size and capacity into the
// new ArrayList, and (3) copying the individual elements from the original
// ArrayList into the new one.
//
// One thing to notice in this function (and in the assignment operator) is
// that we can legally access the private member variables of a ArrayList other
// than "this".  That's because access modifiers like "private" apply to a
// *class*, as opposed to *objects* of a class.  Code within a class can
// access *any* private member of *any* object of that class.  (And this is
// a sensible way to do business, because we trust the ArrayList class to know
// how to appropriately manage these member variables.)

ArrayList::ArrayList(const ArrayList& a)
    : items{new std::string[a.cap]}, sz{a.sz}, cap{a.cap}
{
//    std::cout << "ArrayList::ArrayList(const ArrayList&)" << std::endl;
    arrayCopy(items, a.items, sz);
}


// The destructor has only one job to do: deallocate the dynamically-allocated
// array of strings belonging to this ArrayList.  The member variables
// themselves (items, sz, and cap) will be destroyed automatically -- in the
// reverse of the order they're declared in the ArrayList class declaration
// -- just after the destructor ends, so we don't want to delete them here.

ArrayList::~ArrayList()
{
//    std::cout << "ArrayList::~ArrayList()" << std::endl;
    delete[] items;
}


// The assignment operator is trickier than the others in the Big Three,
// because of two bits of complexity:
//
// (1) It returns a reference to the ArrayList that's been assigned into.
//     Within any member function in ArrayList, there is a pointer called
//     "this" that points to the ArrayList the member function was called
//     on; in the case of the assignment operator, that's the ArrayList
//     that's been assigned into.  But we can't "return this" at the
//     end of the function, because "this" is a pointer and not a
//     reference.  To return a reference, we need to return an actual
//     ArrayList object, namely the one that "this" points to.  How do we
//     access the ArrayList object that "this" points to?  "*this".  So
//     we end the function with "return *this".
//
// (2) Self-assignment (assignment from an object into itself) is
//     possible:
//
//         ArrayList a1;
//         ...
//         a1 = a1;
//
//     In this case, the assignment should simply have no effect.  But how
//     do we know that it's self-assignment?  We need to determine if the
//     object being assigned into has the same address as the object being
//     assigned from.  "this" is a pointer (the address!) of the object
//     being assigned into.  "a" is a reference (not the address, but
//     effectively the object!) to the object being assigned from.  However,
//     we can get the address of the object being assigned from by asking
//     for it: "&a".  So we'll check if "this" is different from "&a"; if
//     so, it's not self-assignment.
//
//     Why the self-assignment check is so important is partly a performance
//     optimization -- avoiding work that doesn't need to be done if the
//     two objects are really the same -- and partly a correctness issue,
//     if the assignment operator, for example, destroys the current contents
//     of the object and then tries to replace them with a copy of the contents
//     of the other.

ArrayList& ArrayList::operator=(const ArrayList& a)
{
//    std::cout << "ArrayList& ArrayList::operator=(const ArrayList&)" << std::endl;

    if (this != &a)
    {
        std::string* newItems = new std::string[a.cap];
        arrayCopy(newItems, a.items, a.sz);

        sz = a.sz;
        cap = a.cap;

        delete[] items;
        items = newItems;
    }

    return *this;
}


// Notice that the bodies of these two functions appear identical.  What makes
// them different, though, is that they return different types: the first
// returns a reference to items[index] that will allow the value in that
// cell to be modified; the second returns a const reference to items[index],
// so it can be read but not modified.

std::string& ArrayList::at(unsigned int index)
{
    return items[index];
}


const std::string& ArrayList::at(unsigned int index) const
{
    return items[index];
}


// add() is straightforward, except when the array is full (i.e., sz == cap),
// in which case we need to allocate a new, larger array to take the place
// of the existing one.  Order is really important in that case; we don't
// want to delete the old array until *after* we've allocated the new one
// *and* copied all of the elements from the old one into the new one.

void ArrayList::add(const std::string& s)
{
    if (sz == cap)
    {
        // The amount of the increase is important here.  If we did it
        // additively (e.g., always allocating 10 more elements than we
        // had before), the average cost of each add() call in a long
        // sequence of add() calls would be O(n).  If we do it multiplicatively
        // -- in this case, we're multiplying by 2, but we could even multiply
        // by something smaller like 3/2 -- the average cost of those add()
        // calls drops to O(1).  The basic principle there is that the
        // multiplicative expansions cost more each time, but they buy you
        // more subsequent cheap operations each time, so the overall cost
        // of the expensive ones spreads evenly over the cheap ones you got
        // as a result.
        //
        // If you're curious about how the analysis works, look up the
        // term "amortized analysis."  That's something we'll see in more
        // detail in ICS 46.
        int newCap = cap * 2 + 1;

        std::string* newItems = new std::string[newCap];
        arrayCopy(newItems, items, sz);

        cap = newCap;

        delete[] items;
        items = newItems;
    }

    items[sz] = s;
    sz++;
}


// size() and capacity() are the least interesting functions, but we still
// need to implement them!

unsigned int ArrayList::size() const
{
    return sz;
}


unsigned int ArrayList::capacity() const
{
    return cap;
}

