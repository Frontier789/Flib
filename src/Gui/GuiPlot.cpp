#include <FRONTIER/Gui/GuiPlot.hpp>
#include <FRONTIER/Gui/GuiLayout.hpp>


namespace fgui
{
	/////////////////////////////////////////////////////////////
	GuiPlot::GuiPlot(fm::vec2i winSize,fm::String title,fm::Delegate<GuiElement*,GuiWindow&> allocator)
	{
		m_fut = std::async(std::launch::async, [=]() {
			
			m_win = GuiWindow(winSize,title);
			m_win.setDepthTest(fg::LEqual);
			
			auto elem = allocator(m_win);
			if (elem)
				m_win.getMainLayout().addChildElement(elem);
			
			m_win.runGuiLoop();
			m_win.close();
		});
	}
	
	/////////////////////////////////////////////////////////////
	GuiPlot::GuiPlot(fm::vec2i winSize,fm::String title) : 
		GuiPlot(winSize, title, [&](){
			return createPlotterElement();
		})
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiPlot::~GuiPlot()
	{
		stop();
		wait();
	}
	
	/////////////////////////////////////////////////////////////
	GuiPlot &GuiPlot::stop()
	{
		m_win.stopGuiLoop();
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	GuiPlot &GuiPlot::wait()
	{
		if (m_fut.valid())
			m_fut.wait();
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	bool GuiPlot::running() const
	{
		return m_fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}
}
