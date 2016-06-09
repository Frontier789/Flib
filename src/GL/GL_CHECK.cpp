////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <deque>

#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
    #include <FRONTIER/System/TlsPtr.hpp>
    #include <FRONTIER/System/Mutex.hpp>
#endif // FRONTIER_PROTECT_SHARED_VARIABLES

namespace fg
{
    namespace priv
    {
        #ifdef FRONTIER_PROTECT_SHARED_VARIABLES

        std::deque<std::deque<fm::Error> > glErrors;
        fm::TlsPtr<std::deque<fm::Error> > tlsGLErrors;
        fm::Mutex glErrorMutex;

        #else

        std::deque<fm::Error> glErrors;

        #endif // FRONTIER_PROTECT_SHARED_VARIABLES

        fm::Delegate<void,const fm::Error &> callback;
    }
}

namespace fg
{
	namespace GL
	{
	    using namespace fg::priv;

		//////////////////////////////////
		fm::Error getLastGLError()
		{
		    #ifdef FRONTIER_PROTECT_SHARED_VARIABLES

		    if (!tlsGLErrors)
                return fm::Result();

            if (tlsGLErrors->size())
            {
                fm::Error ret = tlsGLErrors->back();
                tlsGLErrors->pop_back();
                return ret;
            }

		    #else

		    if (glErrors.size())
            {
                fm::Error ret = glErrors.back();
                glErrors.pop_back();
                return ret;
            }

		    #endif // FRONTIER_PROTECT_SHARED_VARIABLES

            return fm::Result();
		}

		//////////////////////////////////
		bool hasGLError()
		{
		    #ifdef FRONTIER_PROTECT_SHARED_VARIABLES

		    if (!tlsGLErrors)
                return false;

            if (tlsGLErrors->size())
                return true;

		    #else

		    if (glErrors.size())
                return true;

		    #endif // FRONTIER_PROTECT_SHARED_VARIABLES

            return false;
		}

		//////////////////////////////////
		void raiseGLError(const fm::Error &error)
		{
		    #ifdef FRONTIER_PROTECT_SHARED_VARIABLES

		    if (!tlsGLErrors)
            {
                glErrorMutex.lock();
                glErrors.push_back(std::deque<fm::Error>());
                tlsGLErrors = &glErrors.back();
                glErrorMutex.unLock();
            }

            tlsGLErrors->push_back(error);

		    #else

		    glErrors.push_back(error);

		    #endif // FRONTIER_PROTECT_SHARED_VARIABLES

		    fg::priv::callback(error);
		}

		//////////////////////////////////
		void setGLErrorListener(const fm::Delegate<void,const fm::Error &> &callback)
		{
		    fg::priv::callback = callback;
		}

		/// glCheckError /////////////////////////////////////////////////////////
		fm::Result glCheckError(const char *file, unsigned int line,const char *call)
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

                fm::Result ret = fm::Result("GLError",error,details,call,file,line);

                raiseGLError(ret);

				return ret;
			}

			return fm::Result();
		}
	}
}
