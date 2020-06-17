#ifndef PROBLEM4_HPP
#define PROBLEM4_HPP

#include <initializer_list>



template <typename T>
class DisjointSetForest
{
public:
    DisjointSetForest(std::initializer_list<T> elements);

    template <typename Iterator>
    DisjointSetForest(Iterator begin, Iterator end);

    const T& at(std::size_t index) const;
    std::size_t indexOf(const T& element) const;

    std::size_t weightOf(std::size_t index);
    std::size_t weightOf(const T& element);

    std::size_t findRoot(std::size_t index);
    std::size_t findRoot(const T& element);

    bool unionSets(std::size_t index1, std::size_t index2);
    bool unionSets(const T& element1, const T& element2);

    std::size_t elementCount() const;
    std::size_t setCount() const;

private:
    
    struct ForestNode
    {
        ForestNode * parent;
        T element;
    };

    std::vector<ForestNode> elem;
    std::size_t size;
    std::size_t sets;
    
};



template <typename T>
DisjointSetForest<T>::DisjointSetForest(std::initializer_list<T> elements)
{
    std::initializer_list<T>::iterator it;

    for (it = elements.begin(); it != elements.end(); it++) {
        ForestNode * newNode = new ForestNode;
        newNode->element = *it;
        newNode->parent = nullptr;

        elem.push_back(newNode);
    }
    

}


template <typename T>
template <typename Iterator>
DisjointSetForest<T>::DisjointSetForest(Iterator begin, Iterator end)
{
}


template <typename T>
const T& DisjointSetForest<T>::at(std::size_t index) const
{
    static T removeThis;
    return removeThis;
}


template <typename T>
std::size_t DisjointSetForest<T>::indexOf(const T& element) const
{
    return 0;
}


template <typename T>
std::size_t DisjointSetForest<T>::weightOf(std::size_t index)
{
    return 0;
}


template <typename T>
std::size_t DisjointSetForest<T>::weightOf(const T& element)
{
    return 0;
}


template <typename T>
std::size_t DisjointSetForest<T>::findRoot(std::size_t index)
{
    return 0;
}


template <typename T>
std::size_t DisjointSetForest<T>::findRoot(const T& element)
{
    return 0;
}


template <typename T>
bool DisjointSetForest<T>::unionSets(std::size_t index1, std::size_t index2)
{
    return false;
}


template <typename T>
bool DisjointSetForest<T>::unionSets(const T& element1, const T& element2)
{
    return false;
}


template <typename T>
std::size_t DisjointSetForest<T>::elementCount() const
{
    return 0;
}


template <typename T>
std::size_t DisjointSetForest<T>::setCount() const
{
    return 0;
}



#endif

