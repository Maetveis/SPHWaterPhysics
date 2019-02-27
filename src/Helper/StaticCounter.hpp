#ifndef STATIC_COUNTER_HPP
#define STATIC_COUNTER_HPP

#include <queue>
#include <vector>

template<typename T, typename Label>
class StaticCounter
{
private:
	static T contStart; //The first available slot
	static std::priority_queue< T, std::vector<T>, std::greater<T> > holes; //The holes before the continous part
	T slot;
public:
StaticCounter() :
	slot(Request())
{
}

~StaticCounter()
{
	Relase(slot);
}

T Get() const
{
	return slot;
}

operator T() const
{
	return Get();
}

static T Request()
{
	if(holes.empty())
	{
		return contStart++;
	} else {
		T ret = holes.top();
		holes.pop();
		return ret;
	}
}

static void Relase(T slot)
{
	holes.push(slot);
	while(!holes.empty() && contStart - holes.top() <= 1)
	{
		holes.pop();
		--contStart;
	}

	if(holes.empty())
	{
		contStart = 0;
	}
}

};

template<typename T, typename Label>
T StaticCounter<T, Label>::contStart = 0;

template<typename T, typename Label>
std::priority_queue< T, std::vector<T>, std::greater<T> > StaticCounter<T, Label>::holes;

#endif //STATIC_COUNTER_HPP
