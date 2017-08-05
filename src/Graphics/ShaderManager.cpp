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
#include <FRONTIER/Graphics/FixedShaderManager.hpp>
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/Attribute.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Camera.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	////////////////////////////////////////////////////////////
	ShaderManager::ShaderManager() : m_stacks(3,fm::MatrixStack<4,4,float>(fm::mat4())),
									 m_matNames(9,std::string()),
									 m_matState(9,UnknownMat)
	{

	}
	
	/////////////////////////////////////////////////////////////
	ShaderManager::ShaderManager(ShaderManager &&manager)
	{
		manager.swap(*this);
	}
	
	/////////////////////////////////////////////////////////////
	ShaderManager &ShaderManager::operator=(ShaderManager &&manager)
	{
		return this->swap(manager);
	}
	
	/////////////////////////////////////////////////////////////
	ShaderManager &ShaderManager::swap(ShaderManager &manager)
	{
		Shader::swap(manager);
		
		m_stacks     .swap(manager.m_stacks     );
		m_assocPoints.swap(manager.m_assocPoints);
		m_texUseNames.swap(manager.m_texUseNames);
		m_texNames   .swap(manager.m_texNames   );
		m_matNames   .swap(manager.m_matNames   );
		m_matState   .swap(manager.m_matState   );
		
		std::swap(m_cam,manager.m_cam);
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	ShaderManager::~ShaderManager()
	{

	}

	////////////////////////////////////////////////////////////
	void ShaderManager::clearData()
	{
		m_cam = fm::Camera();

		m_assocPoints.clear();
		m_texNames    = std::vector<std::string>();
		m_texUseNames = std::vector<std::string>();

		C(m_matState.size())
			m_matState[i] = UnknownMat;

		Shader::clearData();
	}

	////////////////////////////////////////////////////////////
	ShaderManager &ShaderManager::associate(const std::string &attrName,AssocPoint point,bool overWrite)
	{
		if (overWrite || m_assocPoints.find(point) == m_assocPoints.end())
		{
			m_assocPoints[point] = attrName;
			
			if (point == Assoc::Color)
			{
				bind();
				glVertexAttrib4f(getAttribLocation(attrName),1,1,1,1);
			}
		}
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	ShaderManager &ShaderManager::associate(const std::string &positionName,const std::string &colorName,const std::string &texPositionName,const std::string &normalName,const std::string &tangentName,const std::string &bitangentName)
	{
		associate(positionName,Assoc::Position);
		associate(colorName,Assoc::Color);
		associate(normalName,Assoc::Normal);
		associate(texPositionName,Assoc::TextureUV);
		associate(tangentName,Assoc::Tangent);
		associate(bitangentName,Assoc::Bitangent);
		
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	ShaderManager &ShaderManager::setUniformNames(const std::string &modelMat,
												  const std::string &viewMat,
												  const std::string &projMat,
												  const std::string &normalMat,
												  const std::string &colorMat,
												  const std::string &texUVMat,
												  const std::string &plyPos,
												  const std::string &plyView,
												  const std::string &time)
	{
		m_matNames[0] = projMat;
		m_matNames[1] = viewMat;
		m_matNames[2] = plyPos;
		m_matNames[3] = plyView;
		m_matNames[4] = modelMat;
		m_matNames[5] = normalMat;
		m_matNames[6] = colorMat;
		m_matNames[7] = texUVMat;
		m_matNames[8] = time;

		C(m_matState.size())
			m_matState[i] = (m_matNames[i].length() ? UnknownMat : MissingMat);
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	ShaderManager &ShaderManager::regTexture(const std::string &texName,const std::string &texInUse)
	{
		m_texNames.push_back(texName);
		m_texUseNames.push_back(texInUse);
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	ShaderManager &ShaderManager::useTexture(fm::Ref<const fg::Texture> tex,fm::Size texIndex)
	{
		if (texIndex < m_texNames.size())
		{
			setUniform(m_texNames[texIndex],(const fg::Texture*)tex);

			if (m_texUseNames[texIndex].size() != 0)
				setUniform(m_texUseNames[texIndex],(tex ? 1 : 0));
		}
		
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	fm::Camera &ShaderManager::getCamera()
	{
		return m_cam;
	}
	
	////////////////////////////////////////////////////////////
	const fm::Camera &ShaderManager::getCamera() const
	{
		return m_cam;
	}

	////////////////////////////////////////////////////////////
	ShaderManager &ShaderManager::clearAssociations()
	{
		m_assocPoints.clear();
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	ShaderManager &ShaderManager::clearTextures()
	{
		m_texNames    = std::vector<std::string>();
		m_texUseNames = std::vector<std::string>();
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	fm::Result ShaderManager::update()
	{
		C(m_matState.size())
			if (m_matState[i] == UnknownMat)
				m_matState[i] = (hasUniform(m_matNames[i]) ? FoundMat : MissingMat);
		
		fm::Result res;
		
		if (m_matState[0] == FoundMat)
			res += glCheck(setUniform(m_matNames[0],m_cam.getProjMat()));

		if (m_matState[1] == FoundMat)
			res += glCheck(setUniform(m_matNames[1],m_cam.getViewMat()));

		if (m_matState[2] == FoundMat)
			res += glCheck(setUniform(m_matNames[2],m_cam.getPos()));

		if (m_matState[3] == FoundMat)
			res += glCheck(setUniform(m_matNames[3],m_cam.getViewDir()));

		if (m_matState[4] == FoundMat)
			res += glCheck(setUniform(m_matNames[4],m_stacks[0].top()));

		if (m_matState[5] == FoundMat)
			res += glCheck(setUniform(m_matNames[5],m_stacks[0].top().inverse().transpose()));

		if (m_matState[6] == FoundMat)
			res += glCheck(setUniform(m_matNames[6],m_stacks[1].top()));

		if (m_matState[7] == FoundMat)
			res += glCheck(setUniform(m_matNames[7],m_stacks[2].top()));

		if (m_matState[8] == FoundMat)
			res += glCheck(setUniform(m_matNames[8],float(m_clk.getSeconds())));
		
		return res;
	}
	
	////////////////////////////////////////////////////////////
	fm::Result ShaderManager::postProcess(const std::string *data,const unsigned int *types,unsigned int count)
	{
		(void)types;
		
		std::vector<std::string> names[] = {{"proj","projmat","proj_mat","projection","projectionmat","projection_mat"},
											{"view","viewmat","view_mat"},
											{"plypos","ply_pos","playerpos","player_pos"},
											{"dir","viewdir","view_dir","direction","viewdirection","view_direction"},
											{"model","modelmat","model_mat"},
											{"normmat","norm_mat","normalmat","normal_mat"},
											{"clrmat","clr_mat","colormat","color_mat"},
											{"texturemat","texture_mat","texmat","tex_mat"},
											{"time","secs","seconds","clock"},
											
											{"pos","position","pts"},
											{"clr","color"},
											{"nrm","norm","normal"},
											{"texuv","texcoord","texpos","textureuv","texturecoord","texturepos","tex_uv","tex_coord","tex_pos","texture_uv","texture_coord","texture_pos"},
											{"tan","tangent"},
											{"bitan","bitangent"},
											{"custom","arbitrary"}};
		
		fm::Size matCount = 9;
		
		auto findName = [&](const std::string &source,fm::Size offset,fm::Size &matId,fm::Size &delta) -> bool
		{
			for (fm::Size nameClass = 0;nameClass < sizeof(names)/sizeof(*names);++nameClass)
			{
				for (auto &name : names[nameClass])
				{
					if (offset + name.size() < source.size())
					{
						bool match = true;
						for (fm::Size i=0;i<name.size() && match;++i)
						{
							char c = source[i+offset];
							if (c <= 'Z' && c >= 'A') c += 'a'-'A';
							match = match && c == name[i];
						}
						
						if (match)
						{
							auto isWhiteSpace = [](char c) -> bool {
								return c == ' ' || c == '\t' || c == '\r' || c == '\n';
							};
							
							bool custom = (nameClass+1 == sizeof(names)/sizeof(*names));
							
							if (custom)
							{
								int customId = 0;
								
								fm::Size k;
								for (k=0;k+offset+name.size() < source.size() && !isWhiteSpace(source[k+offset+name.size()]);++k)
								{
									char c = source[k+offset+name.size()];
									
									if (c <= '9' && c >= '0')
									{
										customId = customId*10 + c - '0';
									}
								}
								
								matId = nameClass + customId;
								delta = name.size() + k;
								return true;
							}
							else if (offset+name.size() == source.size() || isWhiteSpace(source[offset+name.size()]))
							{
								matId = nameClass;
								delta = name.size();
								return true;							
							}
						}
					}
				}
			}
			return false;
		};
		
		auto findAssoc = [](const std::string &source,fm::Size offset,std::string &assoc) -> bool
		{
			while (offset < source.size() && (source[offset] == ' ' || source[offset] == '\t')) ++offset;
			
			std::string excl = " \t\r\n;";
			
			while (offset < source.size() && excl.find(source[offset]) == std::string::npos) assoc += source[offset],++offset;
			
			return assoc.length();
		};
		
		std::string frontierTag = "FRONTIER_";
		
		for (auto str = data;str != data+count;++str)
		{
			const std::string &source = *str;
			
			fm::Size pos = source.find(frontierTag,0);
			while (pos != std::string::npos)
			{
				fm::Size matId,delta;
				if (findName(source,pos + frontierTag.size(),matId,delta))
				{
					std::string assoc;
					if (findAssoc(source,pos + frontierTag.size() + delta,assoc))
					{
						if (matId < matCount)
						{
							// cout << "found matrix " << names[matId][0] << " as " << assoc << endl;
							m_matNames[matId] = assoc;
							m_matState[matId] = UnknownMat;							
						}
						else
						{
							// cout << "found attr " << names[matId][0] << " as " << assoc << endl;
							associate(assoc,Assoc::Unused + int(matId - matCount + 1));
						}
					}
				}
				pos = source.find(frontierTag,pos+1);
			}
		}
		
		return fm::Result();
	}

	////////////////////////////////////////////////////////////
	fm::Result ShaderManager::prepareDraw(const fg::DrawData &data)
	{
		fm::Result res = update();
		
		bind();
		
		setBlendMode(m_blendMode);

		for (auto it : m_assocPoints)
		{
			if (data.hasAttr((fg::Assoc::Point)it.first) && hasAttribute(it.second))
			{
				const fg::Attribute &attr = data[(fg::Assoc::Point)it.first];

				fg::Buffer::bind(attr.buf,fg::ArrayBuffer),
				res += setAttribPointer(it.second,attr.components,attr.componentType,false,nullptr,attr.stride);
				
				if (instancingAvailable())
					glVertexAttribDivisor(getAttribLocation(it.second),attr.instancesPerUpdate);
			}
		}
		
		return res;
	}

	////////////////////////////////////////////////////////////
	fm::Result ShaderManager::draw(const fg::DrawData &data)
	{
		return draw(data,0,data.getDrawCount());
	}

	////////////////////////////////////////////////////////////
	fm::Result ShaderManager::draw(const fg::DrawData &data,fm::Size indexSet)
	{
		return draw(data,indexSet,1);
	}

	////////////////////////////////////////////////////////////
	fm::Result ShaderManager::draw(const fg::DrawData &data,fm::Size indexBeg,fm::Size indexCount)
	{
		fm::Result res;
		
		res = prepareDraw(data);
		if (!res) return res;

		C(indexCount)
		{
			const fg::DrawCall &draw = data.getDraw(indexBeg+i);

			fg::Buffer::bind(draw.buf,fg::IndexBuffer);
			
			if (draw.componentType)
			{
				if (instancingAvailable())
				{
					res += glCheck(glDrawElementsInstanced(draw.primitive,draw.indexCount,draw.componentType,nullptr,draw.instances));
				}
				else
				{
					C(draw.instances)
						res += glCheck(glDrawElements(draw.primitive,draw.indexCount,draw.componentType,nullptr));
				}
			}
			else
			{
				fm::Size len = draw.drawLen;
				if (len == 0 && data.hasAttr(fg::Assoc::Position))
					len = data[fg::Assoc::Position].count;
					
				if (len)
				{
					if (instancingAvailable())
					{
						glDrawArraysInstanced(draw.primitive,draw.drawBeg,len,draw.instances);
					}
					else
					{
						C(draw.instances)
							glDrawArrays(draw.primitive,draw.drawBeg,len);
					}
				}
			}
		}
		
		if (indexCount == 0 && data.hasAttr(fg::Assoc::Position))
		{
			res += glCheck(glDrawArrays(fg::Triangles,0,data[fg::Assoc::Position].count));
		}

		for (auto it : m_assocPoints)
		{
			if (data.hasAttr((fg::Assoc::Point)it.first) && hasAttribute(it.second))
			{
				enableAttribPointer(it.second,false);
				
				if (instancingAvailable())
					glVertexAttribDivisor(getAttribLocation(it.second),0);
			}
		}
		
		res += glCheck((void)0);
		
		return res;
	}

	////////////////////////////////////////////////////////////
	fm::MatrixStack<4,4,float> &ShaderManager::getModelStack()
	{
		return m_stacks[0];
	}

	////////////////////////////////////////////////////////////
	fm::MatrixStack<4,4,float> &ShaderManager::getViewStack()
	{
		return m_cam.getViewStack();
	}

	////////////////////////////////////////////////////////////
	fm::MatrixStack<4,4,float> &ShaderManager::getProjStack()
	{
		return m_cam.getProjStack();
	}

	////////////////////////////////////////////////////////////
	fm::MatrixStack<4,4,float> &ShaderManager::getColorStack()
	{
		return m_stacks[1];
	}

	////////////////////////////////////////////////////////////
	fm::MatrixStack<4,4,float> &ShaderManager::getTexUVStack()
	{
		return m_stacks[2];
	}
	
	/////////////////////////////////////////////////////////////
	ShaderManager *ShaderManager::getDefaultShader()
	{
		if (Shader::isAvailable())
		{
			ShaderManager *shader = new ShaderManager;
			
			shader->loadFromMemory(""
"#version 110\n"
"\n"
"#define FRONTIER_MODEL\n"
"#define FRONTIER_VIEW\n"
"#define FRONTIER_PROJ\n"
"#define FRONTIER_POS\n"
"#define FRONTIER_CLR\n"
"#define FRONTIER_TEXPOS\n"
"#define FRONTIER_TEXMAT\n"
"#define FRONTIER_CLRMAT\n"
"\n"
"uniform mat4 FRONTIER_MODEL  u_modelMat;\n"
"uniform mat4 FRONTIER_VIEW   u_viewMat;\n"
"uniform mat4 FRONTIER_PROJ   u_projMat;\n"
"uniform mat4 FRONTIER_TEXMAT u_texUVMat;\n"
"uniform mat4 FRONTIER_CLRMAT u_colorMat;\n"
"\n"
"attribute vec3 FRONTIER_POS    in_pos;\n"
"attribute vec4 FRONTIER_CLR    in_color;\n"
"attribute vec2 FRONTIER_TEXPOS in_texpos;\n"
"\n"
"varying vec4 va_color;\n"
"varying vec2 va_texpos;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_pos,1.0);\n"
"	\n"
"	va_color  = u_colorMat * in_color;\n"
"	va_texpos = (u_texUVMat * vec4(in_texpos,0.0,1.0)).xy;\n"
"}",""
"#version 110\n"
"\n"
"uniform sampler2D u_tex;\n"
"uniform bool u_useTex;\n"
"\n"
"varying vec4 va_color;\n"
"varying vec2 va_texpos;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = va_color;\n"
"	if (u_useTex)\n"
"		gl_FragColor *= texture2D(u_tex,va_texpos);\n"
"}\n");
			shader->regTexture("u_tex","u_useTex");
			shader->useTexture(nullptr);
			return shader;
		}
		
		return new FixedShaderManager;
	}
}
