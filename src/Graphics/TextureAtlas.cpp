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
#include <FRONTIER/Graphics/TextureAtlas.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		class TextureAtlasImpl::Node
		{
		public:
			Node *left;
			Node *right;

			fm::rect2f rct;

			/////////////////////////////////////////////////////////////
			Node() : left(nullptr),
					 right(nullptr),
					 rct(fm::vec2(),fm::vec2(-1,-1))
			{

			}

			/////////////////////////////////////////////////////////////
			Node(const Node &copy) : left(nullptr),
									 right(nullptr),
									 rct(copy.rct)
			{
				if (copy.left)
					left = new Node(*(copy.left));
				
				if (copy.right)
					right = new Node(*(copy.right));
			}

			/////////////////////////////////////////////////////////////
			~Node()
			{
				delete left;
				delete right;
			}

			/////////////////////////////////////////////////////////////
			fm::vec2 getRealSize(const fm::vec2s &imgSize)
			{
				return fm::vec2( (rct.size.w!=-1) ? rct.size.w : imgSize.w-rct.pos.x,
								 (rct.size.h!=-1) ? rct.size.h : imgSize.h-rct.pos.y );
			}

			/////////////////////////////////////////////////////////////
			fm::vec2 correct(fm::vec2 s)
			{
				if (s.x<0) s.x=-1;
				if (s.y<0) s.y=-1;

				return s;
			}

			/////////////////////////////////////////////////////////////
			Node *insert(fm::vec2s rctSize,const fm::vec2s &imgSize,bool keepDistance = true)
			{
				if (keepDistance)
					rctSize += fm::vec2s(1,1);
				
				Node *best = nullptr;
				findBest(rctSize,imgSize,best);
				if (best)
					best->split(rctSize,imgSize);

				return best;
			}

			/////////////////////////////////////////////////////////////
			void findBest(const fm::vec2s &rctSize,const fm::vec2s &imgSize,Node *&best)
			{
				// if not a leaf, continue recursion
				if (left || right)
				{
					left->findBest(rctSize,imgSize,best);
					right->findBest(rctSize,imgSize,best);

					return;
				}
				
				// calculate empty size
				fm::vec2 freeSpace = getRealSize(imgSize);
				
				// test if there is enough empty space
				if (rctSize.w > freeSpace.w || rctSize.h > freeSpace.h)
					return;
				
				// test if this place is better than the current best
				fm::vec2 bestPos = best ? best->rct.pos : fm::vec2();

				fm::vec2 bestFreeSpace = best ? best->getRealSize(imgSize) : fm::vec2();
				
				// if the empty space is nearly as big as the rectangle choose it
				if (best && freeSpace.area()/rctSize.area()<1.1 && freeSpace.area()<bestFreeSpace.area())
				{
					best = this;
					return;
				}
				
				// select the closest one to the origin
				if (!best || bestPos.LENGTH()>rct.pos.LENGTH())
					best = this;
			}

			void split(const fm::vec2 &rctSize,const fm::vec2s &imgSize)
			{
				fm::vec2 freeSpace = getRealSize(imgSize);

				fm::Size w = freeSpace.w - rctSize.w;
				fm::Size h = freeSpace.h - rctSize.h;
				
				// insert new leaves
				left = new Node;
				right = new Node;
				
				// slice the area in an optimal way
				if (w < h)
				{
					right->rct.pos = rct.pos + fm::vec2(rctSize.w,0);
					right->rct.size = fm::vec2(correct(rct.size-rctSize).w,rctSize.h);

					left->rct.pos = rct.pos + fm::vec2(0,rctSize.h);
					left->rct.size = fm::vec2(rct.size.w,correct(rct.size-rctSize).h);
				}
				else
				{
					left->rct.pos = rct.pos + fm::vec2(0,rctSize.h);
					left->rct.size = fm::vec2(rctSize.w,correct(rct.size-rctSize).h);

					right->rct.pos = rct.pos + fm::vec2(rctSize.w,0);
					right->rct.size = fm::vec2(correct(rct.size-rctSize).w,rct.size.h);
				}

				rct.size = rctSize;
			}
		};
		
		/////////////////////////////////////////////////////////////
		TextureAtlasImpl::TextureAtlasImpl() : m_shaderNeedsUpdate(true),
											   m_fboNeedsUpdate(true),
											   m_root(new Node)
		{
			
		}

		/////////////////////////////////////////////////////////////
		TextureAtlasImpl::TextureAtlasImpl(const TextureAtlasImpl &copy) : m_root(new Node(*(copy.m_root)))
		{
			if (copy.m_tex.getSize().area())
				m_tex.loadFromImage(copy.m_tex.copyToImage());
		}
		
		/////////////////////////////////////////////////////////////
		TextureAtlasImpl::~TextureAtlasImpl()
		{
			delete m_root;
		}
		
		/////////////////////////////////////////////////////////////
		void TextureAtlasImpl::resizeTex(const fm::vec2s &newSize)
		{
			// if a texture is present recreate it
			if (m_tex.getSize().area())
			{
				fg::Image img = m_tex.copyToImage();
				
				m_tex.create(newSize);
				m_tex.update(img,fm::vec2s());
			}
			else
				m_tex.create(newSize);
			
			m_fboNeedsUpdate = true;
		}

		/////////////////////////////////////////////////////////////
		TextureAtlasImpl::Node *TextureAtlasImpl::recursiveInsert(const fm::vec2s &rectSize)
		{
			Node *inserted = nullptr;
			
			// resize texture until the image fits
			while (!inserted)
			{
				fm::vec2s texSize = m_tex.getSize();
				inserted = m_root->insert(rectSize,texSize);
				if (!inserted)
				{
					resizeTex(texSize+rectSize*1.1);
				}
			}
			
			return inserted;
		}
		
		/////////////////////////////////////////////////////////////
		Glyph TextureAtlasImpl::upload(const fg::Image &img,const fm::vec2 &leftdown)
		{
			Node *inserted = recursiveInsert(img.getSize());
			m_tex.update(img,inserted->rct.pos);
			
			return Glyph(&m_tex,inserted->rct.pos,img.getSize(),leftdown);
		}
		
		/////////////////////////////////////////////////////////////
		Glyph TextureAtlasImpl::upload(const fg::Texture &tex,const fm::vec2 &leftdown)
		{
			Node *inserted = recursiveInsert(tex.getSize());
				
			if (m_fboNeedsUpdate)
			{
				m_fboNeedsUpdate = false;
				m_updaterFbo.create(m_tex);
			}
			
			if (m_shaderNeedsUpdate)
			{
				m_shaderNeedsUpdate = false;
				m_updaterShader.loadFromMemory(R"(
#version 130

uniform vec2 u_relPos;
uniform vec2 u_relSize;

out vec2 va_p;

void main()
{
	int i = gl_VertexID;
	va_p = vec2(float(i==1 || i==2 || i==4),float(i==2 || i==4 || i==5));
	
	vec2 objP = u_relPos + u_relSize * va_p;
	
	gl_Position = vec4((objP * 2 - vec2(1,1)),0,1);
})",R"(
#version 130

uniform sampler2D u_tex;

in vec2 va_p;

out vec4 out_color;

void main()
{
	out_color = texture2D(u_tex,va_p);
})");
			}
			
			fm::rect2s viewport = fg::FrameBuffer::getViewport();
			
			m_updaterFbo.bind();
			m_updaterShader.bind();
			m_updaterShader.setUniform("u_tex",tex);
			m_updaterShader.setUniform("u_relPos",fm::vec2(inserted->rct.pos) / m_tex.getSize());
			m_updaterShader.setUniform("u_relSize",fm::vec2(tex.getSize()) / m_tex.getSize());
			glDrawArrays(GL_TRIANGLES,0,6);
			
			FrameBuffer::bind(nullptr);
			FrameBuffer::setViewport(viewport);
			
			return Glyph(&m_tex,inserted->rct.pos,tex.getSize(),leftdown);
		}
		
		/////////////////////////////////////////////////////////////
		void TextureAtlasImpl::reset()
		{
			delete m_root;
			m_root = new Node;
			
			m_tex.create(fm::vec2(10,10));
		}
		
		/////////////////////////////////////////////////////////////
		Glyph *TextureAtlasImpl::upload(const fg::Image &atlas,void *glyphPoints,fm::Size stride,fm::Size count)
		{
			Glyph *ret = new Glyph[count];
			
			// for each glyph
			for (fm::Size i=0;i<count;i++)
			{
				// there is a rect, a vec2 and a MappedType in glyphPoints with give stride
				fm::rect2s *rectPtr     = (fm::rect2s*)((char*)glyphPoints + stride*i);
				fm::vec2   *leftdownPtr =   (fm::vec2*)((char*)glyphPoints + stride*i + sizeof(fm::rect2s));
				
				// copy the important part of the image
				fg::Image img;
				img.create(atlas,rectPtr->pos,rectPtr->size);
				
				Node *inserted = nullptr;
				fm::vec2s size = img.getSize();
				
				// resize texture until the image fits
				while (true)
				{
					fm::vec2s texSize = m_tex.getSize();
					inserted = m_root->insert(size,texSize);
					if (inserted)
						break;
					
					resizeTex(texSize+size*1.1);
				}
				
				// upload image
				m_tex.update(img,inserted->rct.pos);
				
				// store created glyph
				ret[i] = Glyph(&m_tex,inserted->rct.pos,size,*leftdownPtr);
			}
			
			return ret;
		}
	
		/////////////////////////////////////////////////////////////
		Texture &TextureAtlasImpl::getTexture()
		{
			return m_tex;
		}
		
		/////////////////////////////////////////////////////////////
		const Texture &TextureAtlasImpl::getTexture() const
		{
			return m_tex;
		}
	}
}
