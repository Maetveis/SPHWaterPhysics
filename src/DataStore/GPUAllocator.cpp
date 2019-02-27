#include "GPUAllocator.hpp"

GPUAllocator::GPUAllocator(GLuint _max) :
	max(_max)
{
	holesByOffset.emplace(0, max);
	holesByLength.emplace(0, max);
}

GPUAllocator::~GPUAllocator()
{
}

bool GPUAllocator::Allocate(GLuint size, GLuint alignment, GLuint* value)
{
	GLuint alignOffset;
	auto iter = holesByLength.lower_bound(size);
	while( iter != holesByLength.end())
	{
		alignOffset = (alignment - iter->offset % alignment) % alignment;
		if(alignOffset + size <= iter->size)
			break;
		++iter;
	}
	if(iter == holesByLength.end())
		return false;

	if(alignOffset > 0)
		AddHole(iter->offset, alignOffset);
	if(alignOffset + size < iter->size)
		AddHole(iter->offset + alignOffset + size, iter->size - alignOffset - size);

	holesByOffset.erase(Hole(iter->offset, iter->size));
	holesByLength.erase(iter);

	*value = iter->offset + alignOffset;

	return true;
};

void GPUAllocator::AddHole(GLuint offset, GLuint size)
{
	holesByLength.emplace(offset, size);
	holesByOffset.emplace(offset, size);
}

void GPUAllocator::DeAllocate(GLuint offset, GLuint size)
{
	GLuint newOffset = offset, newSize = size;

	auto before = holesByOffset.lower_bound(Hole(offset, size));
	--before;
	if(before != holesByOffset.end() && before->offset + before->size >= offset)
	{
		newOffset = before->offset;
		newSize += before->size;

		holesByLength.erase(Hole(before->offset, before->size));
		holesByOffset.erase(before);
	}

	auto after = holesByOffset.upper_bound(Hole(offset, size));
	if(after != holesByOffset.end() && offset + size >= after->offset)
	{
		newSize += after->size;

		holesByLength.erase(Hole(after->offset, after->size));
		holesByOffset.erase(after);
	}

	AddHole(newOffset, newSize);
}
