#ifndef FRONTIER_HOUSING_HPP_INCLUDED
#define FRONTIER_HOUSING_HPP_INCLUDED

#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/Gui/Div.hpp>

namespace Fgui
{
	class Housing : public Div
	{
	private:
		static const std::string m_className; ///< The name of the class, equals to "Housing"

	protected:
		fm::Size m_vertexCount;
		fg::Buffer m_vbo;
		fm::mat4 m_transform;
		fm::vec4 m_color;

		bool m_needBuildVertices;
		bool m_needSetupPosition;
		void buildVertices();
		void setupPosition();

	public:

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// @param name The identifier of the instance
		/// @param anchor The anchor of the widget
		/// @param size The size of the widget
		/// @param parent The parent of the widget
		/// @param margin The margin of the div
		/// @param content The contained widget
		/// @param color The color of the border line
		///
		/////////////////////////////////////////////////////////////
		Housing(const std::string &name = "NoName",
				const Anchor &anchor = Anchor(),
				const fm::vec2 &size = fm::vec2(),
				Widget *parent = fm::nullPtr,
				const fm::vec2 &margin = fm::vec2(),
				Widget *content = fm::nullPtr,
				fm::vec4 color = fm::vec4::Black);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~Housing();

		/////////////////////////////////////////////////////////////
		/// @brief Set the minimal size of the div
		///
		/// @param size The new minimal size
		///
		/////////////////////////////////////////////////////////////
		virtual void setMinSize(const fm::vec2 &size);

		/////////////////////////////////////////////////////////////
		/// @brief Set the margin of the div
		///
		/// @param margin The new margin
		///
		/////////////////////////////////////////////////////////////
		virtual void setMargin(const fm::vec2 &margin);

		/////////////////////////////////////////////////////////////
		/// @brief Set the parent of the div
		///
		/// @param parent The new parent
		///
		/////////////////////////////////////////////////////////////
		virtual void setParent(Widget *parent);

		/////////////////////////////////////////////////////////////
		/// @brief Set the minimal size of the div
		///
		/// @param size The new minimal size
		///
		/////////////////////////////////////////////////////////////
		virtual void setSize(const fm::vec2 &size);

		/////////////////////////////////////////////////////////////
		/// @brief Set the color of the border
		///
		/// @param color The new color
		///
		/////////////////////////////////////////////////////////////
		virtual void setColor(const fm::vec4 &color);

		/////////////////////////////////////////////////////////////
		/// @brief Get the color of the border
		///
		/// @return The color
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec4 getColor();

		/////////////////////////////////////////////////////////////
		/// @brief Set the anchor
		///
		/// @param anchor The new anchor
		///
		/////////////////////////////////////////////////////////////
		virtual void setAnchor(const Anchor &anchor);

		/////////////////////////////////////////////////////////////
		/// @brief Draw the div
		///
		/// Passed through to the contained widget
		///
		/////////////////////////////////////////////////////////////
		virtual void onDraw();

		/////////////////////////////////////////////////////////////
		/// @brief Handle the fixed update
		///
		/// Passed through to the contained widget
		///
		/////////////////////////////////////////////////////////////
		virtual void onUpdate();

		/////////////////////////////////////////////////////////////
		/// @brief Handle the change of parent
		///
		/// Passed through to the contained widget
		///
		/////////////////////////////////////////////////////////////
		virtual void onParentChange();

		/////////////////////////////////////////////////////////////
		/// @brief Set the contained widget
		///
		/// @param content The new contained widget
		///
		/////////////////////////////////////////////////////////////
		virtual void setContent(Widget *content);

		/////////////////////////////////////////////////////////////
		/// @brief Get the real type of the class
		///
		/// When creating a new class it may override this
		/// function to provide a unique name as return value
		///
		/////////////////////////////////////////////////////////////
		virtual const std::string &getRealType() const;
	};
}

#endif // FRONTIER_HOUSING_HPP_INCLUDED
