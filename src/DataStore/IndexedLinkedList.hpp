#include "IndexedLinkedDec.hpp"

// --- iterator ---

template<typename T>
T* IndexedLinkedList<T>::iterator::operator->() const
{
	return &parent->storage[index];
}

template<typename T>
T& IndexedLinkedList<T>::iterator::operator*() const
{
	return &parent->storage[index];
}

template<typename T>
typename IndexedLinkedList<T>::iterator& IndexedLinkedList<T>::iterator::operator++(int)
{
	index = *parent[index].next;
	return *this;
}

template<typename T>
typename IndexedLinkedList<T>::iterator IndexedLinkedList<T>::iterator::operator++()
{
	iterator copy = *this;
	operator++(0);
	return copy;
}

template<typename T>
typename IndexedLinkedList<T>::iterator& IndexedLinkedList<T>::iterator::operator--(int)
{
	index = *parent[index].prev;
	return *this;
}

template<typename T>
typename IndexedLinkedList<T>::iterator IndexedLinkedList<T>::iterator::operator--()
{
	iterator copy = *this;
	operator--(0);
	return copy;
}

template<typename T>
bool IndexedLinkedList<T>::iterator::operator==(const iterator& other) const
{
	return other.parent == parent && other.index == index;
}

template<typename T>
bool IndexedLinkedList<T>::iterator::operator!=(const iterator& other) const
{
	return other.parent != parent || other.index != index;
}

// --- const_iterator ---

template<typename T>
T const * IndexedLinkedList<T>::const_iterator::operator->() const
{
	return &parent->storage[index];
}

template<typename T>
const T& IndexedLinkedList<T>::const_iterator::operator*() const
{
	return &parent->storage[index];
}

template<typename T>
typename IndexedLinkedList<T>::const_iterator& IndexedLinkedList<T>::const_iterator::operator++(int)
{
	index = *parent[index].next;
	return *this;
}

template<typename T>
typename IndexedLinkedList<T>::const_iterator IndexedLinkedList<T>::const_iterator::operator++()
{
	iterator copy = *this;
	operator++(0);
	return copy;
}

template<typename T>
typename IndexedLinkedList<T>::const_iterator& IndexedLinkedList<T>::const_iterator::operator--(int)
{
	index = *parent[index].prev;
	return *this;
}

template<typename T>
typename IndexedLinkedList<T>::const_iterator IndexedLinkedList<T>::const_iterator::operator--()
{
	iterator copy = *this;
	operator--(0);
	return copy;
}

template<typename T>
bool IndexedLinkedList<T>::const_iterator::operator==(const iterator& other) const
{
	return other.parent == parent && other.index == index;
}

template<typename T>
bool IndexedLinkedList<T>::const_iterator::operator!=(const iterator& other) const
{
	return other.parent != parent || other.index != index;
}
