#include <FRONTIER/Graphics/GlObject.hpp>
namespace fg
{
	//////////////////////////////////
	GlObject::GlObject() : m_id(0u)
	{
		
	}
	
	//////////////////////////////////
	GlObject::~GlObject()
	{
		
	}

	//////////////////////////////////
	unsigned int &GlObject::getGlId()
	{
		return m_id;
	}
	
	//////////////////////////////////
	const unsigned int &GlObject::getGlId() const
	{
		return m_id;
	}
}