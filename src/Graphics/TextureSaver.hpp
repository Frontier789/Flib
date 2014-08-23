#ifndef FRONTIER_TEXTURE_SAVER_HPP_INCLUDED
#define FRONTIER_TEXTURE_SAVER_HPP_INCLUDED
#define FRONTIER_TEXTURE_SAVER
namespace fg
{
	namespace priv
	{
		//////////////////////////////////
		/// TextureSaver
		////////////////////////////////// 
		class TextureSaver
		{
			int m_textureId;
		public:
			//////////////////////////////////
			/// constructor
			////////////////////////////////// 
			TextureSaver(bool save = true);
			
			//////////////////////////////////
			/// destructor
			////////////////////////////////// 
			~TextureSaver();
		};
	}
}
#endif