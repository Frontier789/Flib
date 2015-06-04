////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_MATRIX_HPP_INCLUDED
#define FRONTIER_MATRIX_HPP_INCLUDED

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/System/Collector.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#include <FRONTIER/System/StorageOrder.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#define FRONTIER_MATRIX
namespace fm
{
	class Angle;

	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Templated class used for manipulating
	///			   <a href="http://en.wikipedia.org/wiki/Matrix_%28mathematics%29">matricess</a>
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T = float>
	class matrix
	{
		T m_data[W][H]; ///< 2D array holding the data of the matrix
	public:
		typedef T component_type;
		typedef matrix<W,H,T> &reference;
		typedef const matrix<W,H,T> &const_reference;
		enum {
			components = W*H ///< Public value indicating the amount of component_type's in the class
		};


		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes matrix to <a href="http://en.wikipedia.org/wiki/Identity_matrix">identity matrix</a>
		///
		/////////////////////////////////////////////////////////////
		matrix();

		/////////////////////////////////////////////////////////////
		/// @brief Construct with all fields set to same
		///
		/// Initializes matrix with every field set to @a all
		///
		/// @param all Value to set all fields
		///
		/////////////////////////////////////////////////////////////
		matrix(const T &all);

		/////////////////////////////////////////////////////////////
		/// @brief Construct the matrix form a 1D array
		///
		/// @param data Content to initialize with
		///
		/////////////////////////////////////////////////////////////
		matrix(const T (&data)[W*H]);

