#ifndef FRONTIER_IMAGE_HPP_INCLUDED
#define FRONTIER_IMAGE_HPP_INCLUDED

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/Graphics/Color.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Rect.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_IMAGE
#include <string>
#include <vector>
namespace fg
{
	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Class used to hold the pixels of an image
	/// 
	/////////////////////////////////////////////////////////////
	class Image
	{
		std::vector<Color> m_pixels; ///< Hold all the pixels of the image
		fm::vec2s m_size; ///< Holds the size of the image
	public:
		typedef Image &reference;
		typedef const Image &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// The initialized image has a size of (0,0) and doesn't have data
		///
		/////////////////////////////////////////////////////////////
		Image();

		/////////////////////////////////////////////////////////////
		/// @brief Construct an image of given size and fill it with given color
		///
		/// @param width The width of the new image
		/// @param height The height of the new image
		/// @param color The color to fill with
		///
		/////////////////////////////////////////////////////////////
		Image(std::size_t width,std::size_t height,const Color &color = Color::White);

		/////////////////////////////////////////////////////////////
		/// @brief Construct an image of given size and fill it with given color
		///
		/// @param size The size of the new image
		/// @param color The color to fill with
		///
		/////////////////////////////////////////////////////////////
		explicit Image(const fm::vec2s &size,const Color &color = Color::White);

		/////////////////////////////////////////////////////////////
		/// @brief Construct an image as a subimage of an existing one
		///
		/// If @a sourceRect has a size of (0,0) it is understood as to copy the
		/// part of the image that is higher and righter than the position of the rect
		///
		/// @param copy The source image
		/// @param sourceRect The rect to be copied
		///
		/////////////////////////////////////////////////////////////
		Image(const Image &copy,const fm::rect2u sourceRect=fm::rect2u(0,0,0,0));

		/////////////////////////////////////////////////////////////
		/// @brief Create the image with given size and fill with given color
		///
		/// Please note that all changes on this image
		/// before this call will be discarded
		///
		/// @param width The new width
		/// @param height The new height
		/// @param color The color to fill With
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference create(std::size_t width,std::size_t height,const Color &color = Color::White);

		/////////////////////////////////////////////////////////////
		/// @brief Create the image with given size and fill with given color
		///
		/// Please note that all changes on this image
		/// before this call will be discarded
		///
		/// @param size The new size
		/// @param color The color to fill With
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference create(const fm::vec2s &size,const Color &color = Color::White);

		/////////////////////////////////////////////////////////////
		/// @brief Create the image with given size and given data
		///
		/// Please note that all changes on this image
		/// before this call will be discarded
		///
		/// @param width The new width
		/// @param height The new height
		/// @param pixels The data to copy to the image
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference create(std::size_t width,std::size_t height,const Color *pixels);

		/////////////////////////////////////////////////////////////
		/// @brief Create the image with given size and given data
		///
		/// Please note that all changes on this image
		/// before this call will be discarded
		///
		/// @param size The new size
		/// @param pixels The data to copy to the image
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference create(const fm::vec2s &size,const Color *pixels);

		/////////////////////////////////////////////////////////////
		/// @brief Recreate an image as a subimage of an existing one
		///
		/// @param copy The source image
		/// @param sourceRect The rect to be copied
		///
		/////////////////////////////////////////////////////////////
		reference create(const Image &copy,const fm::rect2u sourceRect=fm::rect2u(0,0,0,0));

		/////////////////////////////////////////////////////////////
		/// @brief Copy an image to this image
		/// 
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		/// 
		/// @param source The source image
		/// @param destPos The destination position 
		/// @param sourceRect The rectangle to be copied
		/// @param useAlpha If true then alpha-blending will be used
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference copy(const Image &source,const fm::vec2s &destPos,const fm::rect2u &sourceRect = fm::rect2u(0,0,0,0),bool useAlpha=false);

		/////////////////////////////////////////////////////////////
		/// @brief Copy an image to this image
		/// 
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		/// 
		/// @param source The source image
		/// @param destX The x dimension of the destination position 
		/// @param destY The y dimension of the destination position 
		/// @param sourceRect The rectangle to be copied
		/// @param useAlpha If true then alpha-blending will be used
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference copy(const Image &source,std::size_t destX,std::size_t destY,const fm::rect2u &sourceRect = fm::rect2u(0,0,0,0),bool useAlpha=false);

		/////////////////////////////////////////////////////////////
		/// @brief Copy from this image
		/// 
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		/// 
		/// @param destination The target image
		/// @param destPos The destination position 
		/// @param sourceRect The rectangle to be copied
		/// @param useAlpha If true then alpha-blending will be used
		///
		/////////////////////////////////////////////////////////////
		void copyTo(Image &destination,const fm::vec2s &destPos,const fm::rect2u &sourceRect = fm::rect2u(0,0,0,0),bool useAlpha=false) const;

		/////////////////////////////////////////////////////////////
		/// @brief Copy from this image
		/// 
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		/// 
		/// @param destination The target image
		/// @param destX The x dimension of the destination position 
		/// @param destY The y dimension of the destination position 
		/// @param sourceRect The rectangle to be copied
		/// @param useAlpha If true then alpha-blending will be used
		///
		/////////////////////////////////////////////////////////////
		void copyTo(Image &destination,std::size_t destX,std::size_t destY,const fm::rect2u &sourceRect = fm::rect2u(0,0,0,0),bool useAlpha=false) const;

