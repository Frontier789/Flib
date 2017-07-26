#include <Frontier.hpp>
#include <iostream>
#include <fstream>

using namespace std;

#include "SpriteClasses.hpp"


/*
 - disable/enable only incorrectly set vertex attrib array settings
 
 changelog:
 	added instancing
 	updated sprite engine test
 	
 
*/

namespace finalVersion
{
	class SpriteManager;
	
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to easily manage a lightweight sprite
	///
	/// Requires a SpriteManager
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Sprite
	{
		SpriteManager *m_manager; ///< The manager of the sprite
		fm::Size m_id; ///< The id of the sprite
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~Sprite();
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the sprite with given manager and image
		/// 
		/// @param manager The manager
		/// @param imgID The image id
		/// 
		/////////////////////////////////////////////////////////////
		Sprite(SpriteManager &manager,ImageID imgID);
		
        /////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// @param sprite The sprite to copy
		/// 
        /////////////////////////////////////////////////////////////
		Sprite(const Sprite &sprite);
		
        /////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param sprite The sprite to move
		/// 
        /////////////////////////////////////////////////////////////
		Sprite(Sprite &&sprite);
		
        /////////////////////////////////////////////////////////////
		/// @brief Copy assignment
		/// 
		/// @param sprite The sprite to copy
		/// 
		/// @return Reference to itself
		/// 
        /////////////////////////////////////////////////////////////
		Sprite &operator=(const Sprite &sprite);
		
        /////////////////////////////////////////////////////////////
		/// @brief Move assignment
		/// 
		/// @param sprite The sprite to move
		/// 
		/// @return Reference to itself
		/// 
        /////////////////////////////////////////////////////////////
		Sprite &operator=(Sprite &&sprite);
		
        /////////////////////////////////////////////////////////////
		/// @brief swap two sprites
		/// 
		/// @param sprite The sprite to swap with
		/// 
		/// @return Reference to itself
		/// 
        /////////////////////////////////////////////////////////////
		Sprite &swap(Sprite &sprite);

		/////////////////////////////////////////////////////////////
		/// @brief Set the onscreen position of the sprite
		/// 
		/// @param pos The position on the screen
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Sprite &setPosition(const fm::vec3 &pos);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the onscreen position of the sprite
		/// 
		/// @return The onscreen position
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec3 getPosition() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the direction the sprite is facing
		/// 
		/// @param dir The direction
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Sprite &setDirection(const fm::vec2 &dir);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the direction the sprite is facing
		/// 
		/// @return The direction of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getDirection() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the onscreen size of the sprite
		/// 
		/// @param size The size on the screen
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Sprite &setSize(const fm::vec2 &size);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the onscreen size of the sprite
		/// 
		/// @return The onscreen size
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the image of the sprite
		/// 
		/// @param imgID The image id
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Sprite &setImageID(ImageID imgID);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the image of the sprite
		/// 
		/// @return The image id
		/// 
		/////////////////////////////////////////////////////////////
		ImageID getImageID() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the id of the sprite
		/// 
		/// Mainly used by SpriteManager
		/// 
		/// @return The id
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getId() const;
	
	private:
		/////////////////////////////////////////////////////////////
		/// @brief Set the id of the sprite
		/// 
		/// Mainly used by SpriteManager
		/// 
		/// @param index The new index
		/// 
		/////////////////////////////////////////////////////////////
		void setId(fm::Size index);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the manager of the sprite
		/// 
		/// Mainly used by SpriteManager
		/// 
		/// @param manager The manager
		/// 
		/////////////////////////////////////////////////////////////
		void setManager(SpriteManager *manager);
		
		friend SpriteManager;
	};
	
	
	
	
	/////////////////////////////////////////////////////////////
	/// @brief Class responsible for updating a float attribute
	/// 
	/////////////////////////////////////////////////////////////
	class FloatAttributeUpdater
	{
	public:
		std::vector<float> m_data; ///< The data
		fg::Attribute &m_attrib;   ///< Reference to the attribute needed to be updated
		fm::Size m_firstUpdated;   ///< The index of the first item updated
		fm::Size m_lastUpdated;    ///< The index of the last item updated
		fm::Size m_floatPerVec;    ///< Number of floats in a vec
		fm::Size m_capacity;       ///< The number of allocated items
		fm::Size m_uploads;        ///< The number of uploads done
		bool m_lastTimeMap;        ///< Stores whether last time map was used
		float *m_mappedPtr;        ///< The pointer to the mapped data
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param attrib The attribute to keep updated
		/// @param floatPerVec The number of floats in a vec
		/// 
		/////////////////////////////////////////////////////////////
		FloatAttributeUpdater(fg::Attribute &attrib,fm::Size floatPerVec);
		
