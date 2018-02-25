////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_GUICONTEXT_HPP_INCLUDED
#define FRONTIER_GUICONTEXT_HPP_INCLUDED

#include <FRONTIER/Graphics/SpriteManager.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/Sprite.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Clock.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/System/Time.hpp>
#include <map>

#define FRONTIER_GUICONTEXT

namespace fg
{
	class Shader;
	class Drawable;
	class DrawData;
	class ShaderManager;
}

namespace fw
{
	class Event;
}

namespace fm
{
	class Result;
}

namespace fgui
{
	class GuiLayout;
	
	/////////////////////////////////////////////////////////////
	/// @brief Holds data for gui elements such as fonts
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GuiContext
	{
	protected:
		mutable std::map<fm::String,fg::Font> m_fonts; ///< The loaded fonts
		fg::SpriteManager  m_spriteManager;    ///< The texture atlas for sprites
		fg::ShaderManager *m_shader; ///< The shader used when drawing
		fm::Size   m_defCharSize;    ///< Default character size
		GuiLayout *m_layout; ///< The main layout of the context
		fm::Clock  m_fpsClk; ///< Clock for measuring fps
		fm::vec2s  m_size;   ///< The size of the context
		fm::Time   m_spf;    ///< Second per frame (0 on inf fps)
		bool m_guiLoop;      ///< Indicates whether the gui loop is running 
		
		void setupShader(); ///< Initialises the shader

		virtual void drawElementsGuiLoop(); ///< Draw the elements inside gui loop
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		GuiContext();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct with given size
		/// 
		/// @param size The size of the context
		/// 
		/////////////////////////////////////////////////////////////
		GuiContext(fm::vec2s size);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructr
		///
		/////////////////////////////////////////////////////////////
		virtual ~GuiContext();
		
		/////////////////////////////////////////////////////////////
		/// @brief Store a sprite in the context
		/// 
		/// @param name The name of the sprite
		/// @param spriteImage The image of the sprite
		/// @param frameSize The frame size
		/// 
		/////////////////////////////////////////////////////////////
		void addSprite(const std::string &name,const fg::Image &spriteImage,const fm::vec2s &frameSize = fm::vec2s());
		
		/////////////////////////////////////////////////////////////
		/// @brief Retrieve a named sprite from the context
		/// 
		/// @param name The name of the sprite
		/// 
		/// @return The sprite
		/// 
		/////////////////////////////////////////////////////////////
		fg::Sprite getSprite(const std::string &name);
		
		/////////////////////////////////////////////////////////////
		/// @brief Access the sprite manager of the context
		/// 
		/// @return The sprite manager
		/// 
		/////////////////////////////////////////////////////////////
		fg::SpriteManager &getSpriteManager();
		
		/////////////////////////////////////////////////////////////
		/// @brief Access the sprite manager of the context
		/// 
		/// @return The sprite manager
		/// 
		/////////////////////////////////////////////////////////////
		const fg::SpriteManager &getSpriteManager() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Load a font based on its name
		/// 
		/// @param fontName The name of the font
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result loadFont(const fm::String &fontName) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Load a font from memory
		/// 
		/// @param fontName The name of the font
		/// @param font The font to load 
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		virtual fm::Result loadFont(const fm::String &fontName,fg::Font font) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a font based on its name
		/// 
		/// @param fontName The name of the font
		/// 
		/// @return The font
		/// 
		/////////////////////////////////////////////////////////////
		fg::Font getFont(const fm::String &fontName) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the default font
		/// 
		/// @return The font (null if not found)
		/// 
		/////////////////////////////////////////////////////////////
		fg::Font getDefaultFont();
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the default font
		/// 
		/// @param font The font to set as default
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result setDefaultFont(fg::Font font);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the used shader
		/// 
		/// @param newShader The new shader
		/// @param delPrevShader Indicates whether the previous shader is to be deleted
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		GuiContext &setShader(fg::ShaderManager *newShader,bool delPrevShader = true);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the used shader
		/// 
		/// @return The used shader
		/// 
		/////////////////////////////////////////////////////////////
		fg::ShaderManager &getShader();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the used shader
		/// 
		/// @return The used shader
		/// 
		/////////////////////////////////////////////////////////////
		const fg::ShaderManager &getShader() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw a drawdata
		/// 
		/// @param data The drawdata to use
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result draw(const fg::DrawData &data);
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw an indexset of a drawdata
		/// 
		/// @param data The drawdata to use
		/// @param indexSet The id of the drawcall in @a data to use
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result draw(const fg::DrawData &data,fm::Size indexSet);
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw a drawable object
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		GuiContext &draw(fg::Drawable &drawable);
		
		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the size of the context
		///
		/// @return The size of the context
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2s getSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the context
		///
		/// @param size The size of the context
		///
		/////////////////////////////////////////////////////////////
		virtual void setSize(fm::vec2s size);

		/////////////////////////////////////////////////////////////
		/// @brief Set the main layout of the context
		///
		/// @param layout The new layout
		/// @param delPrevLayout Indicates whether the previous layout is to be deleted
		/// 
		/////////////////////////////////////////////////////////////
		void setMainLayout(GuiLayout *layout,bool delPrevLayout = true);

		/////////////////////////////////////////////////////////////
		/// @brief Set the main layout of the context
		///
		/// @return The layout
		/// 
		/////////////////////////////////////////////////////////////
		GuiLayout &getMainLayout();
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle an event
		///
		/// Forwards event ot the main layout
		/// 
		/// @param ev The event
		/// 
		/////////////////////////////////////////////////////////////
		virtual void handleEvent(fw::Event &ev);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle all pending events
		/// 
		/// This function is to be implemented by derived classes
		/// 
		/////////////////////////////////////////////////////////////
		virtual void handlePendingEvents();
		
		/////////////////////////////////////////////////////////////
		/// @brief Call the update function of the elements in the main layout
		/// 
		/////////////////////////////////////////////////////////////
		void updateElements();
		
		/////////////////////////////////////////////////////////////
		/// @brief Call the draw function of the elements in the main layout
		/// 
		/////////////////////////////////////////////////////////////
		virtual void drawElements();
		
		/////////////////////////////////////////////////////////////
		/// @brief Suspend the thread until fps is dropped to set limit
		/// 
		/////////////////////////////////////////////////////////////
		void applyFpsLimit();
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the desired time between updates
		/// 
		/// Same as calling setMaxFps(1s / interval)
		/// 
		/// @param interval The interval between updates
		/// 
		/////////////////////////////////////////////////////////////
		void setUpdateInterval(fm::Time interval);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the maximum fps (zero means inf)
		/// 
		/// Same as calling setUpdateInterval(1s / interval)
		/// 
		/// @param fps The max fps
		/// 
		/////////////////////////////////////////////////////////////
		void setMaxFps(float fps);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the time between updates
		/// 
		/// @return The interval between updates
		/// 
		/////////////////////////////////////////////////////////////
		fm::Time getUpdateInterval() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Run the gui loop of the context
		/// 
		/// @return The exit code
		/// 
		/////////////////////////////////////////////////////////////
		int runGuiLoop();
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the loop condition in the gui loop to false
		/// 
		/////////////////////////////////////////////////////////////
		void stopGuiLoop();
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the defult character size
		/// 
		/// Initial value is 13
		/// 
		/// @param defSize The new default size 
		/// 
		/////////////////////////////////////////////////////////////
		void setDefCharSize(fm::Size defSize);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the defult character size
		/// 
		/// @return The default size 
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getDefCharSize() const;
	};
}

#endif // FRONTIER_GUICONTEXT_HPP_INCLUDED
