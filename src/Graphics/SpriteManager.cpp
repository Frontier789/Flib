#include <FRONTIER/Graphics/SpriteManager.hpp>
#include <FRONTIER/Graphics/ShaderSource.hpp>
#include <FRONTIER/Graphics/AssocPoint.hpp>
#include <FRONTIER/Graphics/Attribute.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/GL/GL_SO_LOADER.hpp>

namespace fg
{
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::buildVertices(fm::vec3 **pts,
														 fm::vec2 **uvs,
														 fm::vec4 **clr,
														 fm::vec3 pos,
														 fm::vec2 size,
														 const fg::Glyph &shape,
														 fm::vec2 dir,
														 fm::vec4 color)
		{
			fm::vec2 rightv = -dir.perp();
			
			if (!m_useFrames)
			{
				const fm::vec2 tpt[] = {fm::vec2(0,0),fm::vec2(1,0),fm::vec2(1,1),fm::vec2(0,0),fm::vec2(1,1),fm::vec2(0,1)};
				
				if (pts) *pts = new fm::vec3[6];
				if (uvs) *uvs = new fm::vec2[6];
				if (clr) *clr = new fm::vec4[6];
				
				Cx(6)
				{
					if(pts)
					{
						fm::vec2 locp = tpt[x] * size;
						(*pts)[x] = pos + fm::vec3(locp.x * rightv + locp.y * dir,0);
					}
					if (uvs)
						(*uvs)[x] = tpt[x] * shape.size + shape.pos;
					if (clr)
						(*clr)[x] = color;
				}
			}
			else
			{
				if (pts) *pts = new fm::vec3[54];
				if (uvs) *uvs = new fm::vec2[54];
				if (clr) *clr = new fm::vec4[54];
				
				fm::vec3 *ptsPtr = pts ? *pts : nullptr;
				fm::vec2 *uvsPtr = uvs ? *uvs : nullptr;
				fm::vec4 *clrPtr = clr ? *clr : nullptr;
				
				auto convP = [&](fm::vec2 p) -> fm::vec3 {
					
					fm::vec2 coef0011(p.x > 1.5f,p.y > 1.5f);
					fm::vec2 coef0101 = p - coef0011*2;
					
					fm::vec2 locp = coef0011 * size + (coef0101 - coef0011) * shape.leftdown;
					
					return pos + fm::vec3(locp.x * rightv + locp.y * dir,0);
				};
				
				auto convT = [&](fm::vec2 p) -> fm::vec2 {
					
					fm::vec2 coef0011(p.x > 1.5f,p.y > 1.5f);
					fm::vec2 coef0101 = p - coef0011*2;
					
					return shape.pos + coef0011 * shape.size + (coef0101 - coef0011) * shape.leftdown;
				};
				
				fm::vec2 diffs[] = {fm::vec2(0,0),fm::vec2(1,0),fm::vec2(1,1),fm::vec2(0,0),fm::vec2(1,1),fm::vec2(0,1)};
				
				Cv(3)
				{
					C(6)
					{
						if (pts)
							*ptsPtr++ = convP(p + diffs[i]);
						if (uvs)
							*uvsPtr++ = convT(p + diffs[i]);
						if (clr)
							*clrPtr++ = color;
					}
				}
			}
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::handleCreate(fm::Size spriteIndex,
														const fg::Glyph &shape,
														const fm::vec3 &pos,
														const fm::vec2 &size,
														const fm::vec2 &dir,
														const fm::vec4 &clr)
		{
			handleCreate(spriteIndex,spriteIndex+1,
						 shape,pos,size,dir,clr);
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::handleCreate(fm::Size spriteIndexBeg,
														fm::Size spriteIndexEnd,
														const fg::Glyph &shape,
														const fm::vec3 &pos,
														const fm::vec2 &size,
														const fm::vec2 &dir,
														const fm::vec4 &clr)
		{
			if (!m_shader.isLoaded() && m_shaderResult)
				loadShader();
			
			if (m_drawData.getDrawCount() == 0)
				setupDrawData();
			
			fm::vec2 uvsPos = shape.pos;
			fm::vec2 uvsSiz = shape.size;
			fm::vec2 frameS = shape.leftdown;
			
			fm::Size amount = spriteIndexEnd - spriteIndexBeg;
			
			if (useInstancing())
			{
				m_posProp.push(&pos.x,amount);
				m_dirProp.push(&dir.x,amount);
				m_sizProp.push(&size.x,amount);
				m_uvpProp.push(&uvsPos.x,amount);
				m_uvsProp.push(&uvsSiz.x,amount);
				m_frmProp.push(&frameS.x,amount);
				m_clrProp.push(&clr.x,amount);
			}
			else
			{
				fm::vec3 *pts;
				fm::vec2 *uvs;
				fm::vec4 *clrP;
				
				buildVertices(&pts,&uvs,&clrP,pos,size,shape,dir,clr);
				
				m_vertPtsProp.push(&pts[0].x,amount);
				m_vertUVsProp.push(&uvs[0].x,amount);
				m_vertClrProp.push(&clrP[0].x,amount);
				
				delete[] pts;
				delete[] uvs;
				delete[] clrP;
			}
			
			m_spriteCount += amount;
			
			updateDrawCall();
		}
			
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::handleMoveFrom(fm::Size index,fm::Size oldIndex)
		{
			if (useInstancing())
			{
				m_posProp.updateFrom(index,oldIndex);
				m_dirProp.updateFrom(index,oldIndex);
				m_sizProp.updateFrom(index,oldIndex);
				m_uvpProp.updateFrom(index,oldIndex);
				m_uvsProp.updateFrom(index,oldIndex);
				m_clrProp.updateFrom(index,oldIndex);
				
				if (m_useFrames)
					m_frmProp.updateFrom(index,oldIndex);
			}
			else
			{
				m_vertPtsProp.updateFrom(index,oldIndex);
				m_vertUVsProp.updateFrom(index,oldIndex);
				m_vertClrProp.updateFrom(index,oldIndex);
			}
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::handlePropPop()
		{
			if (useInstancing())
			{
				m_posProp.pop();
				m_dirProp.pop();
				m_sizProp.pop();
				m_uvpProp.pop();
				m_uvsProp.pop();
				m_clrProp.pop();
				
				if (m_useFrames)
					m_frmProp.pop();
			}
			else
			{
				m_vertPtsProp.pop();
				m_vertUVsProp.pop();
				m_vertClrProp.pop();
			}
			
			--m_spriteCount;
			
			updateDrawCall();
		}
		
		/////////////////////////////////////////////////////////////
		bool SpriteManagerBaseNonTemplate::useInstancing() const
		{
			return m_useInstancing;
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::updateDrawCall()
		{
			if (useInstancing())
			{
				m_drawData.getDraw(0).instances = m_spriteCount;
			}
			else
			{
				if (!m_useFrames)
					m_drawData.getDraw(0).set(0,m_spriteCount * 6,fg::Triangles);
				else
					m_drawData.getDraw(0).set(0,m_spriteCount * 54,fg::Triangles);
			}
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::handlePosChange(fm::Size spriteIndex,const fm::vec3 &pos)
		{
			if (useInstancing())
			{
				m_posProp.update(spriteIndex,&pos.x);
			}
			else
			{
				fm::vec3 *pts;
				
				buildVertices(&pts,nullptr,nullptr,
							  pos,fetchSize(spriteIndex),
							  m_glyphGetterFunc(this,spriteIndex),fetchDir(spriteIndex),fetchColor(spriteIndex));
				
				m_vertPtsProp.update(spriteIndex,&pts[0].x);
				
				delete[] pts;
			}
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::handleDirChange(fm::Size spriteIndex,const fm::vec2 &dir)
		{
			if (useInstancing())
			{
				m_dirProp.update(spriteIndex,&dir.x);
			}
			else
			{
				fm::vec3 *pts;
				
				buildVertices(&pts,nullptr,nullptr,
							  fetchPos(spriteIndex),fetchSize(spriteIndex),
							  m_glyphGetterFunc(this,spriteIndex),dir,fetchColor(spriteIndex));
				
				m_vertPtsProp.update(spriteIndex,&pts[0].x);
				
				delete[] pts;
			}
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::handleSizeChange(fm::Size spriteIndex,const fm::vec2 &size)
		{
			fm::vec2 cpySize = size;
			
			if (m_useFrames)
			{
				fg::Glyph shape = m_glyphGetterFunc(this,spriteIndex);
				
				cpySize.w = std::max<float>(cpySize.w,shape.leftdown.w*2);
				cpySize.h = std::max<float>(cpySize.h,shape.leftdown.h*2);
			}
			
			if (useInstancing())
			{
				m_sizProp.update(spriteIndex,&cpySize.x);
			}
			else
			{
				fm::vec3 *pts;
				
				buildVertices(&pts,nullptr,nullptr,
							  fetchPos(spriteIndex),cpySize,
							  m_glyphGetterFunc(this,spriteIndex),fetchDir(spriteIndex),fetchColor(spriteIndex));
				
				m_vertPtsProp.update(spriteIndex,&pts[0].x);
				
				delete[] pts;
			}
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::handleColorChange(fm::Size spriteIndex,const fm::vec4 &clr)
		{
			if (useInstancing())
			{
				m_clrProp.update(spriteIndex,&clr.x);
			}
			else
			{
				fm::vec4 *clrP;
				
				buildVertices(nullptr,nullptr,&clrP,
							  fetchPos(spriteIndex),fetchSize(spriteIndex),
							  m_glyphGetterFunc(this,spriteIndex),fetchDir(spriteIndex),clr);
				
				m_vertClrProp.update(spriteIndex,&clrP[0].x);
				
				delete[] clrP;
			}
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::handleImageIDChange(fm::Size spriteIndex,const fg::Glyph &shape)
		{
			if (useInstancing())
			{
				fm::vec2 uvsPos = shape.pos;
				fm::vec2 uvsSiz = shape.size;
				fm::vec2 frameS = shape.leftdown;
				
				m_uvpProp.update(spriteIndex,&uvsPos.x);
				m_uvsProp.update(spriteIndex,&uvsSiz.x);
				
				if (m_useFrames)
					m_frmProp.update(spriteIndex,&frameS.x);
			}
			else
			{
				fm::vec2 *uvs;
				
				buildVertices(nullptr,&uvs,nullptr,
							  fetchPos(spriteIndex),fetchSize(spriteIndex),
							  shape,fetchDir(spriteIndex),fetchColor(spriteIndex));
				
				m_vertUVsProp.update(spriteIndex,&uvs[0].x);
				
				delete[] uvs;
			}
			
			if (!fetchSize(spriteIndex).area())
				handleSizeChange(spriteIndex,shape.size);
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::getControlPoints(fm::vec3 (&pts)[4],const fm::Size spriteIndex) const
		{
			if (!m_useFrames)
			{
				fm::vec3 ptsl[6];
					
				m_vertPtsProp.fetch(spriteIndex,&ptsl[0].x);
				
				pts[0] = ptsl[0];
				pts[1] = ptsl[1];
				pts[2] = ptsl[2];
				pts[3] = ptsl[5];
			}
			else
			{
				fm::vec3 ptsl[54];
					
				m_vertPtsProp.fetch(spriteIndex,&ptsl[0].x);
				
				pts[0] = ptsl[0];
				pts[1] = ptsl[37];		
				pts[2] = ptsl[50];		
				pts[3] = ptsl[17];		
			}
		}
		
		/////////////////////////////////////////////////////////////
		fm::vec3 SpriteManagerBaseNonTemplate::fetchPos(const fm::Size spriteIndex) const
		{
			if (useInstancing())
			{
				fm::vec3 ret;
				m_posProp.fetch(spriteIndex,&ret.x);
				return ret;
			}
			else
			{
				fm::vec3 pts[4];
				getControlPoints(pts,spriteIndex);
				
				return pts[0];
			}
		}
		
		/////////////////////////////////////////////////////////////
		fm::vec2 SpriteManagerBaseNonTemplate::fetchDir(const fm::Size spriteIndex) const
		{
			if (useInstancing())
			{
				fm::vec2 ret;
				m_dirProp.fetch(spriteIndex,&ret.x);
				
				return ret;
			}
			else
			{
				fm::vec3 pts[4];
				getControlPoints(pts,spriteIndex);
				
				return (pts[2] - pts[1]).sgn();
			}
		}
		
		/////////////////////////////////////////////////////////////
		fm::vec2 SpriteManagerBaseNonTemplate::fetchSize(const fm::Size spriteIndex) const
		{
			if (useInstancing())
			{
				fm::vec2 ret;
				m_sizProp.fetch(spriteIndex,&ret.x);
				return ret;
			}
			else
			{
				fm::vec3 pts[4];
				getControlPoints(pts,spriteIndex);
				
				return fm::vec2((pts[0] - pts[1]).length(),(pts[1] - pts[2]).length());
			}
		}
		
		/////////////////////////////////////////////////////////////
		fm::vec4 SpriteManagerBaseNonTemplate::fetchColor(const fm::Size spriteIndex) const
		{
			if (useInstancing())
			{
				fm::vec4 ret;
				m_clrProp.fetch(spriteIndex,&ret.x);
				return ret;
			}
			else
			{
				fm::vec4 clr1[54];
					
				m_vertClrProp.fetch(spriteIndex,&clr1[0].x);
				
				return clr1[0];
			}
		}
		
		/////////////////////////////////////////////////////////////
		bool SpriteManagerBaseNonTemplate::useFrames() const
		{
			return m_useFrames;
		}

		/////////////////////////////////////////////////////////////
		const fm::Result &SpriteManagerBaseNonTemplate::getShaderLoadResult() const
		{
			return m_shaderResult;
		}
		
		/////////////////////////////////////////////////////////////
		fg::ShaderManager &SpriteManagerBaseNonTemplate::getShader()
		{
			return m_shader;
		}
		
		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::setupDrawData()
		{
			if (useInstancing())
			{
				if (m_useFrames)
				{
					fm::vec2 instapts[16];
					C(16)
						instapts[i] = fm::vec2(i%4,i/4);
						
					m_drawData.positions = instapts;
					
					
					fm::Uint8 instaInd[] = {0,4,1,5,2,6,3,7,11,6,10,5,9,4,8,12,9,13,10,14,11,15};
					
					m_drawData.addDraw().set(instaInd,fg::TriangleStrip);
				}
				else
				{
					const fm::vec2 pts[] = {fm::vec2(0,0),fm::vec2(0,1),fm::vec2(1,1),fm::vec2(1,0)};
					
					m_drawData.addDraw(0,4,fg::TriangleFan);
					m_drawData.positions = pts;
				}
				m_drawData[fg::Assoc::Custom+0].instancesPerUpdate = 1;
				m_drawData[fg::Assoc::Custom+1].instancesPerUpdate = 1;
				m_drawData[fg::Assoc::Custom+2].instancesPerUpdate = 1;
				m_drawData[fg::Assoc::Custom+3].instancesPerUpdate = 1;
				m_drawData[fg::Assoc::Custom+4].instancesPerUpdate = 1;
				m_drawData[fg::Assoc::Custom+5].instancesPerUpdate = 1;
				m_drawData[fg::Assoc::Custom+6].instancesPerUpdate = 1;
			}
			else
				m_drawData.addDraw(0,0,fg::Triangles);
		}
		
		/////////////////////////////////////////////////////////////
		fm::Result SpriteManagerBaseNonTemplate::loadShader(fm::Delegate<void,fg::ShaderSource &> sourceTransformer)
		{
			ShaderSource vertSource,fragSource;
			
			m_useInstancing = ::priv::so_loader.getProcAddr("glVertexAttribDivisor") != nullptr;
			
			if (useInstancing())
			{
				vertSource=ShaderSource{130,{{"FRONTIER_MODEL ",""},
											 {"FRONTIER_VIEW  ",""},
											 {"FRONTIER_PROJ  ",""},
											 {"FRONTIER_TEXMAT",""},
											 {"FRONTIER_POS   ",""},
											 {"FRONTIER_CUSTOM_0",""},
											 {"FRONTIER_CUSTOM_1",""},
											 {"FRONTIER_CUSTOM_2",""},
											 {"FRONTIER_CUSTOM_3",""},
											 {"FRONTIER_CUSTOM_4",""},
											 {"FRONTIER_CUSTOM_5",""}},
											{{"mat4 FRONTIER_TEXMAT","u_texMat  "},
											 {"mat4 FRONTIER_MODEL ","u_modelMat"},
											 {"mat4 FRONTIER_VIEW  ","u_viewMat "},
											 {"mat4 FRONTIER_PROJ  ","u_projMat "}},
											{{"vec2 FRONTIER_POS     ","in_tpt"},
											 {"vec3 FRONTIER_CUSTOM_0","in_pos"},
											 {"vec2 FRONTIER_CUSTOM_1","in_dir"},
											 {"vec2 FRONTIER_CUSTOM_2","in_siz"},
											 {"vec2 FRONTIER_CUSTOM_3","in_uvp"},
											 {"vec2 FRONTIER_CUSTOM_4","in_uvs"},
											 {"vec4 FRONTIER_CUSTOM_5","in_clr"}},
											{{"vec2","va_texp"},
											 {"vec4","va_clr"}},
											"",
											{},
											"",
											fg::VertexShader};
				
					
				fragSource=ShaderSource{130,{},
											{{"sampler2D","u_tex"}},
											{{"vec2","va_texp"},
											 {"vec4","va_clr"}},
											{{"vec4","out_color"}},
											"",
											{},
											R"(
	out_color = texture2D(u_tex,va_texp) * va_clr;
											  )",
											fg::FragmentShader};
				
				if (m_useFrames)
				{
					vertSource.defines.push_back(ShaderSource::DefineData{"FRONTIER_CUSTOM_6",""});
					vertSource.inputs.push_back( ShaderSource::VariableData{"vec2 FRONTIER_CUSTOM_6","in_frm"});
					vertSource.mainBody = R"(
	vec2 coef0011 = vec2(greaterThan(in_tpt,vec2(1.5,1.5)));
	vec2 coef0101 = mod(in_tpt,vec2(2.0,2.0));
	vec2 locp = coef0011 * in_siz + (coef0101 - coef0011) * in_frm;
	
	vec2 r = vec2(in_dir.y,-in_dir.x);
	
	gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_pos.xy + locp.x * r + locp.y * in_dir,in_pos.z,1.0);
	
	vec2 va_nrmp = coef0011 * in_uvs + (coef0101 - coef0011) * in_frm;
	va_texp = vec2(u_texMat * vec4(in_uvp + va_nrmp,0.0,1.0));
	
	va_clr = in_clr;
											)";
					
					
				}
				else
				{
					vertSource.mainBody = R"(
	vec2 locp = in_siz * in_tpt;
	
	vec2 r = vec2(in_dir.y,-in_dir.x);
	
	gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_pos.xy + locp.x * r + locp.y * in_dir,in_pos.z,1.0);
	
	va_texp = vec2(u_texMat * vec4(in_uvp + in_tpt * in_uvs,0.0,1.0));
	
	va_clr = in_clr;
											)";
				}
			}
			else
			{
				vertSource=ShaderSource{130,{{"FRONTIER_MODEL ",""},
											 {"FRONTIER_VIEW  ",""},
											 {"FRONTIER_PROJ  ",""},
											 {"FRONTIER_POS   ",""},
											 {"FRONTIER_CLR   ",""},
											 {"FRONTIER_CLRMAT",""},
											 {"FRONTIER_TEXPOS",""},
											 {"FRONTIER_TEXMAT",""}},
											{{"mat4 FRONTIER_TEXMAT","u_texUVMat"},
											 {"mat4 FRONTIER_MODEL ","u_modelMat"},
											 {"mat4 FRONTIER_VIEW  ","u_viewMat "},
											 {"mat4 FRONTIER_CLRMAT","u_colorMat"},
											 {"mat4 FRONTIER_PROJ  ","u_projMat "}},
											{{"vec3 FRONTIER_POS     ","in_pos   "},
											 {"vec2 FRONTIER_TEXPOS  ","in_texpos"},
											 {"vec4 FRONTIER_CLR     ","in_color "}},
											{{"vec4","va_clr "},
											 {"vec2","va_texp"}},
											"",
											{},
											R"(
					gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_pos,1.0);
					
					va_clr  = u_colorMat * in_color;
					va_texp = (u_texUVMat * vec4(in_texpos,0.0,1.0)).xy;
											   )",
											fg::VertexShader};
				
				fragSource=ShaderSource{130,{},
											{{"sampler2D","u_tex"}},
											{{"vec4","va_clr   "},
											 {"vec2","va_texp  "}},
											{{"vec4","out_color"}},
											"",
											{},
											R"(
					out_color = va_clr * texture2D(u_tex,va_texp);
											   )",
											fg::FragmentShader};
			}
			
			sourceTransformer(vertSource);
			sourceTransformer(fragSource);
			
			m_shaderResult = m_shader.loadFromMemory(vertSource,fragSource);
		
			m_shader.regTexture("u_tex");
			
			return m_shaderResult;
		}

		/////////////////////////////////////////////////////////////
		SpriteManagerBaseNonTemplate::SpriteManagerBaseNonTemplate(bool useFrames,
																   fg::Glyph (*glyphGetterFunc)(SpriteManagerBaseNonTemplate*,fm::Size)) : 
																		m_useFrames(useFrames),
																		m_posProp(m_drawData[fg::Assoc::Custom+0],3),
																		m_dirProp(m_drawData[fg::Assoc::Custom+1],2),
																		m_sizProp(m_drawData[fg::Assoc::Custom+2],2),
																		m_uvpProp(m_drawData[fg::Assoc::Custom+3],2),
																		m_uvsProp(m_drawData[fg::Assoc::Custom+4],2),
																		m_frmProp(m_drawData[fg::Assoc::Custom+6],2),
																		m_clrProp(m_drawData[fg::Assoc::Custom+5],4),
																		m_vertPtsProp(m_drawData[fg::Assoc::Position ],3,m_useFrames ? 54 : 6),
																		m_vertUVsProp(m_drawData[fg::Assoc::TextureUV],2,m_useFrames ? 54 : 6),
																		m_vertClrProp(m_drawData[fg::Assoc::Color    ],4,m_useFrames ? 54 : 6),
																		m_spriteCount(0),
																		m_useInstancing(false),
																		m_glyphGetterFunc(glyphGetterFunc)
		{
			
		}

		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::onDrawTex(fg::ShaderManager &shader,fg::Texture &tex)
		{
			if (!m_spriteCount) return;
			
			if (useInstancing())
			{
				m_posProp.prepareDraw();
				m_dirProp.prepareDraw();
				m_sizProp.prepareDraw();
				m_uvpProp.prepareDraw();
				m_uvsProp.prepareDraw();
				m_frmProp.prepareDraw();
				m_clrProp.prepareDraw();
			}
			else
			{
				m_vertPtsProp.prepareDraw();
				m_vertUVsProp.prepareDraw();
				m_vertClrProp.prepareDraw();
			}
			
			m_shader.getModelStack().top(shader.getModelStack().top());
			m_shader.getViewStack().top(shader.getViewStack().top());
			m_shader.getProjStack().top(shader.getProjStack().top());
			
			m_shader.getTexUVStack().push().mul(tex.getPixToUnitMatrix());
			
			m_shader.useTexture(tex);
			m_shader.draw(m_drawData);
			
			m_shader.getTexUVStack().pop();
		}
		
		/////////////////////////////////////////////////////////////
		fm::Size SpriteManagerBaseNonTemplate::getCapacity() const
		{
			return m_posProp.getCapacity();
		}

		/////////////////////////////////////////////////////////////
		void SpriteManagerBaseNonTemplate::setAutoShrink(bool automatic)
		{
			if (useInstancing())
			{
				m_posProp.setAutoShrink(automatic);
				m_dirProp.setAutoShrink(automatic);
				m_sizProp.setAutoShrink(automatic);
				m_uvpProp.setAutoShrink(automatic);
				m_uvsProp.setAutoShrink(automatic);
				m_frmProp.setAutoShrink(automatic);
				m_clrProp.setAutoShrink(automatic);
			}
			else
			{
				m_vertPtsProp.setAutoShrink(automatic);
				m_vertUVsProp.setAutoShrink(automatic);
				m_vertClrProp.setAutoShrink(automatic);
			}
		}
		
		/////////////////////////////////////////////////////////////
		bool SpriteManagerBaseNonTemplate::getAutoShrink() const
		{
			if (useInstancing())
			{
				return m_posProp.getAutoShrink();
			}
			else
			{
				return m_vertPtsProp.getAutoShrink();
			}
		}
	}
}

