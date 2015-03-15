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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/TextureAtlas.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace fg
{
	namespace priv
	{
		class TextureAtlasImpl::Node
		{
		public:
			Node *left;
			Node *right;

			fm::rect2f rct;

			Node() : left(fm::nullPtr),
					 right(fm::nullPtr),
					 rct(fm::vec2(),fm::vec2(-1,-1))
			{

			}

			~Node()
			{
				delete left;
				delete right;
			}

			fm::vec2 getRealSize(const fm::vec2s &imgSize)
			{
				return fm::vec2( (rct.size.w!=-1) ? rct.size.w : imgSize.w-rct.pos.x,
								 (rct.size.h!=-1) ? rct.size.h : imgSize.h-rct.pos.y );
			}

			fm::vec2 correct(fm::vec2 s)
			{
				if (s.x<0) s.x=-1;
				if (s.y<0) s.y=-1;

				return s;
			}

			Node *insert(fm::vec2s rctSize,const fm::vec2s &imgSize)
			{
				Node *best = fm::nullPtr;
				findBest(rctSize,imgSize,best);
				if (best)
					best->split(rctSize,imgSize);

				return best;
			}

			void findBest(fm::vec2s rctSize,const fm::vec2s &imgSize,Node *&best)
			{
				if (left || right)
				{
					left->findBest(rctSize,imgSize,best);
					right->findBest(rctSize,imgSize,best);

					return;
				}

				fm::vec2 freeSpace = getRealSize(imgSize);

				if (rctSize.w > freeSpace.w || rctSize.h > freeSpace.h)
					return;

				fm::vec2 bestPos = best ? best->rct.pos : fm::vec2();

				fm::vec2 bestFreeSpace = best ? best->getRealSize(imgSize) : fm::vec2();

				if (best && freeSpace.area()/rctSize.area()<1.1 && freeSpace.area()<bestFreeSpace.area())
				{
					best = this;
					return;
				}

				if (!best || bestPos.LENGTH()>rct.pos.LENGTH())
					best = this;
			}

			void split(fm::vec2 rctSize,const fm::vec2s &imgSize)
			{
				fm::vec2 freeSpace = getRealSize(imgSize);

				fm::Size w = freeSpace.w - rctSize.w;
				fm::Size h = freeSpace.h - rctSize.h;
				left = new Node;
				right = new Node;

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
		TextureAtlasImpl::TextureAtlasImpl() : m_root(new Node)
		{
			
		}
		
		/////////////////////////////////////////////////////////////
		TextureAtlasImpl::~TextureAtlasImpl()
		{
			delete m_root;
		}
		
		/////////////////////////////////////////////////////////////
		void TextureAtlasImpl::resizeTex(const fm::vec2s &newSize)
		{
			if (m_tex.getSize().area())
			{
				fg::Image img = m_tex.copyToImage();
				
				m_tex.create(newSize);
				
				m_tex.update(img,fm::vec2s());
			}
			else
				m_tex.create(newSize);
			
		}
		
		/////////////////////////////////////////////////////////////
		Glyph TextureAtlasImpl::upload(const fg::Image &img,const fm::vec2s &midpt)
		{
			Node *inserted = fm::nullPtr;
			fm::vec2s size = img.getSize();
			
			while (true)
			{
				fm::vec2s texSize = m_tex.getSize();
				inserted = m_root->insert(size,texSize);
				if (inserted)
					break;
				
				resizeTex(texSize+size*1.1);
			}
			
			m_tex.update(img,inserted->rct.pos);
			
			return Glyph(&m_tex,inserted->rct.pos,size,midpt);
		}
		
		/////////////////////////////////////////////////////////////
		void TextureAtlasImpl::reset()
		{
			delete m_root;
			m_root = new Node;
			
			m_tex.create(10,10);
		}
		
		/////////////////////////////////////////////////////////////
		Glyph *TextureAtlasImpl::upload(const fg::Image &atlas,void *glyphPoints,fm::Size stride,fm::Size count)
		{
			Glyph *ret = new Glyph[count];
			
			for (fm::Size i=0;i<count;i++)
			{
				fm::rect2s *rectPtr = (fm::rect2s*)((char*)glyphPoints + stride*i);
				fm::vec2s *midPtPtr = ( fm::vec2s*)((char*)glyphPoints + stride*i + sizeof(fm::rect2s));
				
				fg::Image img;
				img.create(atlas,fm::rect2s(rectPtr->pos,rectPtr->size));
				
				Node *inserted = fm::nullPtr;
				fm::vec2s size = img.getSize();
				
				while (true)
				{
					fm::vec2s texSize = m_tex.getSize();
					inserted = m_root->insert(size,texSize);
					if (inserted)
						break;
					
					resizeTex(texSize+size*1.1);
				}
				
				m_tex.update(img,inserted->rct.pos);
				
				ret[i] = Glyph(&m_tex,inserted->rct.pos,size,*midPtPtr);
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