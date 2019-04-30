////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/util/RequireCpp11.hpp>

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Delegate.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/Angle.hpp>
#include <type_traits>
#include <array>

#define FRONTIER_MATRIX
namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Templated class used for manipulating <a href="http://en.wikipedia.org/wiki/Matrix_%28mathematics%29">matricess</a>
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<Size W,Size H = W,class T = float>
	class matrix
	{
		std::array<std::array<T,H>,W> m_data; ///< 2D array holding the data of the matrix
	public:
		typedef T component_type;
		typedef matrix<W,H,T> &reference;
		typedef const matrix<W,H,T> &const_reference;
		enum {
			components = W*H, ///< Public value indicating the amount of component_types in the class
			Width  = W, ///< Alias to W
			Height = H  ///< Alias to H
		};
		
		static const bool isSquare; ///< True iff the matrix is a square matrix

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
		/// @brief Create a matrix given a function
		///
		/// @param fun The function to use
		///
		/////////////////////////////////////////////////////////////
		matrix(fm::Delegate<T,vec2i> fun);

		/////////////////////////////////////////////////////////////
		/// @brief Construct a matrix<w,h> from w*h values
		/// 
		/// @param args The values to use
		///
		/////////////////////////////////////////////////////////////
		template<class... ArgTypes,
				 class = typename std::enable_if<
					sizeof...(ArgTypes) == W*H
				 >::type>
		matrix(ArgTypes... args);

		/////////////////////////////////////////////////////////////
		/// @brief Construct a matrix<N,h> from h vectorNs
		/// 
		/// @param args The column vectors to use
		///
		/////////////////////////////////////////////////////////////
		template<class... ArgTypes,
				 class = typename std::enable_if<
					W == 2 && sizeof...(ArgTypes) == H
				 >::type,class=void>
		matrix(ArgTypes... args);

		/////////////////////////////////////////////////////////////
		/// @brief Construct a matrix<N,h> from h vectorNs
		/// 
		/// @param args The column vectors to use
		///
		/////////////////////////////////////////////////////////////
		template<class... ArgTypes,
				 class = typename std::enable_if<
					W == 3 && sizeof...(ArgTypes) == H
				 >::type,class=void,class=void>
		matrix(ArgTypes... args);

		/////////////////////////////////////////////////////////////
		/// @brief Construct a matrix<N,h> from h vectorNs
		/// 
		/// @param args The column vectors to use
		///
		/////////////////////////////////////////////////////////////
		template<class... ArgTypes,
				 class = typename std::enable_if<
					W == 4 && sizeof...(ArgTypes) == H
				 >::type,class=void,class=void,class=void>
		matrix(ArgTypes... args);

		/////////////////////////////////////////////////////////////
		/// @brief Convert to scalar if 1x1 matrix
		///
		/// @return Value at [0][0]
		///
		/////////////////////////////////////////////////////////////
		template<class K,class = typename std::enable_if<W == 1 && H == 1 && std::is_convertible<T,K>::value,K>::type>
		operator K() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Convert to vectorN if Nx1 matrix
		///
		/// @return First column vector
		///
		/////////////////////////////////////////////////////////////
		template<class K,class = typename std::enable_if<W==2 && H == 1 && std::is_convertible<T,K>::value,K>::type>
		operator vector2<K>() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Convert to vectorN if Nx1 matrix
		///
		/// @return First column vector
		///
		/////////////////////////////////////////////////////////////
		template<class K,class = typename std::enable_if<W==3 && H == 1 && std::is_convertible<T,K>::value,K>::type>
		operator vector3<K>() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Convert to vectorN if Nx1 matrix
		///
		/// @return First column vector
		///
		/////////////////////////////////////////////////////////////
		template<class K,class = typename std::enable_if<W==4 && H == 1 && std::is_convertible<T,K>::value,K>::type>
		operator vector4<K>() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Element access with bound check
		///
		/// @param x X offset
		/// @param y Y offset
		///
		/// @return Value at [x][y] or T(), if x is out of [0,W[ or y is out of [0,H[
		///
		/////////////////////////////////////////////////////////////
		T at(Size x,Size y) const;

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
		reference set(Size x,Size y,const T &value);

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
		/// Sets value at [i][j] to @a all
		/// 
		/// @param all The value to use
		/// 
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference reset(const T &all);

		/////////////////////////////////////////////////////////////
		/// @brief Set matrix to <a href="http://en.wikipedia.org/wiki/Identity_matrix">identity</a>
		///
		/// Sets value at [i][i] to @a diag
		/// Sets value at [i][j] if i!=j to @a rest
		/// 
		/// @param diag The value to be present on the main diagonal
		/// @param rest The value to be present outside the main diagonal
		/// 
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference reset(const T &diag,const T &rest);

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
		/// @brief Flips the matrix around its main diagonal ( <a href="http://en.wikipedia.org/wiki/Transpose_matrix">matrix transpose</a>)
		///
		/// Returns a m matrix such that m[i][j]==original[j][i]
		///
		/// @return Transposed matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<H,W,T> t() const;

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
		/// @brief Multiply by a matrix from the left
		/// 
		/// a.preMul(b)  equals to   a = b*a;
		///
		/// @param mat The matrix to multiply by
		///
		/// @return Product of the multiplication
		///
		/////////////////////////////////////////////////////////////
		template<Size H2>
		reference preMul(const matrix<H,H2,T> &mat);

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
		template<Size W2,Size H2,class T2>
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
		template<Size W2,Size H2>
		matrix<W2,H2,T> convert() const;

		/////////////////////////////////////////////////////////////
		/// @brief Calculates the <a href="http://en.wikipedia.org/wiki/Determinant">determinant</a> of a square matrix
		///
		/// It is a slow operation threfore you should implement your own
		/// method to count the determinant if you plan to use it frequently
		///
		/// @return The determinant
		///
		/////////////////////////////////////////////////////////////
		T det() const;

		/////////////////////////////////////////////////////////////
		/// @brief Calculates the matrix of minors see <a href="http://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html">here</a>
		///
		/// @return The matrix of minors
		///
		/////////////////////////////////////////////////////////////
		matrix<W, H, T> minors() const;

		/////////////////////////////////////////////////////////////
		/// @brief Calculates the <a href="http://en.wikipedia.org/wiki/Adjugate_matrix">adjugate</a> of a matrix
		///
		/// @return The adjugate
		///
		/////////////////////////////////////////////////////////////
		matrix<W, H, T> adjugate() const;

		/////////////////////////////////////////////////////////////
		/// @brief Calculates the <a href="http://en.wikipedia.org/wiki/Invertible_matrix">inverse</a> of a matrix
		///
		/// @return The inverse
		///
		/////////////////////////////////////////////////////////////
		matrix<W, W, T> inverse() const;

		/////////////////////////////////////////////////////////////
		/// @brief Alias to the inverse
		///
		/// @return The inverse
		///
		/////////////////////////////////////////////////////////////
		matrix<W, W, T> i() const {return inverse();};

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the <a href="http://en.wikipedia.org/wiki/Trace_%28linear_algebra%29">trace</a> of a matrix
		///
		/// @return The trace
		///
		/////////////////////////////////////////////////////////////
		T trace() const;

		/////////////////////////////////////////////////////////////
		/// @brief Access(rw) a column of the matrix
		///
		/// @param index Index of the row
		///
		/// @return Pointer to the first element of the @a index -th row
		///
		/////////////////////////////////////////////////////////////
		std::array<T,H> &operator[](Size index);

		/////////////////////////////////////////////////////////////
		/// @brief Access (r only) a row of the matrix
		///
		/// @param index Index of the row
		///
		/// @return Pointer to the first element of the @a index -th row
		///
		/////////////////////////////////////////////////////////////
		const std::array<T,H> &operator[](Size index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Apply a function to all the elements of the matrix
		///
		/// @param fun The function to apply
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		matrix<W,H,T> &operator()(fm::Delegate<T,T,vec2i> fun);

		/////////////////////////////////////////////////////////////
		/// @brief Apply a function to all the elements of the matrix
		///
		/// @param fun The function to apply
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		matrix<W,H,T> &apply(fm::Delegate<T,T,vec2i> fun);

		/////////////////////////////////////////////////////////////
		/// @brief Fold (reduce) the matrix in row-major order
		///
		/// @param ffun The folding funciton
		/// @param init The initial value of the fold
		///
		/// @return The result of the folding
		///
		/////////////////////////////////////////////////////////////
		T fold(fm::Delegate<T,T,T> ffun,T init = T()) const;

		/////////////////////////////////////////////////////////////
		/// @brief The <a href="http://en.wikipedia.org/wiki/Identity_matrix">identity matrix</a>
		///
		/// 1 0 . . . 0  <br/>
		/// 0 1 . . . 0  <br/>
		/// . . .     .  <br/>
		/// . .   .   .  <br/>
		/// . .     . .  <br/>
		/// 0 0 . . . 1  <br/>
		///
		/// Its [i][j] element is 1 if i==j otherwise 0
		///
		/////////////////////////////////////////////////////////////
		static matrix<W,H,T> identity;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a rectangle shaped partition (slice) of the matrxi
		///
		/// @return The silce of matrix, copied
		///
		/////////////////////////////////////////////////////////////
		template<Size W2,Size H2=W2,Size X=0,Size Y=0>
		matrix<W2,H2,T> slice() const;
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
	template<Size W,Size H,class T,class T2>
	auto operator*(const T &left,const matrix<W,H,T2> &right) -> matrix<W,H,decltype(left*right[0][0])>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (matrix)
	/// @param right Right operand (scalar)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,class T2>
	auto operator*(const matrix<W,H,T> &left,const T2 &right) -> matrix<W,H,decltype(left[0][0]*right)>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator *
	///
	/// @param left Left operand (matrix)
	/// @param right Right operand (scalar)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,class T2>
	auto operator/(const matrix<W,H,T> &left,const T2 &right) -> matrix<W,H,decltype(left[0][0]/right)>;

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
	template<Size W,Size H,class T,Size H2,class T2>
	auto operator*(const matrix<W,H,T> &left,const matrix<H,H2,T2> &right) -> matrix<W,H2,decltype(left[0][0]*right[0][0])>;

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
	template<Size W,Size H,class T,class T2>
	auto operator+(const matrix<W,H,T> &left,const matrix<W,H,T2> &right) -> matrix<W,H,decltype(left[0][0]+right[0][0])>;

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
	template<Size W,Size H,class T,class T2>
	auto operator-(const matrix<W,H,T> &left,const matrix<W,H,T2> &right) -> matrix<W,H,decltype(left[0][0]-right[0][0])>;

	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of unary operator -
	///
	/// @param mat Matrix to negate
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	auto operator-(const matrix<W,H,T> &mat) -> matrix<W,H,decltype(-mat[0][0])>;

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
	template<Size W,Size H,class T,class T2>
	matrix<W,H,T> &operator+=(matrix<W,H,T> &left,const matrix<W,H,T2> &right);

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
	template<Size W,Size H,class T,class T2>
	matrix<W,H,T> &operator-=(matrix<W,H,T> &left,const matrix<W,H,T2> &right);

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
	template<Size W,Size H,class T,class T2>
	matrix<W,H,T> &operator*=(matrix<W,H,T> &left,const matrix<H,H,T2> &right);

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
	template<Size W ,Size H ,class T,
             Size W2,Size H2,class T2,class K = T,class Eq = T,class = typename std::enable_if<W == W2 && H == H2,K>::type >
	bool operator==(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right);

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
	template<Size W ,Size H ,class T,
             Size W2,Size H2,class T2,class K = T,class = typename std::enable_if<W != W2 || H != H2,K>::type >
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
	template<Size W ,Size H ,class T,
             Size W2,Size H2,class T2,class K = T,class Eq = T,class = typename std::enable_if<W == W2 && H == H2,K>::type>
	bool operator!=(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right);

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
	template<Size W ,Size H ,class T,
             Size W2,Size H2,class T2,class K = T,class = typename std::enable_if<W != W2 || H != H2,K>::type>
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
	template<class T,class T2>
	auto operator*(const matrix<2,2,T> &mat,const vector2<T2> &vec) -> vector2<decltype(mat[0][0]*vec[0])>;

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
	template<class T,class T2>
	auto operator*(const matrix<3,3,T> &mat,const vector3<T2> &vec) -> vector3<decltype(mat[0][0]*vec[0])>;

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
	template<class T,class T2>
	auto operator*(const matrix<4,4,T> &mat,const vector4<T2> &vec) -> vector4<decltype(mat[0][0]*vec[0])>;
	
	/////////////////////////////////////////////////////////////
	/// @relates fm::matrix
	/// @brief Overload of binary operator | as concatenation
	///
	/// Matrices of argument must agree on height
	///
	/// @param mat Left operand (matrix)
	/// @param vec Right operand (vector)
	///
	/// @return Result of the operation
	///
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,Size H2,class T>
	matrix<W,H+H2,T> operator|(const matrix<W,H,T> &m1,const matrix<W,H2,T> &m2);
	
	
	

	/////////////////////////////////////////////////////////////
	///
	/// @brief Namespace used to hold functions that operate on square matrices
	///
	/////////////////////////////////////////////////////////////
	namespace MATRIX
	{
		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Concatenate two matrices of same height
		///
		/// @param m1 The left matrix of the concatenation
		/// @param m2 The right matrix of the concatenation
		///
		/// @return The resulting matrix
		///
		/////////////////////////////////////////////////////////////
		template<Size W,Size H,Size H2,class T>
		matrix<W,H+H2,T> concatH(const matrix<W,H,T> &m1,const matrix<W,H2,T> &m2);
		
		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Concatenate two matrices of same width
		///
		/// @param m1 The upper matrix of the concatenation
		/// @param m2 The lower matrix of the concatenation
		///
		/// @return The resulting matrix
		///
		/////////////////////////////////////////////////////////////
		template<Size W,Size W2,Size H,class T>
		matrix<W+W2,H,T> concatW(const matrix<W,H,T> &m1,const matrix<W2,H,T> &m2);
		
		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Translation_%28geometry%29#Matrix_representation">translation matrix</a>
		///
		/// @param translate The offset of the translation
		///
		/// @return The translation matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> translation(vec3 translate);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Translation_%28geometry%29#Matrix_representation">translation matrix</a>
		///
		/// @param x The x offset of the translation
		/// @param y The y offset of the translation
		///
		/// @return The translation matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> translation(float x,float y);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Translation_%28geometry%29#Matrix_representation">translation matrix</a>
		///
		/// @param x The x offset of the translation
		/// @param y The y offset of the translation
		/// @param z The z offset of the translation
		///
		/// @return The translation matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> translation(float x,float y,float z);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// @param scale The x and y component (z=1) of the the scaling
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> scaling(vec2 scale);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// @param scale The x, y and z component of the the scaling
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> scaling(vec3 scale);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// @param x The x component of the scaling
		/// @param y The y component of the scaling
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> scaling(float x,float y);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Scaling_%28geometry%29#Matrix_representation">scaling matrix</a>
		///
		/// @param x The x component of the scaling
		/// @param y The y component of the scaling
		/// @param z The z component of the scaling
		///
		/// @return The scaling matrix
		///
		/////////////////////////////////////////////////////////////
        matrix<4,4,float> scaling(float x,float y,float z);

		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://en.wikipedia.org/wiki/Rotation_matrix#Basic_rotations">rotation matrix</a> around the Z axis
		///
		/// For rotation with axis other than the Z axis use Quaternions
		/// @see fm::Quat
		///
		/// @param angle The angle of the rotation
		///
		/// @return The rotation matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> rotation(const Angle<float> &angle);


		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a <a href="http://www.scratchapixel.com/lessons/3d-advanced-lessons/perspective-and-orthographic-projection-matrix/perspective-projection-matrix/">perspective projection matrix</a>
		///
		/// @param fieldOfView <a href="http://en.wikipedia.org/wiki/Field_of_view_in_video_games">Field of view</a>
		/// @param aspect The ratio of the window width and height
		/// @param Znear The distance of the near clipping plane
		/// @param Zfar The distance of the far clipping plane
		///
		/// @return The perspective matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> perspective(const Angle<float> &fieldOfView,float aspect,float Znear = .1,float Zfar = 100);

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
		///
		/// @return The orthogonal projection matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> ortho(float left,float bottom,float right,float top,float nearVal = -1,float farVal = 1);


		/////////////////////////////////////////////////////////////
		/// @relates fm::matrix
		/// @brief Calculate a look-at view matrix
		///
		/// This is a combination of a translation and a rotation matrix <br/>
		/// see how glu <a href="http://www.opengl.org/wiki/GluLookAt_code">implemented</a> this function
		///
		/// @param cam_pos Position of the viewer
		/// @param target_pos Position of the objet where the camera is loooking
		///
		/// @return The look-at matrix
		///
		/////////////////////////////////////////////////////////////
		matrix<4,4,float> lookAt(vec3 cam_pos,vec3 target_pos);

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
		///
		/// @return The look-at matrix
		///
		/////////////////////////////////////////////////////////////
        matrix<4,4,float> lookAt(vec3 cam_pos,vec3 target_pos,vec3 up_dir);
	}
	using namespace MATRIX;
	
	template<Size W,Size H,class T>
	const bool matrix<W,H,T>::isSquare = (W == H);

	typedef matrix<2,2,float> mat2f;
	typedef matrix<2,2,float> mat2;
	typedef matrix<2,2,long>  mat2u;
	typedef matrix<2,2,int>   mat2i;

	typedef matrix<3,3,float> mat3f;
	typedef matrix<3,3,float> mat3;
	typedef matrix<3,3,long>  mat3u;
	typedef matrix<3,3,int>   mat3i;

	typedef matrix<4,4,float> mat4f;
	typedef matrix<4,4,float> mat4;
	typedef matrix<4,4,long>  mat4u;
	typedef matrix<4,4,int>   mat4i;
}

#endif //FRONTIER_MATRIX_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Matrix.inl>
#endif
