#include <FRONTIER/Graphics/FloatAttributeUpdater.hpp>
#include <FRONTIER/Graphics/Attribute.hpp>
#include <cstring>

namespace fg
{
	/////////////////////////////////////////////////////////////
	fm::Size FloatAttributeUpdater::getFloatsPerItem() const
	{
		return m_floatPerVec * m_vecsPerItem;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size FloatAttributeUpdater::getBytesPerItem() const
	{
		return getFloatsPerItem()*sizeof(float);
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size FloatAttributeUpdater::getCapacity() const
	{
		return m_capacity;
	}
	
	/////////////////////////////////////////////////////////////
	FloatAttributeUpdater::FloatAttributeUpdater(fg::Attribute &attrib,
												 fm::Size floatPerVec,
												 fm::Size vecsPerItem) : m_attrib(attrib),
																		 m_firstUpdated(0),
																		 m_lastUpdated(0),
																		 m_floatPerVec(floatPerVec),
																		 m_vecsPerItem(vecsPerItem),
																		 m_capacity(0),
																		 m_uploads(0),
																		 m_mappedPtr(nullptr),
																		 m_autoShrink(true)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::update(fm::Size index,const float *value)
	{
		update(index,value,1);
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::update(fm::Size index,const float *value,fm::Size amount)
	{
		for (fm::Size i=0;i<amount;++i)
			std::memcpy(&m_data[(index+i)*getFloatsPerItem()],value,getBytesPerItem());
		
		m_uploads+=amount;
		
		if (m_uploads == amount)
		{
			m_firstUpdated = index;
			m_lastUpdated  = index+amount-1;
		}
		else
		{
			m_firstUpdated = std::min(m_firstUpdated,index);
			m_lastUpdated  = std::max(m_lastUpdated ,index+amount-1);
		}
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::updateFrom(fm::Size index,fm::Size fromIndex)
	{
		update(index,&m_data[fromIndex*getFloatsPerItem()]);
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::fetch(fm::Size index,float *ret) const
	{
		std::memcpy(ret,&m_data[index*getFloatsPerItem()],getBytesPerItem());
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::setCapacity(fm::Size capacity)
	{
		unMap();
		m_uploads = 0;
		
		m_attrib.set(m_floatPerVec,
					 0,
					 capacity,
					 fg::Is_GLDataType<float>::enumVal,
					 nullptr,
					 getBytesPerItem() * capacity,
					 m_attrib.bufferUsage,m_attrib.instancesPerUpdate);
		
		if (m_data.size())
			m_attrib.buf->updateData((void*)&m_data[0],m_data.size() * sizeof(float),0);
		
		m_capacity = capacity;
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::push(const float *value)
	{
		push(value,1);
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::push(const float *value,fm::Size amount)
	{
		m_data.resize(m_data.size() + getFloatsPerItem()*amount);
		
		if (m_data.size() > m_capacity * getFloatsPerItem())
		{
			for (fm::Size i=0;i<amount;++i)
				std::memcpy(&m_data[m_data.size() - getFloatsPerItem()*(amount-i)],value,getBytesPerItem());
			
			fm::Size newCapactiy = 5;
			newCapactiy = std::max<fm::Size>(m_capacity*2,newCapactiy);
			newCapactiy = std::max<fm::Size>(m_capacity+amount,newCapactiy);
			
			setCapacity(newCapactiy);
		}
		else
			update(m_data.size() / getFloatsPerItem() - amount,value,amount);
	}
	
	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::pop()
	{
		m_data.resize(m_data.size() - getFloatsPerItem());
		
		if (m_data.size() < m_capacity/4 * getFloatsPerItem() && m_autoShrink)
			setCapacity(m_capacity/2);
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
		
		if (m_uploads > 0 && m_data.size())
		{
			m_firstUpdated = std::min(m_firstUpdated,m_data.size() / getFloatsPerItem() - 1);
			m_lastUpdated  = std::min(m_lastUpdated ,m_data.size() / getFloatsPerItem() - 1);
			
			fm::Size elemCount = m_lastUpdated - m_firstUpdated + 1;
/*
			float ratio = elemCount / float(m_data.size() / getFloatsPerItem());
			if (ratio > 0.3)
			{
				m_attrib.buf->setData(nullptr,m_data.size() * sizeof(m_data[0]));
				m_attrib.buf->updateData((void*)&m_data[0],m_data.size() * sizeof(m_data[0]));
			}
			else*/
			{
				m_attrib.buf->updateData((void*)&m_data[m_firstUpdated*getFloatsPerItem()],
										getBytesPerItem() * elemCount,
										getBytesPerItem() * m_firstUpdated);
			}
			m_uploads = 0;
		}
	}

	/////////////////////////////////////////////////////////////
	void FloatAttributeUpdater::setAutoShrink(bool automatic)
	{
		m_autoShrink = automatic;
	}
	
	/////////////////////////////////////////////////////////////
	bool FloatAttributeUpdater::getAutoShrink() const
	{
		return m_autoShrink;
	}
}