		/////////////////////////////////////////////////////////////
		/// @brief Construct the matrix form a 2D array
		///
		/// @param data Content to initialize with
		///
		/////////////////////////////////////////////////////////////
		matrix(const T (&data)[W][H]);

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// @param mat The matrix to be copied
		///
		/////////////////////////////////////////////////////////////
		matrix(const matrix<W,H,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @brief Construct from a Collector
		///
		/// If @a col has less elements than W*H
		/// the leftout cells are filled with T()
		///
		/// @param col The collector
		///
		/////////////////////////////////////////////////////////////
		template<class T2,class Container>
		matrix(const fm::Collector<T2,Container> &col);

		/////////////////////////////////////////////////////////////
		/// @brief Element access with bound check
		///
		/// @param x X offset
		/// @param y Y offset
		///
		/// @return Value at [x][y] or T(), if x is out of [0,W[ or y is out of [0,H[
		///
		/////////////////////////////////////////////////////////////
		T at(fm::Size x,fm::Size y) const;

		/////////////////////////////////////////////////////////////
		/// @brief Set element with bound check
		///
		/// Sets value at [x][y] if x is in [0,W[ and y is in [0,H[
		///
		/// @param x X offset
		/// @param y Y offset
		/// @param value Value to set the field to
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference set(fm::Size x,fm::Size y,const T &value);

		/////////////////////////////////////////////////////////////
		/// @brief Set matrix to <a href="http://en.wikipedia.org/wiki/Identity_matrix">identity</a>
		///
		/// Sets value at [i][j] to T(i==j)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference reset();

		/////////////////////////////////////////////////////////////
		/// @brief Set all fields to @a all
		///
		/// Sets value at [i][j] to all
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference reset(T all);

		/////////////////////////////////////////////////////////////
		/// @brief Flips the matrix around its main diagonal ( <a href="http://en.wikipedia.org/wiki/Transpose_matrix">matrix transpose</a>)
		///
		/// Returns a m matrix such that m[i][j]==original[j][i]
		///
		/// @return Transposed matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<H,W,T> transpose() const;

		/////////////////////////////////////////////////////////////
		/// @brief Multiply two matrices component-wisely (<a href="http://en.wikipedia.org/wiki/Hadamard_product_%28matrices%29">Hadamard product</a>)
		///
		/// Multiplication is performed on all fields invidually
		///
		/// @param mat The matrix to multiply by
		///
		/// @return Product of the multiplication
		///
		/////////////////////////////////////////////////////////////
		matrix<W,H,T> byComp(const matrix<W,H,T> &mat) const;

		/////////////////////////////////////////////////////////////
		/// @brief Converts the matrix
		///
		/// Converts the matrix to a W2xH2 sized matrix with base type T2.
		/// Returns a m matrix such that if i<W and j<H m[i][j]==T2(original[i][j])
		/// and if i>=W or j>=H then m[i][j]==T2(i==j)
		///
		/// @return Converted matrix
		///
		/////////////////////////////////////////////////////////////
		template<fm::Size W2,fm::Size H2,class T2>
		matrix<W2,H2,T2> convert() const;

		/////////////////////////////////////////////////////////////
		/// @brief Converts the matrix
		///
		/// Converts the matrix to a W2xH2 sized matrix.
		/// Returns a m matrix such that if i<W and j<H m[i][j]==original[i][j]
		/// and if i>=W or j>=H then m[i][j]==T(i==j)
		///
		/// @return Converted matrix
		///
		/////////////////////////////////////////////////////////////
		template<fm::Size W2,fm::Size H2>
		matrix<W2,H2,T> convert() const;

		/////////////////////////////////////////////////////////////
		/// @brief Access(rw) a row of the matrix
		///
		/// Because it returns a pointer to the first element of the row
		/// an element can be acces as m[x][y]
		///
		/// @param index Index of the row
		///
		/// @return Pointer to the first element of the @a index -th row
		///
		/////////////////////////////////////////////////////////////
		T *operator[](fm::Size index);

		/////////////////////////////////////////////////////////////
		/// @brief Access(r only) a row of the matrix
		///
		/// Because it returns a pointer to the first element of the row
		/// an element can be acces as m[x][y]
		///
		/// @param index Index of the row
		///
		/// @return Pointer to the first element of the @a index -th row
		///
		/////////////////////////////////////////////////////////////
		const T *operator[](fm::Size index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the data of the matrix
		///
		/// @param data Data to replace the matrix's fileds to
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator()(const T (&data)[W][H]);

		/////////////////////////////////////////////////////////////
		/// @brief Set the data of the matrix
		///
		/// @param data Data to replace the matrix's fileds to
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator()(const T (&data)[W*H]);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator *
		///
		/// @param value Value to multiply by
		///
		/// @return Result of the multiplication
		///
		/////////////////////////////////////////////////////////////
		matrix<W,H,T> operator*(const T &value) const;

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator /
		///
		/// @param value Value to divide by
		///
		/// @return Result of the division
		///
		/////////////////////////////////////////////////////////////
		matrix<W,H,T> operator/(const T &value) const;

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator *=
		///
		/// @param value Value to multiply by
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator*=(const T &value);

		/////////////////////////////////////////////////////////////
		/// @brief Overload of binary operator /=
		///
		/// @param value Value to divide by
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator/=(const T &value);


		/////////////////////////////////////////////////////////////
		/// @brief The <a href="http://en.wikipedia.org/wiki/Identity_matrix">identity matrix</a>
		///
		/// 1 0 ... 0  <br/>
		/// 0 1 ... 0  <br/>
		/// . . .   .  <br/>
		/// . .  .  .  <br/>
		/// . .   . .  <br/>
		/// 0 0 ... 1  <br/>
		///
		/// Its [i][j] element is 1 if i==j otherwise 0
		///
		/////////////////////////////////////////////////////////////
		static matrix<W,H,T> identity;
		enum {
			Width=W, ///< Alias to W
			Height=H ///< Alias to H
		};
	};

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (scalar)
	/// @param right Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	matrix<W,H,T> operator*(const T &left,const matrix<W,H,T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (matrix)
	/// @param right Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,fm::Size H2>
	matrix<W,H2,T> operator*(const matrix<W,H,T> &left,const matrix<H,H2,T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator +
	///
	/// @param left Left operand (matrix)
	/// @param right Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	matrix<W,H,T> operator+(const matrix<W,H,T> &left,const matrix<W,H,T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator -
	///
	/// @param left Left operand (matrix)
	/// @param right Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	matrix<W,H,T> operator-(const matrix<W,H,T> &left,const matrix<W,H,T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of unary operator -
	///
	/// @param mat Matrix to negate
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	matrix<W,H,T> operator-(const matrix<W,H,T> &mat);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator +=
	///
	/// @param left Left operand (matrix)
	/// @param right Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	matrix<W,H,T> &operator+=(matrix<W,H,T> &left,const matrix<W,H,T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator -=
	///
	/// @param left Left operand (matrix)
	/// @param right Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	matrix<W,H,T> &operator-=(matrix<W,H,T> &left,const matrix<W,H,T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator *=
	///
	/// @param left Left operand (matrix)
	/// @param right Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	matrix<W,H,T> &operator*=(matrix<W,H,T> &left,const matrix<H,H,T> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator ==
	///
	/// @param left Left operand (matrix)
	/// @param right Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W ,fm::Size H ,class T,
             fm::Size W2,fm::Size H2,class T2>
	bool operator==(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator !=
	///
	/// @param left Left operand (matrix)
	/// @param right Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<fm::Size W ,fm::Size H ,class T,
             fm::Size W2,fm::Size H2,class T2>
	bool operator!=(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right);



	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix fm::vector2
	/// @brief Overload of binary operator *
	///
	/// The multiplication is computed as if the vector was
	/// a 2x1 matrix
	///
	/// @param mat mat operand (matrix)
	/// @param vec vec operand (vector)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	vector2<T> operator*(const matrix<2,2,T> &mat,const vector2<T> &vec);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix fm::vector3
	/// @brief Overload of binary operator *
	///
	/// The multiplication is computed as if the vector was
	/// a 3x1 matrix
	///
	/// @param mat Left operand (matrix)
	/// @param vec Right operand (vector)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	vector3<T> operator*(const matrix<3,3,T> &mat,const vector3<T> &vec);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix fm::vector4
	/// @brief Overload of binary operator *
	///
	/// The multiplication is computed as if the vector was
	/// a 4x1 matrix
	///
	/// @param mat Left operand (matrix)
	/// @param vec Right operand (vector)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	vector4<T> operator*(const matrix<4,4,T> &mat,const vector4<T> &vec);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix fm::vector2
	/// @brief Overload of binary operator *
	///
	/// The multiplication is computed as if the vector was
	/// a 1x2 matrix
	///
	/// @param vec Left operand (vector)
	/// @param mat Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	vector2<T> operator*(const vector2<T> &vec,const matrix<2,2,T> &mat);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix fm::vector3
	/// @brief Overload of binary operator *
	///
	/// The multiplication is computed as if the vector was
	/// a 1x3 matrix
	///
	/// @param vec Left operand (vector)
	/// @param mat Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	vector3<T> operator*(const vector3<T> &vec,const matrix<3,3,T> &mat);

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix fm::vector4
	/// @brief Overload of binary operator *
	///
	/// The multiplication is computed as if the vector was
	/// a 1x4 matrix
	///
	/// @param vec Left operand (vector)
	/// @param mat Right operand (matrix)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	vector4<T> operator*(const vector4<T> &vec,const matrix<4,4,T> &mat);







	/////////////////////////////////////////////////////////////
	///
	/// @brief Namespace used to hold functions that operate on square matrices
	///
	/////////////////////////////////////////////////////////////
	namespace MATRIX
	{
		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculates the <a href="http://en.wikipedia.org/wiki/Determinant">determinant</a> of a square matrix
		///
		/// It is a slow operation threfore you should implement your own
		/// method to count the determinant if you plan to use it frequently
		///
		/// @param mat The matrix what's determinant is to be calculated
		///
		/// @return The determinant
		///
		/////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		T det(const matrix<N,N,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculates the matrix of minors see <a href="http://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html">here</a>
		///
		/// @param mat The matrix what's matrix of minors is to be calculated
		///
		/// @return The matrix of minors
		///
		/////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		matrix<N,N,T> minorMat(const matrix<N,N,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculates the <a href="http://en.wikipedia.org/wiki/Adjugate_matrix">adjugate</a> of a matrix
		///
		/// @param mat The matrix what's adjugate is to be calculated
		///
		/// @return The adjugate
		///
		/////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		matrix<N,N,T> adj(const matrix<N,N,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculates the <a href="http://en.wikipedia.org/wiki/Invertible_matrix">inverse</a> of a matrix
		///
		/// @param mat The matrix what's inverse is to be calculated
		///
		/// @return The inverse
		///
		/////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		matrix<N,N,T> inverse(const matrix<N,N,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Non-member function to <a href="http://en.wikipedia.org/wiki/Transpose_matrix">transpose</a> a matrix
		///
		/// @param mat The matrix what's inverse is to be calculated
		///
		/// @return The inverse
		///
		/////////////////////////////////////////////////////////////
		template<fm::Size W,fm::Size H,class T>
		matrix<H,W,T> transpose(const matrix<W,H,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate the <a href="http://en.wikipedia.org/wiki/Trace_%28linear_algebra%29">trace</a> of a matrix
		///
		/// @param mat The matrix what's trace is to be calculated
		///
		/// @return The trace
		///
		/////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		T trace(const matrix<N,N,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Translation_%28geometry%29#Matrix_representation">translation matrix</a>
		///
		/// @param translate The offset of the translation
		/// @param storeOrder The way to store the result
		///
		/// @return The translation matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> translation(const vec3 &translate,  StorageOrder storeOrder=RowMajor);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Translation_%28geometry%29#Matrix_representation">translation matrix</a>
		///
		/// @param x The x offset of the translation
		/// @param y The y offset of the translation
		/// @param storeOrder The way to store the result
		///
		/// @return The translation matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> translation(float x,float y,		  StorageOrder storeOrder=RowMajor);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Translation_%28geometry%29#Matrix_representation">translation matrix</a>
		///
		/// @param x The x offset of the translation
		/// @param y The y offset of the translation
		/// @param z The z offset of the translation
		/// @param storeOrder The way to store the result
		///
		/// @return The translation matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> translation(float x,float y,float z,StorageOrder storeOrder=RowMajor);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// @param scale The x and y component (z=1) of the the scaling
		/// @param storeOrder The way to store the result
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> scaling(const vec2 &scale,	  StorageOrder storeOrder=RowMajor);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// @param scale The x, y and z component of the the scaling
		/// @param storeOrder The way to store the result
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> scaling(const vec3 &scale,	  StorageOrder storeOrder=RowMajor);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// @param x The x component of the scaling
		/// @param y The y component of the scaling
		/// @param storeOrder The way to store the result
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> scaling(float x,float y,		  StorageOrder storeOrder=RowMajor);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// @param x The x component of the scaling
		/// @param y The y component of the scaling
		/// @param z The z component of the scaling
		/// @param storeOrder The way to store the result
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
        matrix<4,4,float> scaling(float x,float y,float z,StorageOrder storeOrder=RowMajor);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Rotation_matrix#Basic_rotations">rotation matrix</a> around the Z axis
		///
		/// For rotation with axis other than the Z axis use Quaternions
		/// @see fm::Quat
		///
		/// @param angle The angle of the rotation
		/// @param storeOrder The way to store the result
		///
		/// @return The rotation matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> rotation(const Angle &angle,StorageOrder storeOrder=RowMajor);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://www.scratchapixel.com/lessons/3d-advanced-lessons/perspective-and-orthographic-projection-matrix/perspective-projection-matrix/">perspective projection matrix</a>
		///
		/// @param fieldOfView <a href="http://en.wikipedia.org/wiki/Field_of_view_in_video_games">Field of view</a>
		/// @param aspect The ratio of the window width and height
		/// @param Znear The distance of the near clipping plane
		/// @param Zfar The distance of the far clipping plane
		/// @param storeOrder The way to store the result
		///
		/// @return The perspective matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> perspective(const Angle &fieldOfView,float aspect,float Znear,float Zfar,StorageOrder storeOrder=RowMajor);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Orthographic_projection">orthogonal projection matrix</a>
		///
		/// @param left Value of the left side on the vertical clipping plane
		/// @param bottom Value of the bottom side on the vertical clipping plane
		/// @param right Value of the right side on the vertical clipping plane
		/// @param top Value of the upper side on the vertical clipping plane
		/// @param nearVal Distance to the nearer clipping plane
		/// @param farVal Distance to the further clipping plane
		/// @param storeOrder The way to store the result
		///
		/// @return The orthogonal projection matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> ortho(float left,float bottom,float right,float top,float nearVal,float farVal,StorageOrder storeOrder=RowMajor);


		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a look-at view matrix
		///
		/// This is a combination of a translation and a rotation matrix <br/>
		/// see how glu <a href="http://www.opengl.org/wiki/GluLookAt_code">implemented</a> this function
		///
		/// @param cam_pos Position of the viewer
		/// @param target_pos Position of the objet where the camera is loooking
		/// @param storeOrder The way to store the result
		///
		/// @return The look-at matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> lookAt(const vec3 &cam_pos,const vec3 &target_pos,				   StorageOrder storeOrder=RowMajor);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a look-at view matrix
		///
		/// This is a combination of a translation and a rotation matrix <br/>
		/// see how glu <a href="http://www.opengl.org/wiki/GluLookAt_code">implemented</a> this function
		///
		/// @param cam_pos Position of the viewer
		/// @param target_pos Position of the objet where the camera is loooking
		/// @param up_dir Vector pointing upward
		/// @param storeOrder The way to store the result
		///
		/// @return The look-at matrix
		///
		/////////////////////////////////////////////////////////////
        matrix<4,4,float> lookAt(const vec3 &cam_pos,const vec3 &target_pos,const vec3 &up_dir,StorageOrder storeOrder=RowMajor);



		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Translation_%28geometry%29#Matrix_representation">translation matrix</a>
		///
		/// @param translate The offset of the translation
		///
		/// @return The translation matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		matrix<4,4,float> translation(const vec3 &translate);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Translation_%28geometry%29#Matrix_representation">translation matrix</a>
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument
		///
		/// @param x The x offset of the translation
		/// @param y The y offset of the translation
		///
		/// @return The translation matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		matrix<4,4,float> translation(float x,float y);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Translation_%28geometry%29#Matrix_representation">translation matrix</a>
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument
		///
		/// @param x The x offset of the translation
		/// @param y The y offset of the translation
		/// @param z The z offset of the translation
		///
		/// @return The translation matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		matrix<4,4,float> translation(float x,float y,float z);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument
		///
		/// @param scale The x and y component (z=1) of the the scaling
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		matrix<4,4,float> scaling(const vec2 &scale);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument
		///
		/// @param scale The x, y and z component of the the scaling
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		matrix<4,4,float> scaling(const vec3 &scale);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument
		///
		/// @param x The x component of the scaling
		/// @param y The y component of the scaling
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		matrix<4,4,float> scaling(float x,float y);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument
		///
		/// @param x The x component of the scaling
		/// @param y The y component of the scaling
		/// @param z The z component of the scaling
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
        matrix<4,4,float> scaling(float x,float y,float z);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Rotation_matrix#Basic_rotations">rotation matrix</a> around the Z axis
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument.
		///
		/// @param angle The angle of the rotation
		///
		/// @return The rotation matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		matrix<4,4,float> rotation(const Angle &angle);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://www.scratchapixel.com/lessons/3d-advanced-lessons/perspective-and-orthographic-projection-matrix/perspective-projection-matrix/">perspective projection matrix</a>
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument
		///
		/// @param fieldOfView <a href="http://en.wikipedia.org/wiki/Field_of_view_in_video_games">Field of view</a>
		/// @param aspect The ratio of the window width and height
		/// @param Znear The distance of the near clipping plane
		/// @param Zfar The distance of the far clipping plane
		///
		/// @return The perspective matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		matrix<4,4,float> perspective(const Angle &fieldOfView,float aspect,float Znear,float Zfar);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Orthographic_projection">orthogonal projection matrix</a>
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument
		///
		/// @param left Value of the left side on the vertical clipping plane
		/// @param bottom Value of the bottom side on the vertical clipping plane
		/// @param right Value of the right side on the vertical clipping plane
		/// @param top Value of the upper side on the vertical clipping plane
		/// @param nearVal Distance to the nearer clipping plane
		/// @param farVal Distance to the further clipping plane
		///
		/// @return The orthogonal projection matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		matrix<4,4,float> ortho(float left,float bottom,float right,float top,float nearVal,float farVal);


		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a look-at view matrix
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument.
		///
		/// @param cam_pos Position of the viewer
		/// @param target_pos Position of the objet where the camera is loooking
		///
		/// @return The look-at matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		matrix<4,4,float> lookAt(const vec3 &cam_pos,const vec3 &target_pos);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a look-at view matrix
		///
		/// This is a templated version of the original function.
		/// It can be used to supply the store order as a template argument
		///
		/// @param cam_pos Position of the viewer
		/// @param target_pos Position of the objet where the camera is loooking
		/// @param up_dir Vector pointing upward
		///
		/// @return The look-at matrix
		///
		/////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
        matrix<4,4,float> lookAt(const vec3 &cam_pos,const vec3 &target_pos,const vec3 &up_dir);
	}

	typedef matrix<2,2,float> mat2f;
	typedef matrix<2,2,float> mat2;
	typedef matrix<2,2,long> mat2u;
	typedef matrix<2,2,int> mat2i;

	typedef matrix<3,3,float> mat3f;
	typedef matrix<3,3,float> mat3;
	typedef matrix<3,3,long> mat3u;
	typedef matrix<3,3,int> mat3i;

	typedef matrix<4,4,float> mat4f;
	typedef matrix<4,4,float> mat4;
	typedef matrix<4,4,long> mat4u;
	typedef matrix<4,4,int> mat4i;

}

#endif //FRONTIER_MATRIX_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Matrix.inl>
#endif
////////////////////////////////////////////////////////////
/// @class fm::matrix
/// @ingroup System
///
/// fm::matrix is a templated class for manipulating matrices
/// it is capable of storing really big (as big as fm::Size can be) data and
/// can operte on custom types (default is float)
///
/// The template parameter T is the type of the data fields. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or double.
///
/// The most common specializations are typedefed:
/// @li fm::matrix<2,2,float> is fm::mat2
/// @li fm::matrix<2,2,float> is fm::mat2f
/// @li fm::matrix<2,2,long > is fm::mat2i
/// @li fm::matrix<2,2,int  > is fm::mat2u
///
/// @li fm::matrix<3,3,float> is fm::mat3
/// @li fm::matrix<3,3,float> is fm::mat3f
/// @li fm::matrix<3,3,long > is fm::mat3i
/// @li fm::matrix<3,3,int  > is fm::mat3u
///
/// @li fm::matrix<4,4,float> is fm::mat4
/// @li fm::matrix<4,4,float> is fm::mat4f
/// @li fm::matrix<4,4,long > is fm::mat4i
/// @li fm::matrix<4,4,int  > is fm::mat4u
///
/// The fm::matrx class posesses a very flexible and feature rich interface, underlying data array
/// can not be accessed directly however using the appretiated member methods (at,set) or the
/// overloaded [] operators it can be easily managed
/// The MATRIX namespace contains mathematical function to operate on square matrices.
///
/// Usage example:
/// @code
///
/// float data_m[4][4]={{1,0,0,1},
///						{0,1,0,1},
///					    {0,0,1,1},
///					    {0,0,0,1}};
///
/// fm::mat4 m(data_m);
/// std::cout<<m<<std::endl;
///
/// fm::matrix<4,1> m2((float []){1,2,3,4});
///
/// std::cout<<m*m2<<std::endl<<std::endl;
/// std::cout<<m2.transpose()*m<<std::endl;
///
/// @endcode
///
/// Example program:
/// 	computing the solution of a
/// 	linear equation system with N unknowns
/// @code
///
///
/// #include <iostream>
/// #include <FRONTIER/System/Matrix.hpp>
/// #define N 3
/// int main()
/// {
/// 	fm::matrix<N,N> coefs;
/// 	fm::matrix<N,1> consts;
/// 	for (int i=0;i<N;i++)
/// 	{
/// 		std::string ending=((i%10==0 && i!=10) ? "st\0" : ((i%10==1 && i!=11) ? "nd\0" : ((i%10==3 && i!=12) ? "rd\0" : "th\0")));
/// 		std::cout<<"enter the coeffecients of the "<<i+1<<ending<<" equation"<<std::endl;
/// 		for (int x=0;x<N;x++)
/// 			std::cin>>coefs[i][x];
/// 		std::cout<<"enter the result of the "<<i+1<<ending<<" equation"<<std::endl;
/// 		std::cin>>consts[i][0];
/// 	}
/// 	std::cout<<"The solution is:"<<std::endl;
/// 	std::cout<<fm::MATRIX::inverse(coefs)*consts<<std::endl;
/// }
/// @endcode
///
/// Example usage of the program
/// @code
/// 2x + 3y + 2z = 7
///  x +  y +  z = 3
/// 2x + 2y + 3z = 6
///
///
/// Running the program:
///
/// enter the coeffecients of the 1st equation
/// 2 3 2
/// enter the result of the 1st equation
/// 7
/// enter the coeffecients of the 2nd equation
/// 1 1 1
/// enter the result of the 2nd equation
/// 3
/// enter the coeffecients of the 3th equation
/// 2 2 3
/// enter the result of the 3th equation
/// 6
/// The solution is:
/// 2
/// 1
/// 0
///
/// and it is correct:
/// 2*2+3*1+2*0 = 7
/// 1*2+1*1+1*0 = 3
/// 2*2+2*1+3*0 = 6
///
///
/// @endcode
/// @see fm::vector2
/// @see fm::vector3
/// @see fm::vector4
/// @see fm::Angle
////////////////////////////////////////////////////////////
