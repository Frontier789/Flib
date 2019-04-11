#ifndef FRONTIER_WIDGET_HPP_INCLUDED
#define FRONTIER_WIDGET_HPP_INCLUDED
#define FRONTIER_WIDGET

#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Gui/ClickListener.hpp>
#include <FRONTIER/Gui/ScrollListener.hpp>

namespace fgui
{
    /////////////////////////////////////////////////////////////
    /// @brief General widget class with public callbacks for common events
    /// 
    /////////////////////////////////////////////////////////////
    class FRONTIER_API Widget : public GuiElement, public virtual MouseMoveListener, public ScrollListener
    {
    public:
        fm::Delegate<void,fg::ShaderManager &> ondraw; ///< Callback used when drawing
        fm::Delegate<void,fm::vec2,fw::Mouse::Button> onbuttonpress; ///< Callback used when the user presses a button
        fm::Delegate<void,fm::vec2,fw::Mouse::Button> onbuttonrelease; ///< Callback used when the user releases a button
        fm::Delegate<void,fm::vec2,fm::vec2> onmousemove; ///< Callback used when mouse moves
        fm::Delegate<void,float,bool> onscroll; ///< Callback used when scrolling happens
        fm::Delegate<void,fw::Event> onevent; ///< Callback used when any event happens
        fm::Delegate<void,fw::Keyboard::Key> onkeypress; ///< Callback used when a key is started being pressed
        fm::Delegate<void,fw::Keyboard::Key> onkeyrelease; ///< Callback used when a key is no longer pressed
        fm::Delegate<void,fm::Time> onupdate; ///< Callback used once every frame
        fm::Delegate<void,fm::vec2i> onresize; ///< Callback used when the owner context is resized

        /////////////////////////////////////////////////////////////
        /// @brief Default constructor
        /// 
        /// @param owner The owner context
        /// @param size The initial size
        ///
        /////////////////////////////////////////////////////////////
        Widget(GuiContext &owner,fm::vec2 size = fm::vec2());

        /////////////////////////////////////////////////////////////
        /// @brief draw the gui element
        ///
        /// @param shader The shader to use
        ///
        /////////////////////////////////////////////////////////////
        virtual void onDraw(fg::ShaderManager &shader) override;
        
        /////////////////////////////////////////////////////////////
        /// @brief Called when the mouse moves inside the gui element
        /// 
        /// @param p The position of the mouse after moving
        /// @param prevP The position of the mouse before moving
        /// 
        /////////////////////////////////////////////////////////////
        virtual void onMouseMove(fm::vec2 p,fm::vec2 prevP) override;
            
        /////////////////////////////////////////////////////////////
        /// @brief Called when the element is scrolled
        /// 
        /// @param amount The amount the element is scrolled
		/// @param horizontal Indicates whether vertical or horizontal scroll happened
        /// 
        /////////////////////////////////////////////////////////////
        void onScroll(float amount,bool horizontal) override;

        /////////////////////////////////////////////////////////////
        /// @brief Handle an event
        /// 
        /// @param ev The event
        /// 
        /// @return True iff the event got handled
        /// 
        /////////////////////////////////////////////////////////////
        virtual bool onEvent(fw::Event &ev) override;

        /////////////////////////////////////////////////////////////
        /// @brief update the gui element
        /// 
        /// @param dt The elapsed time since last update
        /// 
        /////////////////////////////////////////////////////////////
        virtual void onUpdate(const fm::Time &dt) override;
    };
}

#endif // FRONTIER_WIDGET_HPP_INCLUDED
