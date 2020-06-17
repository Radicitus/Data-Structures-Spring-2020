// HashSet.hpp
//
// ICS 46 Spring 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;

    struct Node
    {
        ElementType element;
        Node* next;
    };

    Node **hashArray;
    int hashArrayCapacity;
    int hashArrayElements;
    void addToArray(Node **arrayToAdd, const ElementType& element);
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    hashArray = new Node*[DEFAULT_CAPACITY];
    hashArrayCapacity = DEFAULT_CAPACITY;
    hashArrayElements = 0;

    for (int i = 0; i < hashArrayCapacity; ++i) {
        hashArray[i] = nullptr;
    }
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    for (int i = 0; i < hashArrayCapacity; ++i) {
        Node* current = hashArray[i];
        Node* next;
        while (current != nullptr) {
            next = current->next;
            delete current;
            current = next;
        }
    }

    delete[] hashArray;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    hashFunction = s.hashFunction;

    hashArray = new Node*[s.hashArrayCapacity];
    hashArrayCapacity = s.hashArrayCapacity;
    hashArrayElements = 0;

    for (int i = 0; i < hashArrayCapacity; ++i) {
        hashArray[i] = nullptr;
    }

    for (int i = 0; i < hashArrayCapacity; ++i) {
        if (s.hashArray[i] != nullptr) {
            Node * temp = s.hashArray[i];
            while (temp != nullptr) {
                add(temp->element);
                temp = temp->next;
            }
        }
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    hashFunction = s.hashFunction;
    s.hashFunction = NULL;

    hashArray = s.hashArray;
    s.hashArray = nullptr;

    hashArrayCapacity = s.hashArrayCapacity;
    s.hashArrayCapacity = 0;

    hashArrayElements = s.hashArrayElements;
    s.hashArrayElements = 0;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    //Delete everything in hashArray
    for (int i = 0; i < hashArrayCapacity; i++)
    {
        if (hashArray[i] != nullptr) {
            while (hashArray[i] != nullptr) {
                Node * toDelete = hashArray[i];
                hashArray[i] = hashArray[i]->next;
                delete toDelete;
            }
        }
    }

    //Copy everything over
    hashFunction = s.hashFunction;
    hashArrayCapacity = s.hashArrayCapacity;
    hashArrayElements = 0;

    for (int i = 0; i < hashArrayCapacity; ++i) {
        if (s.hashArray[i] != nullptr) {
            Node * temp = s.hashArray[i];
            while (temp != nullptr) {
                add(temp->element);
                temp = temp->next;
            }
        }
    }
    return * this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    HashFunction tempFunction = hashFunction;
    hashFunction = s.hashFunction;
    s.hashFunction = tempFunction;

    Node ** tempArray = hashArray;
    hashArray = s.hashArray;
    s.hashArray = tempArray;

    int tempArrayCapacity = hashArrayCapacity;
    hashArrayCapacity = s.hashArrayCapacity;
    s.hashArrayCapacity = tempArrayCapacity;

    int tempArrayElements = hashArrayElements;
    hashArrayElements = s.hashArrayElements;
    s.hashArrayElements = tempArrayElements;

    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if (!contains(element)) {
        if (static_cast<double>(hashArrayElements)/hashArrayCapacity > .8) {
            int newCapacity = hashArrayCapacity * 2 + 1;
            Node ** newHashArray = new Node*[newCapacity];
            for (int i = 0; i < newCapacity; ++i) {
                newHashArray[i] = nullptr;
            }
            //Rehash the old hashArray
            hashArrayElements = 0;
            for (int i = 0; i < hashArrayCapacity; ++i) {
                if (hashArray[i] != nullptr) {
                    Node * temp = hashArray[i];
                    Node * tempNext;
                    while (temp != nullptr) {
                        tempNext = temp->next;
                        addToArray(newHashArray, temp->element);
                        delete temp;
                        temp = tempNext;
                    }
                }
            }
            //Add new element
            addToArray(newHashArray, element);
            hashArrayCapacity = newCapacity;
            //Remove old hashArray
            delete[] hashArray;
            //Point to new hashArray
            hashArray = newHashArray;
        } else {
            addToArray(hashArray, element);
        }
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int hashValue = hashFunction(element);

    int hashIndex = hashValue % hashArrayCapacity;

    Node * temp = hashArray[hashIndex];

    if (temp != nullptr) {
        while (temp != nullptr) {
            if (temp->element == element) {return true;}
            temp = temp->next;
        }
        return false;
    } else {
        return false;
    }
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return hashArrayElements;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    unsigned int count = 0;

    Node * temp = hashArray[index];
    while (temp != nullptr) {
        temp = temp->next;
        count++;
    }

    return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    Node * temp = hashArray[index];
    while (temp != nullptr) {
        if (temp->element == element) {return true;}
        temp = temp->next;
    }
    return false;
}

template<typename ElementType>
void HashSet<ElementType>::addToArray(HashSet::Node **arrayToAdd, const ElementType &element) {
    Node * tempNode = new Node;
    tempNode->element = element;
    tempNode->next = nullptr;

    unsigned int hashValue = hashFunction(element);

    int hashIndex = hashValue % hashArrayCapacity;

    if (elementsAtIndex(hashIndex) > 0) {
        Node * temp = hashArray[hashIndex];
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = tempNode;
        hashArrayElements++;
    } else {
        arrayToAdd[hashIndex] = tempNode;
        hashArrayElements++;
    }
}


#endif // HASHSET_HPP

