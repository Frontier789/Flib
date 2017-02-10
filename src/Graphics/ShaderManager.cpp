////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)		   ///
///																	///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			 ///
/// condition that this disclaimer is not modified/removed.			///
/// You may not misclaim the origin of this software.				  ///
///																	///
/// If you use this software in your program/project a				 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							///
///																	///
/// You should have received a copy of GNU GPL with this software	  ///
///																	///
////////////////////////////////////////////////////////////////////////// -->
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
									 m_cam(nullptr),
									 m_matNames(8,std::string()),
									 m_matState(8,UnknownMat)
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
		m_cam = nullptr;

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
		if (overWrite)
			m_assocPoints[point] = attrName;
		else if (m_assocPoints.find(point) == m_assocPoints.end())
				 m_assocPoints[point] = attrName;
		
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
	ShaderManager &ShaderManager::setMatrices(const std::string &modelMat,
											  const std::string &normalMat,
											  const std::string &colorMat,
											  const std::string &texUVMat)
	{
		m_matNames[4] = modelMat;
		m_matNames[5] = normalMat;
		m_matNames[6] = colorMat;
		m_matNames[7] = texUVMat;

		m_matState[4] = modelMat.length()  ? UnknownMat : MissingMat;
		m_matState[5] = normalMat.length() ? UnknownMat : MissingMat;
		m_matState[6] = colorMat.length()  ? UnknownMat : MissingMat;
		m_matState[7] = texUVMat.length()  ? UnknownMat : MissingMat;
		
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
	ShaderManager &ShaderManager::changeCamera(fm::Ref<fm::Camera> cam)
	{
		m_cam = cam;
		
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	ShaderManager &ShaderManager::setCamera(fm::Ref<fm::Camera> cam,const std::string &projMat,const std::string &viewMat,const std::string &plyPos,const std::string &plyView)
	{
		changeCamera(cam);

		m_matNames[0] = projMat;
		m_matNames[1] = viewMat;
		m_matNames[2] = plyPos;
		m_matNames[3] = plyView;

		C(m_matState.size())
			m_matState[i] = (m_matNames[i].length() ? UnknownMat : MissingMat);
			
		return *this;
	}
	////////////////////////////////////////////////////////////
	fm::Camera *ShaderManager::getCamera()
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
	ShaderManager &ShaderManager::update()
	{
		C(m_matState.size())
			if (m_matState[i] == UnknownMat)
				m_matState[i] = (hasUniform(m_matNames[i]) ? FoundMat : MissingMat);

		if (m_cam)
		{
			if (m_matState[0] == FoundMat)
				setUniform(m_matNames[0],m_cam->getProjMat());

			if (m_matState[1] == FoundMat)
				setUniform(m_matNames[1],m_cam->getViewMat());

			if (m_matState[2] == FoundMat)
				setUniform(m_matNames[2],m_cam->getPos());

			if (m_matState[3] == FoundMat)
				setUniform(m_matNames[3],m_cam->getViewDir());
		}
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	fm::Result ShaderManager::prepareDraw(const fg::DrawData &data)
	{
		update();
		
		bind();
		
		setBlendMode(m_blendMode);
		
		fm::Result res;

		if (m_matState[4] == FoundMat)
			res += glCheck(setUniform(m_matNames[4],m_stacks[0].top()));

		if (m_matState[5] == FoundMat)
			res += glCheck(setUniform(m_matNames[5],m_stacks[0].top().inverse().transpose()));

		if (m_matState[6] == FoundMat)
			res += glCheck(setUniform(m_matNames[6],m_stacks[1].top()));

		if (m_matState[7] == FoundMat)
			res += glCheck(setUniform(m_matNames[7],m_stacks[2].top()));

		for (std::map<AssocPoint,std::string>::const_iterator it = m_assocPoints.begin();it != m_assocPoints.end();++it)
		{
			if (data.hasAttr((fg::Assoc::Point)it->first) && hasAttribute(it->second))
			{
				const fg::Attribute &attr = data[(fg::Assoc::Point)it->first];

				fg::Buffer::bind(attr.buf,fg::ArrayBuffer),
				res += setAttribPointer(it->second,attr.components,attr.componentType,false,nullptr,attr.stride);
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
				res += glCheck(glDrawElements(draw.primitive,draw.indexCount,draw.componentType,nullptr));
			else
				res += glCheck(glDrawArrays(draw.primitive,draw.drawBeg,draw.drawLen));
		}
		
		if (indexCount == 0 && data.hasAttr(fg::Assoc::Position))
		{
			res += glCheck(glDrawArrays(fg::Triangles,0,data[fg::Assoc::Position].count));
		}
		
		res += glCheck((void)0);

		for (std::map<AssocPoint,std::string>::const_iterator it = m_assocPoints.begin();it != m_assocPoints.end();++it)
		{
			if (data.hasAttr((fg::Assoc::Point)it->first) && hasAttribute(it->second))
			{
				enableAttribPointer(it->second,false);
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
	fm::MatrixStack<4,4,float> &ShaderManager::getColorStack()
	{
		return m_stacks[1];
	}

	////////////////////////////////////////////////////////////
	fm::MatrixStack<4,4,float> &ShaderManager::getTexUVStack()
	{
		return m_stacks[2];
	}
}