		/////////////////////////////////////////////////////////////
		/// @brief Copy an image to this image
		/// 
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		/// 
		/// @param source The source image
		/// @param destPos The destination position 
		/// @param sourceRect The rectangle to be copied
		/// @param useAlpha If true then alpha-blending will be used
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference copyFrom(const Image &source,const fm::vec2s &destPos,const fm::rect2u &sourceRect = fm::rect2u(0,0,0,0),bool useAlpha=false);

		/////////////////////////////////////////////////////////////
		/// @brief Copy an image to this image
		/// 
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		/// 
		/// @param source The source image
		/// @param destX The x dimension of the destination position 
		/// @param destY The y dimension of the destination position 
		/// @param sourceRect The rectangle to be copied
		/// @param useAlpha If true then alpha-blending will be used
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference copyFrom(const Image &source,std::size_t destX,std::size_t destY,const fm::rect2u &sourceRect = fm::rect2u(0,0,0,0),bool useAlpha=false);

		/////////////////////////////////////////////////////////////
		/// @brief Load image from a file 
		///  
		/// Supported types are jpg, jpeg, png, gif, tga 
		/// it uses stb_image http://nothings.org
		/// 
		/// If anything goes wrong this function returns false and 
		/// a description of the rror is printed to fg_log
		///
		/// @param filename The name of the file to load
		/// 
		/// @return True if everything went ok
		/// 
		/////////////////////////////////////////////////////////////
		bool loadFromFile(const std::string &filename);

		/////////////////////////////////////////////////////////////
		/// @brief Save image to a file 
		///  
		/// Supported types are jpg, jpeg, png, gif, tga 
		/// it uses stb_image http://nothings.org
		/// 
		/// If anything goes wrong this function returns false and 
		/// a description of the rror is printed to fg_log
		///
		/// @param filename The name of the file to save to
		/// 
		/// @return True if everything went ok
		/// 
		/////////////////////////////////////////////////////////////
		bool saveToFile(const std::string &filename);

		/////////////////////////////////////////////////////////////
		/// @brief Get size of the image
		/// 
		/// @return The size of the image
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Change a pixel of the image
		/// 
		/// No bound check is performed on x and y
		/// because the data of the image is linear in memory if x is begger than
		/// the width of the image the changed pixel will be at (x%width,y+int(x/width))
		/// 
		/// @param x The x coordinate of the pixel
		/// @param y The y coordinate of the pixel
		/// @param color the new color of the pixel
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setPixel(std::size_t x,std::size_t y,const Color &color);

		/////////////////////////////////////////////////////////////
		/// @brief Change a pixel of the image
		/// 
		/// No bound check is performed on x and y
		/// because the data of the image is linear in memory if x is begger than
		/// the width of the image the changed pixel will be at (x%width,y+int(x/width))
		/// 
		/// @param pos The position of the pixel 
		/// @param color the new color of the pixel
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setPixel(const fm::vec2s &pos,const Color &color);

		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a pixel
		/// 
		/// @param x The x coordinate of the pixel
		/// @param y The y coordinate of the pixel
		/// 
		/// @return The color of the pixel
		/// 
		/////////////////////////////////////////////////////////////
		const Color &getPixel(std::size_t x,std::size_t y) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a pixel
		/// 
		/// @param x The x coordinate of the pixel
		/// @param y The y coordinate of the pixel
		/// 
		/// @return The color of the pixel
		/// 
		/////////////////////////////////////////////////////////////
		Color &getPixel(std::size_t x,std::size_t y);

		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a pixel
		/// 
		/// @param pos The position of the pixel
		/// 
		/// @return The color of the pixel
		/// 
		/////////////////////////////////////////////////////////////
		const Color &getPixel(const fm::vec2s &pos) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a pixel
		/// 
		/// @param pos The position of the pixel
		/// 
		/// @return The color of the pixel
		/// 
		/////////////////////////////////////////////////////////////
		Color &getPixel(const fm::vec2s &pos);

		/////////////////////////////////////////////////////////////
		/// @brief Access the pixels in c-style
		/// 
		/// @return Pointer to the pixels
		/// 
		/////////////////////////////////////////////////////////////
		Color *getPixelsPtr();

		/////////////////////////////////////////////////////////////
		/// @brief Access the pixels in c-style
		/// 
		/// @return Pointer to the pixels
		/// 
		/////////////////////////////////////////////////////////////
		const Color *getPixelsPtr() const;

		/////////////////////////////////////////////////////////////
		/// @brief Flip the image horizontally
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference flipHorizontally();

		/////////////////////////////////////////////////////////////
		/// @brief Flip the image vertically
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference flipVertically();
	};
}
#endif

////////////////////////////////////////////////////////////
/// @class fg::Image
/// @ingroup Graphics
///
/// fg::Image is used to load, process and save images
/// although it doesn't have any advanced photo processing function
/// 
/// Usage example:
/// @code
/// 
/// fg::Image img;
/// img.create(500,500);
/// for (std::size_t x=0;x<img.getSize().w;x++)
/// 	for (std::size_t y=0;y<img.getSize().h;y++)
/// 		img.setPixel(x,y,(y%50<25 ? (x%50<25) : ((49-(x%50))<25)) ? fg::Color(80,80,80) : fg::Color(200,200,200));
/// img.saveToFile("chessBoard.png");
///
/// @endcode
/// 
/// @see fg::Color
///
////////////////////////////////////////////////////////////