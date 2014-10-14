////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_FONT_HPP_INCLUDED
#define FRONTIER_FONT_HPP_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/Glyph.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/Config.hpp>
#define FRONTIER_FONT
#include <string>
#include <vector>
#include <map>
namespace fg
{
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		///
		/// 	@brief Simple class used to convert characters to codepoints
		///
		/////////////////////////////////////////////////////////////
		class FRONTIER_API CodePoint
		{
		public:
			fm::Uint32 cp; ///< The value of the codepoint
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			///
			/////////////////////////////////////////////////////////////
			CodePoint();
			
			/////////////////////////////////////////////////////////////
			/// @brief Construct codepoint from character
			/// 
			/// @param c single-byte character
			/// 
			/////////////////////////////////////////////////////////////
			CodePoint(char c);
			
			/////////////////////////////////////////////////////////////
			/// @brief Construct codepoint from wide character
			/// 
			/// @param c multi-byte character
			/// 
			/////////////////////////////////////////////////////////////
			CodePoint(wchar_t c);
			
			/////////////////////////////////////////////////////////////
			/// @brief Convert codepoint to integer value
			/// 
			/// @return The integer value
			/// 
			/////////////////////////////////////////////////////////////
			operator fm::Uint32();
		};
		
		/////////////////////////////////////////////////////////////
		///
		/// 	@brief Class used to store glyph mapping on the font atlas
		///
		/////////////////////////////////////////////////////////////
		class FRONTIER_API GlyphMap
		{
		public:
			/////////////////////////////////////////////////////////////
			///
			/// 	@brief Class used store data about a character that uniquely identifies it
			///
			/////////////////////////////////////////////////////////////
			class FRONTIER_API Identifier
			{
			public:
				CodePoint codePoint; ///< The codepoint of the character
				unsigned int style;  ///< The style of the character (e.g. Underlined)
			
				/////////////////////////////////////////////////////////////
				/// @brief Default constructor
				/// 
				/////////////////////////////////////////////////////////////
				Identifier();
				
				/////////////////////////////////////////////////////////////
				/// @brief Construct the identifier from codepoint and style
				/// 
				/// @param codePoint The codepoint
				/// @param style The style
				/// 
				/////////////////////////////////////////////////////////////
				Identifier(CodePoint codePoint,unsigned int style);
				
				/////////////////////////////////////////////////////////////
				/// @brief Overload of binary operator <
				/// 
				/// This function is used by std::map when sorting
				/// 
				/// @param other Right operand
				/// 
				/////////////////////////////////////////////////////////////
				bool operator<(const Identifier &other) const;
			};

			/////////////////////////////////////////////////////////////
			///
			/// 	@brief Class used to store data about a glyph-row in the font atlas
			///
			/////////////////////////////////////////////////////////////
			class FRONTIER_API Row
			{
			public:
				std::size_t start;  ///< The vertical offset of the row
				std::size_t height; ///< The height of the row
				std::size_t width;  ///< Indicates how long is the row filled
				
				/////////////////////////////////////////////////////////////
				/// @brief Default constructor
				/// 
				/////////////////////////////////////////////////////////////
				Row();
				
