#ifndef FRONTIER_GLOBJECT_HPP_INCLUDED
#define FRONTIER_GLOBJECT_HPP_INCLUDED
#define FRONTIER_GLOBJECT
namespace fg
{
	//////////////////////////////////
	/// 
	/// 	@brief Base class for opengl objects that have id such as texture, shader etc
	/// 
	//////////////////////////////////
    class GlObject
    {
		unsigned int m_id; ///< The id of the object
    public:
		
		//////////////////////////////////
		/// @brief Default constructor
		/// 
		/// This constructor initalizes the id to 0 (invalid for any OpenGL id type)
		/// 
		//////////////////////////////////
		GlObject();
		
		//////////////////////////////////
		/// @brief Virtual destructor
		/// 
		//////////////////////////////////
		virtual ~GlObject();
		
		//////////////////////////////////
		/// @brief Get rw access to the id
		/// 
		/// @return The id
		/// 
		//////////////////////////////////
		unsigned int &getGlId();
		
		//////////////////////////////////
		/// @brief Get read-only access to the id
		/// 
		/// @return The id
		/// 
		//////////////////////////////////
		const unsigned int &getGlId() const;
    };
	//////////////////////////////////
	/// @class GlObject
	/// 
	/// @ingroup Graphics
	/// 
	/// Every class in the Graphics module that represents a 
	/// OpenGL object is derived from this class.
	/// Therefore their id is publicly accessible, however be warned
	/// that if you modify the OpenGL object the class doesn't query directly 
	/// the OpenGL object (rather the functions called on it) and will have corrupt data.
	/// 
	//////////////////////////////////
}

#endif //FRONTIER_GLOBJECT_HPP_INCLUDED