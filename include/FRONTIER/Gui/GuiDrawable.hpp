#ifndef FRONTIER_GUIDRAWABLE_HPP_INCLUDED
#define FRONTIER_GUIDRAWABLE_HPP_INCLUDED
#define FRONTIER_GUIDRAWABLE

#include <FRONTIER/Gui/GuiElement.hpp>

namespace fgui
{
    /////////////////////////////////////////////////////////////
    /// @brief Proxy object for fg::Drawable to serve as a gui element
    /// 
    /////////////////////////////////////////////////////////////
    class FRONTIER_API GuiDrawable : public GuiElement
    {
        Drawable &m_obj; ///< The drawable
    
    public:
        /////////////////////////////////////////////////////////////
        /// @brief Create the object, referencing an existing drawable
        /// 
        /// @param cont The owner context of the gui element
        /// @param drawable The drawable
        /// 
        /////////////////////////////////////////////////////////////
        GuiDrawable(GuiContext &cont,Drawable &drawable);
        
        /////////////////////////////////////////////////////////////
        /// @brief Draw the assigned drawable object
        /// 
        /// @param shader The shader to use
        /// 
        /////////////////////////////////////////////////////////////
        void onDraw(fg::ShaderManager &shader) override;
    };
}

#endif // FRONTIER_GUIDRAWABLE_HPP_INCLUDED
