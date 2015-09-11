#include <FRONTIER/System/macros/OFFSETOF.hpp>
#include <FRONTIER/Graphics/Primitive.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Vertex.hpp>
#include <FRONTIER/System/Polar2.hpp>
#include <FRONTIER/Gui/Housing.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace Fgui
{
	const std::string Housing::m_className = "Housing";

	void Housing::buildVertices()
	{
		float r = m_margin.minwh();
		float l = ((r*3.14159265358979/2.0) / 4.0);

		if (l != int(l)) l = int(l)+1;

		fm::Size cornerPts = l + 1;

		m_vertexCount = cornerPts*4;

		fm::vert2f *vrts = new fm::vert2f[m_vertexCount];

		fm::vec2 s = getSize();

		C(cornerPts)
			vrts[cornerPts*0 + i].pos = s*fm::vec2(0,0) + fm::vec2( r, r) + fm::vec2( fm::pol2(r,90.0*(i+1)/cornerPts + 180)),
			vrts[cornerPts*1 + i].pos = s*fm::vec2(1,0) + fm::vec2(-r, r) + fm::vec2( fm::pol2(r,90.0*(i+1)/cornerPts + 270)),
			vrts[cornerPts*2 + i].pos = s*fm::vec2(1,1) + fm::vec2(-r,-r) + fm::vec2( fm::pol2(r,90.0*(i+1)/cornerPts + 0)),
			vrts[cornerPts*3 + i].pos = s*fm::vec2(0,1) + fm::vec2( r,-r) + fm::vec2( fm::pol2(r,90.0*(i+1)/cornerPts + 90));

		C(m_vertexCount)
			vrts[i].clr = m_color;

		m_vbo.setData(vrts,sizeof(*vrts)*m_vertexCount);

		delete[] vrts;
	}

	void Housing::setupPosition()
	{
		m_transform = fm::MATRIX::translation(getPosInRoot());
	}

	/////////////////////////////////////////////////////////////
	Housing::Housing(const std::string &name,
					 const Anchor &anchor,
					 const fm::vec2 &size,
					 Widget *parent,
					 const fm::vec2 &margin,
					 Widget *content,
					 fm::vec4 color) : Div(name,anchor,size,parent,margin,content),
									   m_vertexCount(0),
									   m_needBuildVertices(true),
									   m_needSetupPosition(true),
									   m_color(color)
	{

	}

	/////////////////////////////////////////////////////////////
	Housing::~Housing()
	{

	}

	/////////////////////////////////////////////////////////////
	void Housing::setMinSize(const fm::vec2 &size)
	{
		m_needBuildVertices = true;
		m_needSetupPosition = true;

		Div::setMinSize(size);
	}

	/////////////////////////////////////////////////////////////
	void Housing::setMargin(const fm::vec2 &margin)
	{
		m_needBuildVertices = true;
		m_needSetupPosition = true;

		Div::setMargin(margin);
	}

	/////////////////////////////////////////////////////////////
	void Housing::setParent(Widget *parent)
	{
		m_needBuildVertices = true;
		m_needSetupPosition = true;

		Div::setParent(parent);
	}

	/////////////////////////////////////////////////////////////
	void Housing::setSize(const fm::vec2 &size)
	{
		m_needBuildVertices = true;
		m_needSetupPosition = true;

		Div::setSize(size);
	}

	/////////////////////////////////////////////////////////////
	void Housing::setColor(const fm::vec4 &color)
	{
		m_needBuildVertices = true;
		
		m_color = color;
	}

	/////////////////////////////////////////////////////////////
	fm::vec4 Housing::getColor()
	{
		return m_color;
	}

	/////////////////////////////////////////////////////////////
	void Housing::setAnchor(const Anchor &anchor)
	{
		m_needBuildVertices = true;
		m_needSetupPosition = true;

		Div::setAnchor(anchor);
	}

	/////////////////////////////////////////////////////////////
	void Housing::onDraw()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&m_transform.transpose()[0][0]);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		m_vbo.bind();
		glVertexPointer(2,GL_FLOAT,sizeof(fm::vert2f),(void*)fm::offsetOf(fm::vert2f,pos));
		glColorPointer(4,GL_FLOAT,sizeof(fm::vert2f),(void*)fm::offsetOf(fm::vert2f,clr));

		glDrawArrays(fg::LineLoop,0,m_vertexCount);

		fg::Buffer::bind(fm::nullPtr,fg::ArrayBuffer);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glDisable(GL_BLEND);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&fm::mat4()[0][0]);

		Div::onDraw();
	}

	/////////////////////////////////////////////////////////////
	void Housing::onUpdate()
	{
		if (m_needBuildVertices)
			m_needBuildVertices = false,
			buildVertices();

		if (m_needSetupPosition)
			m_needSetupPosition = false,
			setupPosition();

		Div::onUpdate();
	}

	/////////////////////////////////////////////////////////////
	void Housing::onParentChange()
	{
		m_needBuildVertices = true;
		m_needSetupPosition = true;

		Div::onParentChange();
	}

	/////////////////////////////////////////////////////////////
	void Housing::setContent(Widget *content)
	{
		m_needBuildVertices = true;
		m_needSetupPosition = true;

		Div::setContent(content);
	}

	/////////////////////////////////////////////////////////////
	const std::string &Housing::getRealType() const
	{
		return m_className;
	}
}