				/////////////////////////////////////////////////////////////
				/// @brief Construct the row data from its data
				/// 
				/// @param start The vertical offset of the row
				/// @param height The height of the row
				/// @param width Indicates how long is the row filled
				/// 
				/////////////////////////////////////////////////////////////
				Row(std::size_t start,std::size_t height,std::size_t width=0);
			};
			std::vector<Row> rows;                 ///< Holds the data of the rows in the texture atlas
			std::map<Identifier,Glyph> glyphTable; ///< Maps Identifiers to Glyphs
			fg::Texture atlas;                     ///< The texture atlas
		};
	}
	
	/////////////////////////////////////////////////////////////
	/// @brief Namespace used to hold values that indicate the style of a text
	/// 
	/////////////////////////////////////////////////////////////
	namespace Style
	{
		enum {
			Regular     = 0x0000000, ///< Indicates that the text is regular
			Underlined  = 0x0000001, ///< Indicates that the text is underlined
			Crossed     = 0x0000010, ///< Indicates that the text is striked through
			Outline     = 0x0000100, ///< Indicates that the text is an outline
			Bold        = 0x0001000, ///< Indicates that the text is emboldened
			Italic      = 0x0010000, ///< Indicates that the text is itlicized
			Superscript = 0x0100000, ///< Indicates that the text is saller and is stituated above th baseline
			Subscript   = 0x1000000  ///< Indicates that the text is saller and is stituated below the baseline
        };
	}
	
	//////////////////////////////////
	/// 
	/// 	@brief Font class that implements <a href="http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02#Creating_a_texture_atlas">Font atlas</a> technique
	/// 
	/// @ingroup Graphics
	/// 
	//////////////////////////////////
	class FRONTIER_API Font
	{
	public:
		class Renderer;
		
		//////////////////////////////////
		/// @brief This POD class holds some typical data about a font
		/// 
		//////////////////////////////////
		class FRONTIER_API Metrics
		{
		public:
			int maxH;    ///< How high is the highest character
			int minH;    ///< How low can a character go (e.g. the letter 'g')
			int lineGap; ///< The width of a space
		};
	private:
		unsigned int *m_refCount; ///< How many instances does reference this object
		Renderer 	 *m_renderer; ///< A pointer to a fg::Font::Renderer that does the rendering part
		mutable std::map<unsigned int,priv::GlyphMap> *m_glyphMaps; ///< Maps characer sizes to different font atlases
	public:
		typedef Font &reference;
		typedef const Font &const_reference;
			
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// After this the object is invalid (uninitialized)
		/// 
		/////////////////////////////////////////////////////////////
		Font();
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// If @a copy is invalid (uninitialized) then this the object will be invalid (uninitialized)
		/// 
		/////////////////////////////////////////////////////////////
		Font(const_reference copy);
		
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		~Font();
		
		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a ttf file
		/// 
		/// If the file cannot be loaded or does not contain a valid
		/// ttf font then an error is prompted to fg_log and object becomes invalid
		/// 
		/// @param fileName The name of the ttf file
		/// @param size The initial size
		/// 
		/// @return True if everything went right false otherwise
		/// 
		/////////////////////////////////////////////////////////////
		bool loadFromFile(const std::string &fileName,unsigned int size=64);
		
		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a ttf file that loaded in memory
		/// 
		/// If @a fileContent contains an invalid
		/// ttf font then an error is prompted to fg_log and object becomes invalid
		/// 
		/// @param fileContent A pointer to the beggining of the file in memory
		/// @param fileSizeInBytes The number of bytes in the loaded file
		/// @param size The initial size
		/// 
		/// @return True if everything went right false otherwise
		/// 
		/////////////////////////////////////////////////////////////
        bool loadFromMemory(const void *fileContent,unsigned int fileSizeInBytes,unsigned int size=64);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the mignification and magnification filter on the texture atlases
		/// 
		/// @param smooth If true then linear filtering is used, nearest filtering otherwise
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        reference setSmooth(bool smooth);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a glyph from its codepoint and style with the current size
		/// 
		/// This function searches the glyphTable and if doesn't
		/// find the glyph it creates the glyph.
		/// 
		/// @param letter The codepoint of the glyph
		/// @param type The style of the glyph
		/// 
		/// @return The glyph
		/// 
		/////////////////////////////////////////////////////////////
        Glyph getGlyph(priv::CodePoint letter,unsigned int type = Style::Regular) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Render a glyph's image with the current size
		/// 
		/// This function does not access the glyphTable
		/// 
		/// @param letter The codepoint of the glyph
		/// @param type The style of the glyph
		/// @param leftDown If not NULL it will be set to the offset of the glyph from the baseline
		/// 
		/// @return The rendered image
		/// 
		/////////////////////////////////////////////////////////////
        Image renderGlyph(priv::CodePoint letter,unsigned int type = Style::Regular,fm::vector2<float> *leftDown=NULL) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a glyph with given size
		/// 
		/// This function does not access the glyphTable
		/// 
		/// @param codePoint The codepoint of the glyph
		/// @param characterSize The size of the created character in pixels
		/// @param type The style of the glyph
		/// 
		/// @return The glyph
		/// 
		/////////////////////////////////////////////////////////////
        Glyph createGlyph(priv::CodePoint codePoint, unsigned int characterSize,unsigned int type = Style::Regular) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get information about the font metrics 
		/// 
		/// @return The metrics of the font
		/// 
		/////////////////////////////////////////////////////////////
        Metrics getMetrics() const;

		/////////////////////////////////////////////////////////////
		/// @brief Change the currently used size
		/// 
		/// @param size The new size
		/// 
		/////////////////////////////////////////////////////////////
        void setSize(unsigned int size) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the currently used font atlas
		/// 
		/// @return The font atlas
		/// 
		/////////////////////////////////////////////////////////////
        const Texture &getTexture() const;
    private:
    	
		/////////////////////////////////////////////////////////////
		/// @brief Iternal function used to clean up the resources
		/// 
		/////////////////////////////////////////////////////////////
        void cleanUp();
        
		/////////////////////////////////////////////////////////////
		/// @brief Iternal function used to initialize the resources
		/// 
		/////////////////////////////////////////////////////////////
		void init();
	};
	
	
	//////////////////////////////////
	/// 
	/// 	@brief The heart of the Font class, this renders the glyph images
	/// 
	//////////////////////////////////
	class FRONTIER_API Font::Renderer : fm::NonCopyable
	{
		bool  m_loaded;                           ///< Iternal data  
		void *m_fontInfo;                         ///< Iternal data  
		mutable float m_scale;                    ///< Iternal data  
		mutable unsigned int m_currentSize;       ///< Iternal data  
        mutable int m_maxH;                       ///< Iternal data 
        mutable int m_minH;                       ///< Iternal data 
        mutable int m_lineGap;                    ///< Iternal data 
		std::vector<unsigned char> m_fileContent; ///< Iternal data  
	public:
		typedef Renderer &reference;
		typedef const Renderer &const_reference;
        
			
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// After this the object is invalid (uninitialized)
		/// 
		/////////////////////////////////////////////////////////////
		Renderer();
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		~Renderer();
		
		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a ttf file
		/// 
		/// If the file cannot be loaded or does not contain a valid
		/// ttf font then an error is prompted to fg_log and object becomes invalid
		/// 
		/// @param fileName The name of the ttf file
		/// @param size The initial size
		/// 
		/// @return True if everything went right false otherwise
		/// 
		/////////////////////////////////////////////////////////////
		bool loadFromFile(const std::string &fileName,unsigned int size=64);
		
		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a ttf file that loaded in memory
		/// 
		/// If @a fileContent contains an invalid
		/// ttf font then an error is prompted to fg_log and object becomes invalid
		/// 
		/// @param fileContent A pointer to the beggining of the file in memory
		/// @param fileSizeInBytes The number of bytes in the loaded file
		/// @param size The initial size
		/// 
		/// @return True if everything went right false otherwise
		/// 
		/////////////////////////////////////////////////////////////
        bool loadFromMemory(const void *fileContent,unsigned int fileSizeInBytes,unsigned int size=64);
		
		/////////////////////////////////////////////////////////////
		/// @brief Render a glyph's image with the current size
		/// 
		/// This function does not access the glyphTable
		/// The rendered image is pale white and the alpha channel indicates whether is
		/// a pixel inside of the character
		/// 
		/// @param letter The codepoint of the glyph
		/// @param type The style of the glyph
		/// @param leftDown If not NULL it will be set to the offset of the glyph from the baseline
		/// 
		/// @return The rendered image
		/// 
		/////////////////////////////////////////////////////////////
        Image renderGlyph(priv::CodePoint letter,unsigned int type = Style::Regular,fm::vector2<float> *leftDown=NULL) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get information about the font metrics 
		/// 
		/// @return The metrics of the font
		/// 
		/////////////////////////////////////////////////////////////
        Font::Metrics getMetrics() const;

		/////////////////////////////////////////////////////////////
		/// @brief Change the currently used size
		/// 
		/// @param size The new size
		/// 
		/////////////////////////////////////////////////////////////
        void setSize(unsigned int size) const;
        
        friend class Font;
	};
	////////////////////////////////////////////////////////////
	/// @class fg::Font
	/// @ingroup Graphics
	/// 
	/// @class fg::Font::Renderer
	/// @ingroup Graphics
	///
	/// This class is used to load ttf files and render images of character with it
	/// It works without OpenGL
	/// 
	/// Example:
	/// @code
	/// fg::Font::Renderer arial;
	/// if (arial.loadFromFile("arial.ttf"))
	/// {
	/// 	fg::Image A = arial.renderGlyph('A');
	/// 	A.saveToFile("A.png");
	/// }
	/// else
	/// 	return ERROR;
	/// @endcode
	/// 
	/// 
	/// @see fg::Font
	/// @see fg::Image
	///
	////////////////////////////////////////////////////////////
}

#endif // FRONTIER_FONT_HPP_INCLUDED
