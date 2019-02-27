#ifndef SLOT_CONTAINER_HPP
#define SLOT_CONTAINER_HPP

#include <vector>

template<typename T>
class SlotContainer
{
private:
	class alignas(T) EmptyNode
	{
		std::size_t next;
	};

	using Node = union {
		T data;
		EmptyNode free;
	};

	std::vector<Node> storage;

	std::size_t freeHead;
public:
	std::size_t Push(const T& data);
	std::size_t Push(T&& data);
	void Erase(std::size_t index);

	T& operator[](std::size_t index);
	const T& operator[](std::size_t index) const;

	std::size_t Size()
	{
	};

	SlotContainer() :
		storage(1)
	{
		storage.push_back(EmptyNode(0));
	}


	SlotContainer(const SlotContainer&) = default;
	SlotContainer(SlotContainer&&) = default;

	SlotContainer& operator=(const SlotContainer&) = default;
	SlotContainer& operator=(SlotContainer&&) = default;

	~SlotContainer() = default;
};

#endif
