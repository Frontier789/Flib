#include <Frontier.hpp>

class ColorPicker : public GuiElement, public ClickListener, public CallbackUser<ColorPicker,Color>
{
	DrawData m_drawData;
	DrawData m_circle;
	vec2 m_lastp;
	
	Color getColorAt(vec2 p);
	
public:
	ColorPicker(GuiContext &cont);
	
	void onDraw(fg::ShaderManager &shader) override;
	void onPress(fw::Mouse::Button button,fm::vec2 p) override;
	bool contains(fm::vec2 p) const override;
	void onMouseMoved(fm::vec2 p,fm::vec2 prevP) override;
	bool onEvent(fw::Event &ev) override;
};