		/////////////////////////////////////////////////////////////
		/// @brief Register an update
		/// 
		/// @param index The index of the updated item
		/// @param value The new value of the updated item
		/// 
		/////////////////////////////////////////////////////////////
		void update(fm::Size index,const float *value);
		
		/////////////////////////////////////////////////////////////
		/// @brief Register an update from an index
		/// 
		/// @param index The index of the updated item
		/// @param fromIndex The index to copy from
		/// 
		/////////////////////////////////////////////////////////////
		void updateFrom(fm::Size index,fm::Size fromIndex);
		
		/////////////////////////////////////////////////////////////
		/// @brief Fetch value at index
		/// 
		/// @param index The index of th item to fetch
		/// @param The value returned
		/// 
		/////////////////////////////////////////////////////////////
		void fetch(fm::Size index,float *ret) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the allocated capacity
		/// 
		/// @param capacity The new capacity
		/// 
		/////////////////////////////////////////////////////////////
		void setCapacity(fm::Size capacity);
		
		/////////////////////////////////////////////////////////////
		/// @brief Push a new value by increasing the allocated data size
		/// 
		/// @param value The value new item
		/// 
		/////////////////////////////////////////////////////////////
		void push(const float *value);
		
		/////////////////////////////////////////////////////////////
		/// @brief Pop the last item
		/// 
		/////////////////////////////////////////////////////////////
		void pop();
		
		/////////////////////////////////////////////////////////////
		/// @brief Unmap all mapped data
		/// 
		/////////////////////////////////////////////////////////////
		void unMap();
		
		/////////////////////////////////////////////////////////////
		/// @brief Prepare the attribute for being used
		/// 
		/////////////////////////////////////////////////////////////
		void prepareDraw();
	};
	
