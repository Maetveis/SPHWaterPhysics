#include "Logger.h"

namespace Logging
{
	void Settings::SetLevel(Level _level)
	{
		level = _level;
	}

	Level Settings::GetLevel()
	{
		return level;
	}

	Level Settings::level = Level::Debug;
}
