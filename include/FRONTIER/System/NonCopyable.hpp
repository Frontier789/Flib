#ifndef FRONTIER_NONCOPYABLE_HPP_INCLUDED
#define FRONTIER_NONCOPYABLE_HPP_INCLUDED
#define FRONTIER_NONCOPYABLE
namespace fm
{
	//////////////////////////////////
	/// @brief Virtual base class for types that cannot be copied
	/// 
	//////////////////////////////////
    class NonCopyable
    {
    public:
    
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
        inline NonCopyable()  {}
        
		/////////////////////////////////////////////////////////////
		/// @brief Virtual destructor
		///
		/////////////////////////////////////////////////////////////
        inline virtual ~NonCopyable() {}
    private:
        
		/////////////////////////////////////////////////////////////
		/// @brief Private copy constructor
		/// 
		/// Also this this function is not defined
		/// 
		/////////////////////////////////////////////////////////////
        NonCopyable(const NonCopyable &);
        
		/////////////////////////////////////////////////////////////
		/// @brief Private overload of the assignment operator
		/// 
		/// Also this this function is not defined
		/// 
		/////////////////////////////////////////////////////////////
        NonCopyable &operator=(const NonCopyable &);
    };
}
#endif //FRONTIER_NONCOPYABLE_HPP_INCLUDED

////////////////////////////////////////////////////////////
/// @class fm::NonCopyable
/// @ingroup System
///
/// If a class shall not be copied by copy constructruction or by assignment that class should be derived
/// from fm::NonCopyable
/// 
/// Usage example:
/// @code
/// 
/// 
/// class A : public fm::NonCopyable
/// {
/// public:
/// 	A() : array(new int[5]) {}
/// 	~A()
/// 	{
/// 		delete[] array;
/// 	}
/// 	int *array;
/// };
/// void f_val(A a)
/// {
/// 	for (int i=0;i<5;i++)
/// 		*(a.array+i) = i*i;
/// }
/// void f_ref(A &a)
/// {
/// 	for (int i=0;i<5;i++)
/// 		*(a.array+i) = i*i;
/// }
/// int main()
/// {
/// 	A a;
/// 	f_val(a); // produces compilation error
/// 	f_ref(a); // legit since no copying is done
/// }
/// 
/// @endcode
////////////////////////////////////////////////////////////
