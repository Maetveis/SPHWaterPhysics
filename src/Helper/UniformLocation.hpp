#ifndef UNIFORM_LOCATION_HPP
#define UNIFORM_LOCATION_HPP

#include <GL/glew.h>

namespace GL {
	class Program;
}

namespace GL {

template<typename T>
class UniformLocation
{
private:
	const Program& program;
	GLuint location;
public:
	UniformLocation(const Program& _program, GLuint _location) :
		program(_program),
		location(_location)
	{
	}


};

}

#endif
