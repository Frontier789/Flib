#include <FRONTIER/Gui/ElementBounds.hpp>

namespace fgui {
	ElementBounds::ElementBounds(GuiContext &cont,GuiElement *target,fm::vec4 col) :
		GuiElement(cont),
		m_col(col),
		m_enabled(true),
		m_target(target)
	{
		m_dd.positions = {fm::vec2(0,0),fm::vec2(1,0),fm::vec2(1,1),fm::vec2(0,1)};
		m_dd.colors = {col,col,col,col};
		
		m_dd.addDraw(fg::LineLoop);
	}
	
	void ElementBounds::setColor(fm::vec4 col)
	{
		m_col = col;
		
		m_dd.colors = {col,col,col,col};
	}
	
	fm::vec4 ElementBounds::getColor() const {return m_col;}
	
	void ElementBounds::enable(bool enabled)
	{
		m_enabled = enabled;
	}
	
	bool ElementBounds::enabled() const {return m_enabled;}
	
	void ElementBounds::onDraw(fg::ShaderManager &shader)
	{
		if (!m_enabled) return;
		
		shader.useTexture(nullptr);
		
		auto root = (m_target ? m_target : (GuiElement*)&getOwnerContext().getMainLayout());
		auto &stck = shader.getModelStack();
		
		stck.push();
		root->traverseHierarchy([&](GuiElement &e) {
			fm::vec2 p = e.getPosition();
			fm::vec2 s = e.getSize();
			
			stck.mul(fm::MATRIX::translation(p) * fm::MATRIX::scaling(s));
			
			shader.draw(m_dd);
			
			stck.pop();
		});
	}
}