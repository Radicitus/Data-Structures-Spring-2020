#include "ArrayList.hpp"

//Asymptotic Notation: Theta(1); As we are simply moving the values, it is extremely cheap and the cost doesn't change
// regardless of the size of "items". sz and cap are irrelevant as they are constants, and thus cheap regardless.
ArrayList::ArrayList(ArrayList &&a) noexcept
    : items{nullptr}, sz{0}, cap{0}
{
    std::swap(items, a.items);
    std::swap(sz, a.sz);
    std::swap(cap, a.cap);
}
//Asymptotic Notation: Theta(1); As we are simply moving the values, it is extremely cheap and the cost doesn't change
// regardless of the size of "items". sz and cap are irrelevant as they are constants, and thus cheap regardless.
ArrayList &ArrayList::operator=(ArrayList &&a) noexcept {
    std::swap(items, a.items);
    std::swap(sz, a.sz);
    std::swap(cap, a.cap);
    return * this;
}