	/////////////////////////////////////////////////////////////
	FloatAttributeUpdater::FloatAttributeUpdater(fg::Attribute &attrib,
									   fm::Size floatPerVec) : m_attrib(attrib),
															   m_firstUpdated(0),
															   m_lastUpdated(0),
															   m_floatPerVec(floatPerVec),
															   m_capacity(0),
															   m_uploads(0),
															   m_lastTimeMap(false),
															   m_mappedPtr(nullptr)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::update(fm::Size index,const float *value)
	{
		std::memcpy(&m_data[index*m_floatPerVec],value,sizeof(float)*m_floatPerVec);
		/*
		if (!m_mappedPtr && m_lastTimeMap)
			m_mappedPtr = (float*)m_attrib.buf->map(false,true);
		
		m_uploads++;
		
		m_lastTimeMap = m_uploads > 20;
		
		if (m_mappedPtr)
			std::memcpy(m_mappedPtr + index*m_floatPerVec,value,sizeof(float)*m_floatPerVec);
		else
			m_attrib.buf->updateData(value,sizeof(float) * m_floatPerVec,sizeof(float) * m_floatPerVec * index);
		*/
		m_firstUpdated = std::min(m_firstUpdated,index);
		m_lastUpdated  = std::max(m_lastUpdated ,index);
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::updateFrom(fm::Size index,fm::Size fromIndex)
	{
		update(index,&m_data[fromIndex*m_floatPerVec]);
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::fetch(fm::Size index,float *ret) const
	{
		std::memcpy(ret,&m_data[index*m_floatPerVec],sizeof(float)*m_floatPerVec);
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::setCapacity(fm::Size capacity)
	{
		cout << "set capacity for " << this << " to " << capacity << endl;
		
		unMap();
		m_firstUpdated = m_data.size() / m_floatPerVec - 1;
		m_lastUpdated  = 0;
		
		m_attrib.set(m_floatPerVec,
					 0,
					 capacity,
					 fg::Is_GLDataType<float>::enumVal,
					 nullptr,
					 sizeof(float) * m_floatPerVec * capacity,
					 m_attrib.bufferUsage,m_attrib.instancesPerUpdate);
		
		if (m_data.size())
			m_attrib.buf->updateData((void*)&m_data[0],m_data.size() * sizeof(float),0);
		
		m_capacity = capacity;
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::push(const float *value)
	{
		m_data.resize(m_data.size() + m_floatPerVec);
		
		if (m_data.size() > m_capacity * m_floatPerVec)
			setCapacity(std::max<fm::Size>(m_capacity*2,5));
		else
			update(m_data.size() / m_floatPerVec - 1,value);
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::pop()
	{
		m_data.resize(m_data.size() - m_floatPerVec);
		
		if (m_data.size() < m_capacity/4 * m_floatPerVec)
			setCapacity(m_capacity/2);
		
		if (m_lastUpdated * m_floatPerVec == m_data.size())
			m_lastUpdated--;
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::unMap()
	{
		if (m_mappedPtr)
		{
			m_mappedPtr = nullptr;
			m_attrib.buf->unMap();
		}
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::prepareDraw()
	{
		unMap();
		
		if (m_firstUpdated <= m_lastUpdated)
		{
			fm::Size elemCount = m_lastUpdated - m_firstUpdated + 1;
			
			m_attrib.buf->updateData((void*)&m_data[m_firstUpdated*m_floatPerVec],
									 sizeof(float) * m_floatPerVec * elemCount,
									 sizeof(float) * m_floatPerVec * m_firstUpdated);
		}
		
		m_firstUpdated = m_data.size() / m_floatPerVec - 1;
		m_lastUpdated  = 0;
		
		m_uploads = 0;
	}
	
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to manage sprites
	/// 
	/////////////////////////////////////////////////////////////
	class SpriteManager : public fg::Drawable
	{
		struct SpriteData
		{
			Sprite *ptr;
			ImageID imgID;
		};
		
		
		std::vector<SpriteData> m_spriteDatas;
		TextureAtlas<ImageID> m_atlas;
		fg::DrawData m_drawData;
		
		FloatAttributeUpdater m_posProp;
		FloatAttributeUpdater m_dirProp;
		FloatAttributeUpdater m_sizProp;
		FloatAttributeUpdater m_uvpProp;
		FloatAttributeUpdater m_uvsProp;
		
		FloatAttributeUpdater m_vertPtsProp;
		FloatAttributeUpdater m_vertUVsProp;
		
		ShaderManager m_instancingShader;
		
		bool useInstancing() const {return true;}
		
		/////////////////////////////////////////////////////////////
		/// @brief Build the vertices of a quad for sprite
		/// 
		/// @param pts The points returned
		/// @param uvs The texture positions returned
		/// @param size The size of the quad
		/// @param shape The glyph for the sprite
		/// @param dir The direction of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		void buildVertices(std::array<fm::vec3,6> &pts,
						   std::array<fm::vec2,6> &uvs,
						   fm::vec3 pos,
						   fm::vec2 size,
						   const Glyph &shape,
						   fm::vec2 dir);
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Handle creating s sprite
		/// 
		/// @param sprite An invalid sprite 
		/// @param imgID The imageId to use
		/// @param pos The position of the new sprite
		/// @param size The size of the new sprite
		/// @param dir The direction of the new sprite
		/// 
		/////////////////////////////////////////////////////////////
		void handleCreate(Sprite &sprite,ImageID imgIDconst,const fm::vec3 &pos,const fm::vec2 &size,const fm::vec2 &dir);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle destroying a sprite
		/// 
		/// @param sprite A valid sprite 
		/// 
		/////////////////////////////////////////////////////////////
		void handleDestroy(Sprite &sprite);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle emplacing a sprite
		/// 
		/// @param sprite A (moved) valid sprite 
		/// 
		/////////////////////////////////////////////////////////////
		void handleEmplace(Sprite &sprite);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle changing the position of a sprite
		/// 
		/// @param sprite A valid sprite 
		/// @param pos The new position of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		void handlePosChange(Sprite &sprite,const fm::vec3 &pos);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle changing the direction of a sprite
		/// 
		/// @param sprite A valid sprite 
		/// @param dir The new direction of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		void handleDirChange(Sprite &sprite,const fm::vec2 &dir);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle changing the size of a sprite
		/// 
		/// @param sprite A valid sprite 
		/// @param size The new size of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		void handleSizeChange(Sprite &sprite,const fm::vec2 &size);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle changing the image id of a sprite
		/// 
		/// @param sprite A valid sprite 
		/// @param imgID The new image id of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		void handleImageIDChange(Sprite &sprite,ImageID imgID);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current position of a sprite
		/// 
		/// @param sprite A valid sprite 
		/// 
		/// @return The current position of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec3 fetchPos(const Sprite &sprite) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current direction of a sprite
		/// 
		/// @param sprite A valid sprite 
		/// 
		/// @return The current direction of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 fetchDir(const Sprite &sprite) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current size of a sprite
		/// 
		/// @param sprite A valid sprite 
		/// 
		/// @return The current size of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 fetchSize(const Sprite &sprite) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current image id of a sprite
		/// 
		/// @param sprite A valid sprite 
		/// 
		/// @return The current image id of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		ImageID fetchImgID(const Sprite &sprite) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Register an image with given id for use
		/// 
		/// @param img The image to register
		/// @param id The id to associate with
		/// @param frameSize The frame size for the image
		/// 
		/////////////////////////////////////////////////////////////
		void addImage(const Image &img,ImageID id,const fm::vec2 &frameSize = fm::vec2());
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the size of a registered image
		/// 
		/// @param id The id of the image to query
		/// 
		/// @return The size of the image
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getImageSize(ImageID id) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		SpriteManager();
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		~SpriteManager();
		
		/////////////////////////////////////////////////////////////
		/// @brief Update the drawcall
		/// 
		/////////////////////////////////////////////////////////////
		void updateDrawCall();
		
		/////////////////////////////////////////////////////////////
		/// @brief Create a sprite with given image id
		/// 
		/// @param id The id to use
		/// 
		/// @return The sprite
		/// 
		/////////////////////////////////////////////////////////////
		Sprite getSprite(ImageID id);
		
		/////////////////////////////////////////////////////////////
		/// @brief Fetch an existing sprite with given id
		/// 
		/// @param id The id search
		/// 
		/// @return The sprite
		/// 
		/////////////////////////////////////////////////////////////
		Sprite &getSpriteById(fm::Size id);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of existing sprites
		/// 
		/// @return The number of sprites
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getSpriteCount() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle being updated
		/// 
		/////////////////////////////////////////////////////////////
		void onUpdate(const fm::Time & /* dt */ ) override {};
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw the sprites of the manager
		/// 
		/// @param shader The shader to draw with
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onDraw(ShaderManager &shader) override;
		
		void listSprites()
		{
			for (auto &a : m_spriteDatas)
				cout << a.ptr 
					 << " - " 
					 << a.ptr->getId() 
					 << " at (" << a.ptr->getPosition() << ") "
					 << " s (" << a.ptr->getSize() << ") "
					 << endl;
			endl(cout);
			endl(cout);
			endl(cout);
		}
	};
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::buildVertices(std::array<fm::vec3,6> &pts,
									  std::array<fm::vec2,6> &uvs,
									  fm::vec3 pos,
									  fm::vec2 size,
									  const Glyph &shape,
									  fm::vec2 dir)
	{
		fm::vec2 rightv = -dir.perp();
		
		const fm::vec2 tpt[] = {fm::vec2(0,0),fm::vec2(1,0),fm::vec2(1,1),fm::vec2(0,0),fm::vec2(1,1),fm::vec2(0,1)};
		
		Cx(6)
		{
			fm::vec2 locp = tpt[x] * size - size / 2;
			pts[x] = pos + vec3(locp.x * rightv + locp.y * dir,0);
			uvs[x] = tpt[x] * shape.size + shape.pos;
		}
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleCreate(Sprite &sprite,ImageID imgID,const fm::vec3 &pos,const fm::vec2 &size,const fm::vec2 &dir)
	{
		fm::Size index = m_spriteDatas.size();
		
		sprite.setId(index);
		sprite.setManager(this);
		
		m_spriteDatas.push_back(SpriteData{&sprite,imgID});
		
		fg::Glyph shape = m_atlas.fetch(imgID);
		fm::vec2 uvsPos = shape.pos;
		fm::vec2 uvsSiz = shape.size;
		
		if (useInstancing())
		{
			m_posProp.push(&pos.x);
			m_dirProp.push(&dir.x);
			m_sizProp.push(&size.x);
			m_uvpProp.push(&uvsPos.x);
			m_uvsProp.push(&uvsSiz.x);
		}
		else
		{
			std::array<fm::vec3,6> pts;
			std::array<fm::vec2,6> uvs;
			
			buildVertices(pts,uvs,pos,size,shape,dir);
			
			m_vertPtsProp.push(&pts[0].x);
			m_vertUVsProp.push(&uvs[0].x);
		}
		
		updateDrawCall();
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleDestroy(Sprite &sprite)
	{
		Sprite &lastSprite = *m_spriteDatas.back().ptr;
		
		fm::Size index = sprite.getId();
		fm::Size oldIndex = lastSprite.getId();
		
		if (oldIndex != index)
		{
			lastSprite.setId(index);
			m_spriteDatas[index] = m_spriteDatas[oldIndex];
			
			if (useInstancing())
			{
				m_posProp.updateFrom(index,oldIndex);
				m_dirProp.updateFrom(index,oldIndex);
				m_sizProp.updateFrom(index,oldIndex);
				m_uvpProp.updateFrom(index,oldIndex);
				m_uvsProp.updateFrom(index,oldIndex);
			}
			else
			{
				m_vertPtsProp.updateFrom(index,oldIndex);
				m_vertUVsProp.updateFrom(index,oldIndex);
			}
		}
		
		m_spriteDatas.pop_back();
		
		if (useInstancing())
		{
			m_posProp.pop();
			m_dirProp.pop();
			m_sizProp.pop();
			m_uvpProp.pop();
			m_uvsProp.pop();
		}
		else
		{
			m_vertPtsProp.pop();
			m_vertUVsProp.pop();
		}
		
		
		updateDrawCall();
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::updateDrawCall()
	{
		if (useInstancing())
		{
			m_drawData.getDraw(0).instances = m_spriteDatas.size();
		}
		else
		{
			m_drawData.getDraw(0).set(0,m_spriteDatas.size() * 6,fg::Triangles);
		}
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleEmplace(Sprite &sprite)
	{
		m_spriteDatas[sprite.getId()].ptr = &sprite;
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::handlePosChange(Sprite &sprite,const fm::vec3 &pos)
	{
		if (useInstancing())
		{
			m_posProp.update(sprite.getId(),&pos.x);
		}
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleDirChange(Sprite &sprite,const fm::vec2 &dir)
	{
		if (useInstancing())
		{
			m_dirProp.update(sprite.getId(),&dir.x);
		}
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleSizeChange(Sprite &sprite,const fm::vec2 &size)
	{
		if (useInstancing())
		{
			m_sizProp.update(sprite.getId(),&size.x);
		}
	} 
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleImageIDChange(Sprite &sprite,ImageID imgID)
	{
		m_spriteDatas[sprite.getId()].imgID = imgID;
		
		if (useInstancing())
		{
			Glyph shape = m_atlas.fetch(imgID);
			fm::vec2 uvsPos = shape.pos;
			fm::vec2 uvsSiz = shape.size;
			
			m_uvpProp.update(sprite.getId(),&uvsPos.x);
			m_uvsProp.update(sprite.getId(),&uvsSiz.x);
		}
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec3 SpriteManager::fetchPos(const Sprite &sprite) const
	{
		// if (useInstancing())
		{
			fm::vec3 ret;
			m_posProp.fetch(sprite.getId(),&ret.x);
			return ret;
		}
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 SpriteManager::fetchDir(const Sprite &sprite) const
	{
		// if (useInstancing())
		{
			fm::vec2 ret;
			m_dirProp.fetch(sprite.getId(),&ret.x);
			return ret;
		}
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 SpriteManager::fetchSize(const Sprite &sprite) const
	{
		// if (useInstancing())
		{
			fm::vec2 ret;
			m_sizProp.fetch(sprite.getId(),&ret.x);
			return ret;
		}
	}
	
	/////////////////////////////////////////////////////////////
	ImageID SpriteManager::fetchImgID(const Sprite &sprite) const
	{
		return m_spriteDatas[sprite.getId()].imgID;
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::addImage(const Image &img,ImageID id,const fm::vec2 &frameSize)
	{
		m_atlas.upload(img,id,frameSize);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 SpriteManager::getImageSize(ImageID id) const
	{
		return m_atlas.fetch(id).size;
	}
	
	/////////////////////////////////////////////////////////////
	SpriteManager::~SpriteManager()
	{
		for (auto &sprDat : m_spriteDatas)
			sprDat.ptr->setManager(nullptr);
	}


	/////////////////////////////////////////////////////////////
	SpriteManager::SpriteManager() : m_posProp(m_drawData[Assoc::Custom+0],3),
									 m_dirProp(m_drawData[Assoc::Custom+1],2),
									 m_sizProp(m_drawData[Assoc::Custom+2],2),
									 m_uvpProp(m_drawData[Assoc::Custom+3],2),
									 m_uvsProp(m_drawData[Assoc::Custom+4],2),
									 m_vertPtsProp(m_drawData[Assoc::Position],18),
									 m_vertUVsProp(m_drawData[Assoc::TextureUV],12)
	{
		if (useInstancing())
		{
			m_drawData.addDraw(0,4,fg::TriangleFan);
			
			m_drawData.positions = {fm::vec2(0,0),fm::vec2(0,1),fm::vec2(1,1),fm::vec2(1,0)};
			m_drawData[Assoc::Custom+0].instancesPerUpdate = 1;
			m_drawData[Assoc::Custom+1].instancesPerUpdate = 1;
			m_drawData[Assoc::Custom+2].instancesPerUpdate = 1;
			m_drawData[Assoc::Custom+3].instancesPerUpdate = 1;
			m_drawData[Assoc::Custom+4].instancesPerUpdate = 1;
			
			
			fm::Result res = m_instancingShader.loadFromMemory(
"#version 110\n"
"\n"
"#define FRONTIER_MODEL\n"
"#define FRONTIER_VIEW\n"
"#define FRONTIER_PROJ\n"
"#define FRONTIER_TEXMAT\n"
"#define FRONTIER_POS\n"
"#define FRONTIER_CUSTOM_0\n"
"#define FRONTIER_CUSTOM_1\n"
"#define FRONTIER_CUSTOM_2\n"
"#define FRONTIER_CUSTOM_3\n"
"#define FRONTIER_CUSTOM_4\n"
"\n"
"uniform mat4 FRONTIER_TEXMAT u_texMat;\n"
"uniform mat4 FRONTIER_MODEL  u_modelMat;\n"
"uniform mat4 FRONTIER_VIEW   u_viewMat;\n"
"uniform mat4 FRONTIER_PROJ   u_projMat;\n"
"\n"
"attribute vec2 FRONTIER_POS      in_tpt;\n"
"attribute vec3 FRONTIER_CUSTOM_0 in_pos;\n"
"attribute vec2 FRONTIER_CUSTOM_1 in_dir;\n"
"attribute vec2 FRONTIER_CUSTOM_2 in_siz;\n"
"attribute vec2 FRONTIER_CUSTOM_3 in_uvp;\n"
"attribute vec2 FRONTIER_CUSTOM_4 in_uvs;\n"
"\n"
"varying vec2 va_texp;\n"
"\n"
"void main()\n"
"{\n"
"	vec2 locp = in_siz * (in_tpt - vec2(.5,.5));\n"
"	\n"
"	vec2 r = vec2(in_dir.y,-in_dir.x);\n"
"	\n"
"	gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_pos.xy + locp.x * r + locp.y * in_dir,in_pos.z,1.0);\n"
"	\n"
"	va_texp = vec2(u_texMat * vec4(in_uvp + in_tpt * in_uvs,0.0,1.0));\n"
"}\n",
"#version 110\n"
"\n"
"uniform sampler2D u_tex;\n"
"\n"
"varying vec2 va_texp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = texture2D(u_tex,va_texp);\n"
"}\n");
			m_instancingShader.regTexture("u_tex");
			
			if (!res)
				std::cout << res << std::endl;
		} 
		else
		{
			m_drawData.addDraw(0,0,fg::Triangles);
		}
	}

	/////////////////////////////////////////////////////////////
	Sprite SpriteManager::getSprite(ImageID id)
	{
		return Sprite(*this,id);
	}
	
	/////////////////////////////////////////////////////////////
	Sprite &SpriteManager::getSpriteById(fm::Size id)
	{
		return *m_spriteDatas[id].ptr;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size SpriteManager::getSpriteCount() const
	{
		return m_spriteDatas.size();
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::onDraw(ShaderManager &shader)
	{
		if (useInstancing())
		{
			m_posProp.prepareDraw();
			m_dirProp.prepareDraw();
			m_sizProp.prepareDraw();
			m_uvpProp.prepareDraw();
			m_uvsProp.prepareDraw();
			
			
			m_instancingShader.getModelStack().top(shader.getModelStack().top());
			m_instancingShader.getViewStack().top(shader.getViewStack().top());
			m_instancingShader.getProjStack().top(shader.getProjStack().top());
			
			m_instancingShader.getTexUVStack().push().mul(m_atlas.getTexture().getPixToUnitMatrix());
			
			m_instancingShader.useTexture(m_atlas.getTexture());
			m_instancingShader.draw(m_drawData);
			
			m_instancingShader.getTexUVStack().pop();
		}
	}
	
	
	
	
	
	/////////////////////////////////////////////////////////////
	Sprite::~Sprite()
	{
		if (m_manager)
			m_manager->handleDestroy(*this);
	}
	
	/////////////////////////////////////////////////////////////
	Sprite::Sprite(SpriteManager &manager,ImageID imgID) : m_manager(&manager)
	{
		m_manager->handleCreate(*this,imgID,fm::vec3(),m_manager->getImageSize(imgID),fm::vec2(0,1));
	}
	
	/////////////////////////////////////////////////////////////
	Sprite::Sprite(const Sprite &sprite) : m_manager(sprite.m_manager)
	{
		m_manager->handleCreate(*this,sprite.getImageID(),
								sprite.getPosition(),
								sprite.getSize(),
								sprite.getDirection());
	}
	
	/////////////////////////////////////////////////////////////
	Sprite::Sprite(Sprite &&sprite)
	{
		this->operator=((Sprite&&)sprite);
	}
	
	/////////////////////////////////////////////////////////////
	Sprite &Sprite::operator=(const Sprite &sprite)
	{
		m_manager->handleDestroy(*this);
		
		m_manager = sprite.m_manager;
		m_manager->handleCreate(*this,sprite.getImageID(),
								sprite.getPosition(),
								sprite.getSize(),
								sprite.getDirection());
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Sprite &Sprite::operator=(Sprite &&sprite)
	{
		m_manager = sprite.m_manager;
		m_id      = sprite.m_id;
		
		sprite.m_manager = nullptr;
		
		m_manager->handleEmplace(*this);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Sprite &Sprite::swap(Sprite &sprite)
	{
		if (m_manager == sprite.m_manager)
		{
			std::swap(m_id,sprite.m_id);
			
			m_manager->handleEmplace(*this);
			m_manager->handleEmplace(sprite);
		}
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Sprite &Sprite::setPosition(const fm::vec3 &pos)
	{
		m_manager->handlePosChange(*this,pos);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec3 Sprite::getPosition() const
	{
		return m_manager->fetchPos(*this);
	}

	/////////////////////////////////////////////////////////////
	Sprite &Sprite::setDirection(const fm::vec2 &dir)
	{
		m_manager->handleDirChange(*this,dir);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 Sprite::getDirection() const
	{
		return m_manager->fetchDir(*this);
	}

	/////////////////////////////////////////////////////////////
	Sprite &Sprite::setSize(const fm::vec2 &size)
	{
		m_manager->handleSizeChange(*this,size);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 Sprite::getSize() const
	{
		return m_manager->fetchSize(*this);
	}
	
	/////////////////////////////////////////////////////////////
	Sprite &Sprite::setImageID(ImageID imgID)
	{
		m_manager->handleImageIDChange(*this,imgID);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	ImageID Sprite::getImageID() const
	{
		return m_manager->fetchImgID(*this);
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size Sprite::getId() const
	{
		return m_id;
	}
	
	/////////////////////////////////////////////////////////////
	void Sprite::setId(fm::Size index)
	{
		m_id = index;
	}
	
	/////////////////////////////////////////////////////////////
	void Sprite::setManager(SpriteManager *manager)
	{
		m_manager = manager;
	}
}


#define NAMESPACE_TO_TEST finalVersion

class SpriteTester : public Drawable
{
	std::vector<NAMESPACE_TO_TEST::Sprite> m_sprites;
	NAMESPACE_TO_TEST::SpriteManager *m_manager;
	fm::Size m_usedSprites;
	bool m_recreateSprites;
	float m_moveSprites;
	fm::Time m_elapsed;
	Image m_snowImg;
	vec2u m_winSize;
	
public:
	/////////////////////////////////////////////////////////////
	SpriteTester(NAMESPACE_TO_TEST::SpriteManager *newManager,
				 fm::vec2 winSize) : m_manager(nullptr),
									 m_usedSprites(10),
									 m_recreateSprites(false),
									 m_moveSprites(0),
									 m_winSize(winSize)
	{
		Result res = m_snowImg.loadFromFile("sheet.png");
		
		if (!res) cout << res << endl;
		
		setManager(newManager);
	}
	
	/////////////////////////////////////////////////////////////
	void setMovingSprites(float moving)
	{
		m_moveSprites = moving;
	}
	
	/////////////////////////////////////////////////////////////
	void removeSpriteAt(fm::Size index)
	{
		if (index+1 != m_sprites.size())
		{
			m_sprites[index].swap(m_sprites.back());
		}
		
		m_sprites.pop_back();
	}
	
	/////////////////////////////////////////////////////////////
	~SpriteTester()
	{
		delete m_manager;
	}
	
	/////////////////////////////////////////////////////////////
	void setManager(NAMESPACE_TO_TEST::SpriteManager *newManager)
	{
		delete m_manager;
		m_manager = newManager;
		m_sprites = std::vector<NAMESPACE_TO_TEST::Sprite>();
		
		C(6)
			m_manager->addImage(m_snowImg.getSubImage(vec2(i*100,0),vec2(100,100)),i);
	}

	/////////////////////////////////////////////////////////////
	void onDraw(ShaderManager &shader)
	{
		m_manager->onDraw(shader);
	}
	
	/////////////////////////////////////////////////////////////
	void setSpriteCount(fm::Size num)
	{
		m_usedSprites = num;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size getSpriteCount() const
	{
		return m_sprites.size();
	}
	
	/////////////////////////////////////////////////////////////
	void randSizePos(vec2 &s,vec2 &p) 
	{	
		s = vec2(5 + rand()%50);
		p = vec2(rand()%int(m_winSize.w - s.w),rand()%int(m_winSize.h - s.h));
	};
	
	/////////////////////////////////////////////////////////////
	void addSprite()
	{
		vec2 s,p;
		randSizePos(s,p);
		
		NAMESPACE_TO_TEST::Sprite spr = m_manager->getSprite(rand()%6);
		
		spr.setPosition(p).setSize(s);
		
		m_sprites.emplace_back(spr);
	}
	
	/////////////////////////////////////////////////////////////
	void onUpdate(const fm::Time &dt)
	{
		m_elapsed += dt;
		
		float dif = m_usedSprites - float(m_sprites.size());
		
		while (m_sprites.size() < m_usedSprites && m_elapsed.asSecs() > 1 / dif)
		{
			m_elapsed -= seconds(1 / dif);
			
			addSprite();
		}
		
		for (fm::Size index=0;index<m_sprites.size()*m_moveSprites;++index)
		{
			NAMESPACE_TO_TEST::Sprite &spr = m_sprites[index];
			vec2 siz = spr.getSize();
			
			if (siz.area() < 10)
			{
				if (m_sprites.size() > m_usedSprites)
				{
					removeSpriteAt(index);
					--index;
					continue;
				}
				else
				{ 
					vec2 s,p;
					randSizePos(s,p);
					
					spr.setPosition(p).setSize(s);						
				}
			}
			else
			{
				spr.setSize(siz * 0.992);
				
				vec2 p = spr.getPosition();
				vec2 toOrigo = m_winSize/2 - p;
				vec2 perp = toOrigo.perp().sgn();
				
				spr.setPosition(vec2(spr.getPosition()) + toOrigo / toOrigo.LENGTH() * 8 + perp * 4);					
				spr.setDirection((m_winSize/2 - vec2(spr.getPosition())).sgn());
			}
		}
	}
};


#include <FRONTIER/OpenGL.hpp>

int main()
{
	srand(time(0));
	
	GuiWindow win(vec2(640,480),"sprite");
	
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_VENDOR) << endl;
	cout << Buffer::keepShadowCopy() << endl;
	cout << win.getShader().instancingAvailable() << endl;
	
	finalVersion::SpriteManager *manager = new finalVersion::SpriteManager();
	SpriteTester tester(manager,win.getSize());
	
	GuiText *scText = new GuiText(win,"10");
	scText->setPosition(vec2(100,0));
	
	GuiText *spText = new GuiText(win,"0");
	spText->setPosition(vec2(150,0));
	
	fm::Size spriteCount = 10;
	
	ScrollBar *sb1 = new ScrollBar(win,vec2(100,20),[&](GuiScrollBar &sb){
		
		fm::Size p = 5;
		
		spriteCount = pow(10,sb.getScrollState()*(p-1) + 1);
		scText->setText(fm::toString(spriteCount));
		tester.setSpriteCount(spriteCount);
	});
	
	GuiText *mvText = new GuiText(win,"0%");
	
	ScrollBar *pb1 = new ScrollBar(win,vec2(100,20),[&](GuiScrollBar &sb){
		mvText->setText(fm::toString(int(sb.getScrollState()*100)) + "%");
		tester.setMovingSprites(sb.getScrollState());
	});
	
	pb1->setPosition(sb1->getPosition() + sb1->getSize() * vec2(0,1));
	mvText->setPosition(pb1->getPosition() + pb1->getSize() * vec2(1.5,0));
	
	int usedManagerId = 0;
	
	GuiText *mdText = new GuiText(win,"Manager: Unique");
	
	PushButton *pb2 = new PushButton(win,"SwitchMode",[&](){
		++usedManagerId;
		usedManagerId %= 3;
		/*
		if (usedManagerId == 0) tester.setManager(new test::UniqueSpriteManager), mdText->setText("Manager: Unique");
		if (usedManagerId == 1) tester.setManager(new test::PerOneSpriteManager), mdText->setText("Manager: PerOne update");
		if (usedManagerId == 2) tester.setManager(new test::MapBufSpriteManager), mdText->setText("Manager: Map buffer ❤");
		*/
	});
	
	pb2->setPosition(pb1->getPosition() + pb1->getSize() * vec2(0,1));
	mdText->setPosition(pb2->getPosition() + pb1->getSize() * vec2(1.5,0));
	
	win.getMainLayout().addChildElement(sb1);
	win.getMainLayout().addChildElement(scText);
	win.getMainLayout().addChildElement(spText);
	win.getMainLayout().addChildElement(pb1);
	win.getMainLayout().addChildElement(mvText);
	win.getMainLayout().addChildElement(pb2);
	win.getMainLayout().addChildElement(mdText);
	
	Clock fpsDispClk;
	bool running = true;
	for (fm::Size frameCounter=0;running;++frameCounter)
	{
		if (fpsDispClk.getSeconds() > 1)
		{
			fpsDispClk.setTime(fpsDispClk.getSeconds() - 1);
			win.setTitle("sprite " + fm::toString(frameCounter));
			frameCounter = 0;
		}
		spText->setText(fm::toString(tester.getSpriteCount()));
		 
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::L)
				{
					manager->listSprites();
				}
			}
		}
		
		tester.onUpdate(win.getUpdateInterval());
		
		
		win.clear();
		
		win.draw(tester);
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}
