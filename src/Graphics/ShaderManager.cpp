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
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/ShaderSource.hpp>
#include <FRONTIER/Graphics/Attribute.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Camera.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <algorithm>

namespace fg
{
	////////////////////////////////////////////////////////////
	ShaderManager::ShaderManager() : m_stacks(3,fm::MatrixStack<4,4,float>(fm::mat4())),
									 m_uniNames(10,std::string()),
									 m_uniState(10,UnknownUni)
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
		m_uniNames   .swap(manager.m_uniNames   );
		m_uniState   .swap(manager.m_uniState   );
		
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

		C(m_uniState.size())
			m_uniState[i] = UnknownUni;

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
		m_uniNames[0] = projMat;
		m_uniNames[1] = viewMat;
		m_uniNames[2] = plyPos;
		m_uniNames[3] = plyView;
		m_uniNames[4] = modelMat;
		m_uniNames[5] = normalMat;
		m_uniNames[6] = colorMat;
		m_uniNames[7] = texUVMat;
		m_uniNames[8] = time;

		C(m_uniState.size())
			m_uniState[i] = (m_uniNames[i].length() ? UnknownUni : MissingUni);
		
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
	fm::Clock &ShaderManager::getClock()
	{
		return m_clk;
	}
	
	////////////////////////////////////////////////////////////
	const fm::Clock &ShaderManager::getClock() const
	{
		return m_clk;
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
		C(m_uniState.size())
			if (m_uniState[i] == UnknownUni)
				m_uniState[i] = (hasUniform(m_uniNames[i]) ? FoundUni : MissingUni);
		
		fm::Result res;
		
		if (m_uniState[0] == FoundUni)
			res += glCheck(setUniform(m_uniNames[0],m_cam.getProjMat()));

		if (m_uniState[1] == FoundUni)
			res += glCheck(setUniform(m_uniNames[1],m_cam.getViewMat()));

		if (m_uniState[2] == FoundUni)
			res += glCheck(setUniform(m_uniNames[2],m_cam.getPosition()));

		if (m_uniState[3] == FoundUni)
			res += glCheck(setUniform(m_uniNames[3],m_cam.getViewDir()));

		if (m_uniState[4] == FoundUni)
			res += glCheck(setUniform(m_uniNames[4],m_stacks[0].top()));

		if (m_uniState[5] == FoundUni)
			res += glCheck(setUniform(m_uniNames[5],m_stacks[0].top().inverse().transpose()));

		if (m_uniState[6] == FoundUni)
			res += glCheck(setUniform(m_uniNames[6],m_stacks[1].top()));

		if (m_uniState[7] == FoundUni)
			res += glCheck(setUniform(m_uniNames[7],m_stacks[2].top()));

		if (m_uniState[8] == FoundUni)
			res += glCheck(setUniform(m_uniNames[8],float(m_clk.getSeconds())));

		if (m_uniState[9] == FoundUni)
			res += glCheck(setUniform(m_uniNames[9],m_cam.getVPMat() * m_stacks[0].top()));
		
		return res;
	}

	namespace {	
		bool starts_with_frntr(const std::string &word)
		{
			std::string s;
			
			for (auto c : word) if (c != '_') s += toupper(c);
			
			return s.find("FRONTIER") == 0 || s.find("FRNTR") == 0;
		}
		
		std::string canonize(const std::string &name)
		{
			std::string s;
			
			for (auto c : name) if (c != '_') s += toupper(c);
			
			if (s.find("FRONTIER") == 0) return s.substr(8);
			if (s.find("FRNTR") == 0)    return s.substr(5);
			
			return s;
		}
		
		const std::vector<std::string> uniform_names[]{
			{"PROJ","PROJMAT","PROJECTION","PROJECTIONMAT"},
			{"VIEW","VIEWMAT"},
			{"PLYPOS","PLAYERPOS","CAMPOS","CAMERAPOS"},
			{"DIR","VIEWDIR","DIRECTION","VIEWDIRECTION"},
			{"MODEL","MODELMAT"},
			{"NORMMAT","NRMMAT","NORMALMAT"},
			{"CLRMAT","COLORMAT"},
			{"TEXTUREMAT","TEXMAT"},
			{"TIME","SECS","SECONDS","CLOCK"},
			{"MVP","MODELVIEWPROJ","MODELVIEWPROJECTION"}
		};
		
