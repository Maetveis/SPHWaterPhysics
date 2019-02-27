#include "SlotContainerDec.hpp"

template<typename T>
std::size_t SlotContainer<T>::Push(const T& data)
{
	if(freeHead > 0)
	{
		std::size_t next = storage[freeHead].free.next;
		storage[freeHead].data = data;
		freeHead = next;
	}
	else
	{
		storage.push_back(data);
		return storage.size();
	}
}

template<typename T>
std::size_t SlotContainer<T>::Push(T&& data)
{
	if(freeHead > 0)
	{
		std::size_t next = storage[freeHead].free.next;
		storage[freeHead].data = data;
		freeHead = next;
	}
	else
	{
		storage.push_back(data);
		return storage.size();
	}
}

template<typename T>
void SlotContainer<T>::Erase(std::size_t index)
{
	storage[index].free.next = freeHead;
	freeHead = index;
}

template<typename T>
T& SlotContainer<T>::operator[](std::size_t index)
{
	return storage[index].data;
}

template<typename T>
const T& SlotContainer<T>::operator[](std::size_t index) const
{
	return storage[index].data;
}
