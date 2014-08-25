#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/Config.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <ostream>

namespace fg
{
	namespace priv
	{
		/// glCheckError /////////////////////////////////////////////////////////
		void glCheckError(const char* file, unsigned int line)
		{
			unsigned int errorCode = glGetError();

			if (errorCode)
			{
				std::string fileName = file;
							fileName = fileName.substr(fileName.find_last_of("\\/")+1);
				std::string error = "unknown error";
				std::string details  = "no details";

					if (errorCode == 0x500) ///GL_INVALID_ENUM
					{
						error = "GL_INVALID_ENUM";
						details = "an unacceptable value has been specified for an enumerated argument";
					}

					if (errorCode == 0x501)//GL_INVALID_VALUE
					{
						error = "GL_INVALID_VALUE";
						details = "a numeric argument is out of range";
					}

					if (errorCode == 0x502) ///GL_INVALID_OPERATION
					{
						error = "GL_INVALID_OPERATION";
						details = "the specified operation is not allowed in the current state";
					}

					if (errorCode == 0x503) ///GL_STACK_OVERFLOW
					{
						error = "GL_STACK_OVERFLOW";
						details = "this command would cause a stack overflow";
					}

					if (errorCode == 0x504) ///GL_STACK_UNDERFLOW
					{
						error = "GL_STACK_UNDERFLOW";
						details = "this command would cause a stack underflow";
					}

					if (errorCode == 0x505) ///GL_OUT_OF_MEMORY
					{
						error = "GL_OUT_OF_MEMORY";
						details = "there is not enough memory left to execute the command";
					}
					
					if (errorCode == 0x506) ///GL_INVALID_FRAMEBUFFER_OPERATION
					{
						error = "GL_INVALID_FRAMEBUFFER_OPERATION";
						details = "the object bound to GL_FRAMEBUFFER is not \"framebuffer complete\"";
					}
				

				fg_log << "An internal OpenGL call failed in "
						  << fileName << " (" << line << ") : "
						  << error << ", " << details
						  << std::endl;
			}
		}
	}
}
