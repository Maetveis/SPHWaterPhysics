#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

namespace Logging
{

enum class Level: short
{
	Debug = 0,
	Info = 1,
	Warning = 2,
	Error = 3,
	None = 10
};

template<Level L>
struct LevelString
{
};

template<>
struct LevelString<Level::Debug>
{
	static constexpr char value[] = "[Debug]";
};

template<>
struct LevelString<Level::Info>
{
	static constexpr char value[] = "[Info]";
};

template<>
struct LevelString<Level::Warning>
{
	static constexpr char value[] = "[Warning]";
};

template<>
struct LevelString<Level::Error>
{
	static constexpr char value[] = "[Error]";
};

class Settings
{
private:
	static Level level;
public:
	static void SetLevel(Level _level);

	static Level GetLevel();
};

template<Level L>
class Instance
{
public:
	Instance()
	{
		if(L >= Settings::GetLevel())
		{
			std::cerr << LevelString<L>::value << ' ';
		}
	}

	template<typename T>
	Instance& operator<<(T&& t)
	{
		if(L >= Settings::GetLevel())
		{
			std::cerr << t;
		}
		return *this;
	}

	template<typename T>
	Instance& operator<<(const T&& t)
	{
		if(L >= Settings::GetLevel())
		{
			std::cerr << t;
		}
		return *this;
	}
};

struct Logger
{
	using Debug = Logging::Instance<Logging::Level::Debug>;
	using Info = Logging::Instance<Logging::Level::Info>;
	using Warning = Logging::Instance<Logging::Level::Warning>;
	using Error = Logging::Instance<Logging::Level::Error>;
};

}// namespace Logger

using Logger = Logging::Logger;

#endif
