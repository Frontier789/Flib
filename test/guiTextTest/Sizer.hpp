class Sizer : public GuiElement, public ClickListener
{
public:
	DrawData ddRect;
	GuiElement &elem;
	int sideGrab;
	
	Sizer(GuiElement &elem) : GuiElement(elem.getOwnerContext()), elem(elem), sideGrab(-1)
	{
		setPosition(elem.getPosition());
		setSize(elem.getSize());
		
		vec2 pts[] = {vec2(0,0),vec2(1,0),vec2(1,0),vec2(1,1),vec2(1,1),vec2(0,1),vec2(0,1),vec2(0,0)};
		vec4 clr[] = {vec4::Red,vec4::Red,vec4::Red,vec4::Red,vec4::Red,vec4::Red,vec4::Red,vec4::Red};
		
		ddRect.positions = pts;
		ddRect.colors = clr;
		ddRect.addDraw(0,8,fg::Lines);
	}
	
	/////////////////////////////////////////////////////////////
	void onDraw(fg::ShaderManager &shader) override
	{
		elem.onDraw(shader);
		shader.getModelStack().push().mul(MATRIX::translation(getPosition()) * MATRIX::scaling(vec2(getSize())));
		shader.useTexture(nullptr);
		shader.draw(ddRect);
		shader.getModelStack().pop();
	}
	
	/////////////////////////////////////////////////////////////
	void onUpdate(const fm::Time &dt) override
	{
		elem.onUpdate(dt);
	}
	
	/////////////////////////////////////////////////////////////
	void onEvent(fw::Event &ev) override
	{
		elem.onEvent(ev);
	}
	
	/////////////////////////////////////////////////////////////
	void setSize(fm::vec2s size) override
	{
		elem.setSize(size);
		GuiElement::setSize(size);
	}
	
	/////////////////////////////////////////////////////////////
	void setPosition(fm::vec2i pos) override
	{
		elem.setPosition(pos);
		GuiElement::setPosition(pos);
	}
	
	constexpr static const float grabW = 7;
	
	/////////////////////////////////////////////////////////////
	bool contains(fm::vec2 p) const override
	{
		float w = grabW;
		
		bool inOuter = fm::rect2f(getPosition() - vec2(w,w),getSize() + vec2(w,w)*2).contains(p);
		bool inInner = fm::rect2f(getPosition() + vec2(w,w),getSize() - vec2(w,w)*2).contains(p);
		
		return inOuter && !inInner;
	}
	
	/////////////////////////////////////////////////////////////
	bool downSideContains(vec2 p)
	{
		vec2 pos = getPosition();
		vec2 siz = getSize();
		return rect2f(pos + vec2(-grabW,siz.h - grabW),vec2(siz.w+grabW*2,grabW*2)).contains(p);
	}

	/////////////////////////////////////////////////////////////
	bool rightSideContains(vec2 p)
	{
		vec2 pos = getPosition();
		vec2 siz = getSize();
		return rect2f(pos + vec2(siz.w - grabW,-grabW),vec2(grabW*2,siz.h+grabW*2)).contains(p);
	}

	/////////////////////////////////////////////////////////////
	void onPress(fw::Mouse::Button button,fm::vec2 p) override
	{
		if (button == fw::Mouse::Left)
		{
			bool dIn = downSideContains(p);
			bool rIn = rightSideContains(p);
			
			if (dIn && !rIn)
			{
				sideGrab = 0;
				cout << "down side grab" << endl;
				
				ddRect.colors = {vec4::Red,vec4::Red,vec4::Red,vec4::Red,vec4::Lime,vec4::Lime,vec4::Red,vec4::Red};
			}
			
			if (!dIn && rIn)
			{
				sideGrab = 1;
				cout << "right side grab" << endl;
				
				ddRect.colors = {vec4::Red,vec4::Red,vec4::Lime,vec4::Lime,vec4::Red,vec4::Red,vec4::Red,vec4::Red};
			}
			
			if (dIn && rIn)
			{
				sideGrab = 2;
				cout << "right and down side grab" << endl;
				
				ddRect.colors = {vec4::Red,vec4::Red,vec4::Red,vec4::Lime,vec4::Lime,vec4::Red,vec4::Red,vec4::Red};
			}
		}
	}
	
	/////////////////////////////////////////////////////////////
	void onMouseMoved(fm::vec2 p,fm::vec2 prevP) override
	{
		if (sideGrab == 0)
		{
			setSize(getSize() + vec2(0,p.y - prevP.y));
		}
		if (sideGrab == 1)
		{
			setSize(getSize() + vec2(p.x - prevP.x,0));
		}
		if (sideGrab == 2)
		{
			setSize(getSize() + p - prevP);
		}
		
		bool dIn = downSideContains(p);
		bool rIn = rightSideContains(p);
		
		if (rIn && !dIn)
		{
			Mouse::setCursor(Mouse::SizeRight);
		}
		if (!rIn && dIn)
		{
			Mouse::setCursor(Mouse::SizeBottom);
		}
		if (rIn && dIn)
		{
			Mouse::setCursor(Mouse::SizeBottomRight);
		}
		if (!rIn && !dIn)
		{
			Mouse::setCursor(Mouse::Arrow);
		}
	}
	
	/////////////////////////////////////////////////////////////
	void onMouseEnter(fm::vec2 p) override
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void onMouseLeave(fm::vec2 p) override
	{
		if (!isPressed(Mouse::Left))
		{
			Mouse::setCursor(Mouse::Arrow);
		}
	}
	
	/////////////////////////////////////////////////////////////
	void onRelease(fw::Mouse::Button button,fm::vec2 p) override
	{
		if (button == fw::Mouse::Left)
		{
			sideGrab = -1;
			
			ddRect.colors = {vec4::Red,vec4::Red,vec4::Red,vec4::Red,vec4::Red,vec4::Red,vec4::Red,vec4::Red};
		}
	}
};