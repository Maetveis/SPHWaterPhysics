#ifndef LOGGER_H
#define LOGGER_H

#include <type_traits>
#include <iostream>
#include <utility>

enum class LogSeverity
{
	Debug = 0,
	Info = 1,
	Warning = 2,
	Error = 3,
	None = 10
};

template<LogSeverity S>
struct LogSeverityString
{
};

template<>
struct LogSeverityString<LogSeverity::Debug>
{
	static constexpr char value[] = "[Debug]";
};

template<>
struct LogSeverityString<LogSeverity::Info>
{
	static constexpr char value[] = "[Info]";
};

template<>
struct LogSeverityString<LogSeverity::Warning>
{
	static constexpr char value[] = "[Warning]";
};

template<>
struct LogSeverityString<LogSeverity::Error>
{
	static constexpr char value[] = "[Error]";
};

template<LogSeverity A>
class LoggerInstance
{
public:
	//Actual logging class
	template
	<
		LogSeverity C,
		bool = (A <= C)
	>
	struct Output
	{
		template<typename T>
		std::ostream& operator<< (T&& data) const
		{
			return std::cerr << LogSeverityString<C>::value << ' ' << std::forward<T>(data);
		}
	};

	//Dummy class
	template
	<
		LogSeverity C
	>
	struct Output<C, false>
	{
		template<typename T>
		constexpr Output operator<< (T&&) const
		{
			return Output();
		}
	};
	
	static constexpr Output<LogSeverity::Debug> Debug{};
	static constexpr Output<LogSeverity::Info> Info{};
	static constexpr Output<LogSeverity::Warning> Warning{};
	static constexpr Output<LogSeverity::Error> Error{};
};

//Main compile-time loglevel set here
using Logger = LoggerInstance<LogSeverity::Debug>;

#endif //LOGGER_H