		const std::vector<std::string> attr_names[]{
			{"POS","POSITION","PTS"},
			{"CLR","COLOR"},
			{"NRM","NORM","NORMAL"},
			{"TEXUV","TEXCOORD","TEXPOS","TEXTUREUV","TEXTURECOORD","TEXTUREPOS"},
			{"TAN","TANGENT"},
			{"BITAN","BITANGENT"}
		};
		
		template<size_t N>
		size_t find_name_id(std::string name,const std::vector<std::string> (&arr)[N])
		{
			C(N)
				for (auto s : arr[i]) 
					if (s == name)
						return i;
			
			if (name.find("CUSTOM") == 0 || name.find("ARBITRARY") == 0) {
				return N + std::stoi(name.substr(name.find_first_of("0123456789")));
			}
			
			return size_t(-1);
		}
	}

	//////////////////////////////////////////////////////////
	fm::Result ShaderManager::postProcess(const std::string *data,const unsigned int *types,unsigned int count)
	{
		(void)types;
		
		for (auto it = data;it != data + count;++it) {
			const std::string &source = *it;
			
			std::string spacers = " \t\r\n;(){}=";
			size_t c = source.find_first_not_of(spacers,0);
			
			std::vector<std::string> words;
			
			while (c != std::string::npos) {
				size_t e = source.find_first_of(spacers,c);
				
				std::string word = source.substr(c,e-c);
				words.push_back(word);
				
				c = source.find_first_not_of(spacers,e);
			}
			
			for (size_t i=0;i<words.size();++i) {
				
				if ((words[i] == "uniform" || words[i] == "in") && i+3 < words.size()) {
					std::string name;
					if (starts_with_frntr(words[i+1])) name = canonize(words[i+1]);
					if (starts_with_frntr(words[i+2])) name = canonize(words[i+2]);
					
					if (name != "") {
						if (words[i] == "uniform") {
							size_t id = find_name_id(name,uniform_names);
							if (id != size_t(-1)) {
								// cout << "Found uniform of type " << id << " with name " << words[i+3] << endl;
								m_uniNames[id] = words[i+3];
								m_uniState[id] = UnknownUni;
							}
						} else {
							size_t id = find_name_id(name,attr_names);
							if (id != size_t(-1)) {
								// cout << "Found attribute of type " << id << " with name " << words[i+3] << endl;
								associate(words[i+3],Assoc::Unused + int(id + 1));
							}
						}
					}
				}
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

		m_vao.setAttributes(data,[&](AssocPoint pt) -> fm::Size {return getAttribLocation(m_assocPoints[pt]);});
		
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
						res += glCheck(glDrawArraysInstanced(draw.primitive,draw.drawBeg,len,draw.instances));
					}
					else
					{
						C(draw.instances)
							res += glCheck(glDrawArrays(draw.primitive,draw.drawBeg,len));
					}
				}
			}
		}
		
		if (indexCount == 0 && data.hasAttr(fg::Assoc::Position))
		{
			res += glCheck(glDrawArrays(fg::Triangles,0,data[fg::Assoc::Position].count));
		}
		
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
	ShaderManager &ShaderManager::copyStacksFrom(const ShaderManager &source)
	{
		m_stacks = source.m_stacks;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	ShaderManager *ShaderManager::getDefaultShader(fm::Delegate<void,ShaderSource&> transf)
	{
		if (Shader::isAvailable())
		{
			ShaderManager *shader = new ShaderManager;
			
			ShaderSource vertSource{130,{{"FRONTIER_MODEL ",""},
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
										{{"vec4","va_color "},
										 {"vec2","va_texpos"}},
										"",
										{},
										R"(
				gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_pos,1.0);
				
				va_color  = u_colorMat * in_color;
				va_texpos = (u_texUVMat * vec4(in_texpos,0.0,1.0)).xy;
										   )",
										fg::VertexShader};
			
			ShaderSource fragSource{130,{},
										{{"sampler2D","u_tex"},
										 {"bool","u_useTex"}},
										{{"vec4","va_color "},
										 {"vec2","va_texpos"}},
										{{"vec4","out_color"}},
										"",
										{},
										R"(
				out_color = va_color;
				if (u_useTex)
					out_color *= texture2D(u_tex,va_texpos);
										   )",
										fg::FragmentShader};
			
			transf(vertSource);
			transf(fragSource);

			shader->loadFromMemory(vertSource,fragSource);
			shader->regTexture("u_tex","u_useTex");
			shader->useTexture(nullptr);
			return shader;
		}
		
		return nullptr;
	}
}
