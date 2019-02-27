#ifndef INDEXED_LINKED_LIST_HPP
#define INDEXED_LINKED_LIST_HPP

#include <vector>

#include "SlotContainer.hpp"

template<typename T>
class IndexedLinkedList
{
private:
	class Node
	{
		T data;
		std::size_t prev;
		std::size_t next;
	};

	std::vector<Node> storage;

	class iterator
	{
	private:
		IndexedLinkedList* parent;
		std::size_t index;

		iterator(IndexedLinkedList* _parent, std::size_t _index) :
			parent(_parent),
			index(_index)
		{
		}
	public:
		iterator(const iterator&) = default;
		iterator& operator=(const iterator&) = default;

		T* operator->() const;
		T& operator*() const;
		iterator& operator++(int);
		iterator operator++();
		iterator& operator--(int);
		iterator operator--();
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;
	};

	class const_iterator
	{
	private:
		IndexedLinkedList const* parent;
		std::size_t index;

		const_iterator(IndexedLinkedList const* _parent, std::size_t _index) :
			parent(_parent),
			index(_index)
		{
		}
	public:
		const_iterator(const const_iterator&) = default;
		const_iterator& operator=(const const_iterator&) = default;

		T const * operator->() const;
		const T& operator*() const;
		const_iterator& operator++(int);
		const_iterator operator++();
		const_iterator& operator--(int);
		const_iterator operator--();
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;
	};

	SlotContainer<T> container;

	std::size_t front;
	std::size_t back;
public:
	IndexedLinkedList() :
		front(0),
		back(0)
	{
	}

	
};

#endif
