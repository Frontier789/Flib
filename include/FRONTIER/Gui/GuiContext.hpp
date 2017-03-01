////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Result.hpp>
#include <map>

#define FRONTIER_GUICONTEXT

namespace fg
{
	class Font;
	class Shader;
	class Drawable;
	class DrawData;
	class ShaderManager;
}

namespace fm
{
	class Result;
}

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Holds data for gui elements such as fonts
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class GuiContext
	{
	protected:
		std::map<fm::String,fg::Font*> m_fonts; ///< The loaded fonts
		fg::ShaderManager *m_shader; ///< The shader used when drawing
		fm::vec2s m_size; ///< The size of the context
		
		void setupShader(); ///< Initialises the shader
		
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
		/// @brief Move constructor
		///
		/// @param cont The context to move
		///
		/////////////////////////////////////////////////////////////
		GuiContext(GuiContext &&cont);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructr
		///
		/////////////////////////////////////////////////////////////
		virtual ~GuiContext();
		
		/////////////////////////////////////////////////////////////
		/// @brief Load a font based on its name
		/// 
		/// @param fontName The name of the font
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result loadFont(const fm::String &fontName);
		
		/////////////////////////////////////////////////////////////
		/// @brief Load a font from memory
		/// 
		/// @param fontName The name of the font
		/// @param font The font to load 
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		virtual fm::Result loadFont(const fm::String &fontName,fg::Font *newFont);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a font based on its name
		/// 
		/// @param fontName The name of the font
		/// 
		/// @return The font (null if not found)
		/// 
		/////////////////////////////////////////////////////////////
		fg::Font *getFont(const fm::String &fontName);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the default font
		/// 
		/// @return The font (null if not found)
		/// 
		/////////////////////////////////////////////////////////////
		fg::Font *getDefaultFont();
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the default font
		/// 
		/// @param font The font to set as default
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result setDefaultFont(fg::Font *font);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the used shader
		/// 
		/// @param newShader The new shader
		/// @param delPrevShader Indeicates whether the previous shader is to be deleted
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
		/// @brief Draw an indexset of a drawdata
		/// 
		/// @param data The drawdata to use
		/// @param indexSet The id of the drawcall in @a data to use
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result draw(const fg::DrawData &data,fm::Size indexSet = 0);
		
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
		/// @brief Swap two gui contexts
		///
		/// @param cont The context to swap with
		/// 
		/// @param Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		GuiContext &swap(GuiContext &cont);

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment operator
		///
		/// @param cont The context to move
		/// 
		/// @param Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		GuiContext &operator=(GuiContext &&cont);
	};
}

#endif // FRONTIER_GUICONTEXT_HPP_INCLUDED
