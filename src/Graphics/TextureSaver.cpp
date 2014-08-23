#include "TextureSaver.hpp"
#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/Config.hpp>
#include <FRONTIER/OpenGL.hpp>
namespace fg
{
	namespace priv
	{
		/// constructor /////////////////////////////////////////////////////////
		TextureSaver::TextureSaver(bool save)
		{
			if (save)
			{
				GLint textureId;
				glCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureId));
				m_textureId = textureId;
			}
			else
				m_textureId = -1;
		}

		/// destructor /////////////////////////////////////////////////////////
		TextureSaver::~TextureSaver()
		{
			if (m_textureId != -1)
			{
				GLint textureId = m_textureId;
				glCheck(glBindTexture(GL_TEXTURE_2D, textureId));
			}
		}
	}
}
