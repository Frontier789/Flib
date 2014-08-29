#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/Config.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>

namespace fg
{
	fm::Log glCheck_log = std::cout;
	namespace priv
	{
		/// glCheckError /////////////////////////////////////////////////////////
		void glCheckError(const char *file, unsigned int line)
		{
			unsigned int errorCode = glGetError();

			if (errorCode)
			{
				std::string fileName = file;
							fileName = fileName.substr(fileName.find_last_of("\\/")+1);
				std::string error = "unknown error";
				std::string details  = "no details";

					if (errorCode == GL_INVALID_ENUM)
						details = "an unacceptable value has been specified for an enumerated argument",
						error = "GL_INVALID_ENUM";

					if (errorCode == GL_INVALID_VALUE)
						details = "a numeric argument is out of range",
						error = "GL_INVALID_VALUE";

					if (errorCode == GL_INVALID_OPERATION)
						details = "the specified operation is not allowed in the current state",
						error = "GL_INVALID_OPERATION";

					if (errorCode == GL_STACK_OVERFLOW)
						details = "this command would cause a stack overflow",
						error = "GL_STACK_OVERFLOW";

					if (errorCode == GL_STACK_UNDERFLOW)
						details = "this command would cause a stack underflow",
						error = "GL_STACK_UNDERFLOW";

					if (errorCode == GL_OUT_OF_MEMORY)
						details = "there is not enough memory left to execute the command",
						error = "GL_OUT_OF_MEMORY";
					
					if (errorCode == GL_INVALID_FRAMEBUFFER_OPERATION)
						details = "the object bound to GL_FRAMEBUFFER is not \"framebuffer complete\"",
						error = "GL_INVALID_FRAMEBUFFER_OPERATION";
				
				glCheck_log << "An internal OpenGL call failed in "
							<< fileName << " (" << line << ") : "
							<< error << ", " << details
							<< std::endl;
			}
		}
	}
}
