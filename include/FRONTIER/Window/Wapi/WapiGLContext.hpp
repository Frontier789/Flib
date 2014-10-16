#ifndef FRONTIER_WAPI_GLCONTEXT_INCLUDED
#define FRONTIER_WAPI_GLCONTEXT_INCLUDED
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Window/GLContext.hpp>
#define FRONTIER_WAPI_GLCONTEXT
#include <windows.h>
#include <string>

namespace fw
{
	/////////////////////////////////////////////////////////////
	/// @brief Get last windows api error string
	/// 
	/// @return The last error's string
	/////////////////////////////////////////////////////////////
	std::string WapiGetLastError();
	
	namespace Wapi
	{
		/////////////////////////////////////////////////////////////
		/// @brief Windows api OpenGL context
		/// 
		/// Upon error every function of this class prints to fw::fw_log
		/// 
		/////////////////////////////////////////////////////////////
		class FRONTIER_API GlContext
		{
			HDC   m_hdc;    ///< The handle of the device context
			HWND  m_hwnd;   ///< The handle of the window
			HGLRC m_hglrc;  ///< The handle of the OpenGL rednering context
			bool cleanUp(); ///< Iternal function that cleans up resources
			fw::GlContext::Settings m_settings; ///< The attributes of the context
			bool setPixelFormat(); ///< Iternal function
			bool m_ownWindow;      ///< Indicates whether the context holds a dummy window
			
			static unsigned int m_contextCount; ///< Holds the number of contexts with dummy windows
			
		public:
			typedef GlContext &reference;
			typedef const GlContext &const_reference;
			
			typedef HGLRC Handle; ///< The handle type is defined for all GLContext implementations
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			/// 
			/////////////////////////////////////////////////////////////
			GlContext();
			
			/////////////////////////////////////////////////////////////
			/// @brief Default destructor
			/// 
			/////////////////////////////////////////////////////////////
			~GlContext();
			
			/////////////////////////////////////////////////////////////
			/// @brief Makes the context active in the current thread
			/// 
			/// If the context wasn't successfully initialized
			/// then this function deactivates the current context
			/// 
			/// @param active True to make the context active False to deactive the currently active one
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool setActive(bool active=true);
			
			/////////////////////////////////////////////////////////////
			/// @brief Swap the back and front buffer
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool swapBuffers();
			
			/////////////////////////////////////////////////////////////
			/// @brief Initializes the context
			/// 
			/// @param windowHandle The handle of the associated window (shouldn't be NULL)
			/// @param sharedContext The context to share the OpenGL objects with 
			/// @param settings The attributes of the context (only a hint!)
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool create(HWND windowHandle,HGLRC sharedContext=NULL,fw::GlContext::Settings settings = fw::GlContext::Settings());
			
			/////////////////////////////////////////////////////////////
			/// @brief Initializes the context with a dummy (hidden) window
			/// 
			/// @param sharedContext The context to share the OpenGL objects with 
			/// @param settings The attributes of the context (only a hint!)
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool create(HGLRC sharedContext=NULL,fw::GlContext::Settings settings = fw::GlContext::Settings());
			
			/////////////////////////////////////////////////////////////
			/// @brief Get the handle of the context
			/// 
			/// Returns NULL if not initialized
			/// 
			/// @return The handle
			/// 
			/////////////////////////////////////////////////////////////
			Handle getHandle() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Implicitly convert to handle
			/// 
			/// Returns NULL if not initialized
			/// 
			/// @return The handle
			/// 
			/////////////////////////////////////////////////////////////
			operator Handle() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Read the attributes of the context
			/// 
			/// @return The settings
			/// 
			/////////////////////////////////////////////////////////////
			const fw::GlContext::Settings &getSettings() const;
		};
	}
}


#endif // FRONTIER_WAPI_GLCONTEXT_INCLUDED