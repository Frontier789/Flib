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
#ifndef FRONTIER_NEURALNET_HPP_INCLUDED
#define FRONTIER_NEURALNET_HPP_INCLUDED

#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Matrix.hpp>

#define FRONTIER_NEURALNET

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class implementing one layer of a Neural Network
	/// 
	/// The input layer needs not be treated as an actual layer
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T = float>
	class NeuralLayer
	{
	public:
		matrix<I,O,T> weights; ///< The weights of the neural "axons"
		matrix<1,O,T> biases;  ///< The bias values employed 
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// Initializes the weights and biases to random values
		/// 
		/////////////////////////////////////////////////////////////
		NeuralLayer();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the NN with given weight function
		/// 
		/// @param w The weight function
		/// 
		/////////////////////////////////////////////////////////////
		NeuralLayer(fm::Delegate<T,vec2i> w);
		
		mutable matrix<1,O,T> out; ///< The last output
		mutable matrix<1,I,T> in;  ///< The last input
		
		/////////////////////////////////////////////////////////////
		/// @brief Feed input through the layer
		/// 
		/// @param input The input to feed through
		/// 
		/////////////////////////////////////////////////////////////
		void feedForw(const matrix<1,I,T> &input) const;
		
		matrix<1,I,T> dE_dIn; ///< Derivate of the total output error with respect to the inputs 
		
		/////////////////////////////////////////////////////////////
		/// @brief Propagate the error term with given learning rate backwards
		/// 
		/// @param dE_dOut Derivate of the error function with respect to the output
		/// @param learn_rate The speed at which the NN learns (faster learning is less stable)
		/// 
		///////////////////////////////////////////////////////////// 
		void backProp(const matrix<1,O,T> &dE_dOut,T learn_rate);
	};

	/////////////////////////////////////////////////////////////
	/// @brief Class implementing standard Deep Feed Forward Neural Network with Back Propagation
	/// 
	/// The implementation considers all hidden layers to be of same size
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<Size I,Size N,Size H,Size O,class T = float>
	class NeuralNet
	{
		NeuralLayer<I,H,T> m_layerIn; ///< The input layer of the NN
		std::array<NeuralLayer<H,H,T>,N-1> m_layersHidden; ///< The array of hidden layers in the NN
		NeuralLayer<H,O,T> m_layerOut; ///< The output layer of the NN
	public:
		
		/////////////////////////////////////////////////////////////
		/// @brief Evaluate the Neural Network on a given input
		/// 
		/// @param input The input to evaluate on
		/// 
		/// @return The response of the NN
		/// 
		/////////////////////////////////////////////////////////////
		matrix<1,O,T> eval(const matrix<1,I,T> &input) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Use the NN as a Functor
		/// 
		/// @param input The input to evaluate on
		/// 
		/// @return The response of the NN
		/// 
		/////////////////////////////////////////////////////////////
		matrix<1,O,T> operator()(const matrix<1,I,T> &input) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Train the NN on an input with given rate
		/// 
		/// @param input The input to train on
		/// @param target The expected output on the input
		/// @param learning_rate The rate at which the NN learns from the input
		/// 
		/////////////////////////////////////////////////////////////
		void train(const matrix<1,I,T> &input,const matrix<1,O,T> &target,T learning_rate);
	};

	/////////////////////////////////////////////////////////////
	/// @brief Class implementing standard Deep Feed Forward Neural Network with Back Propagation
	/// 
	/// This partial specialization uses only a single layer of neurons
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<Size I,Size H,Size O,class T>
	class NeuralNet<I,0,H,O,T>
	{
		NeuralLayer<I,O,T> m_layer; ///< The single layer of the NN
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Evaluate the Neural Network on a given input
		/// 
		/// @param input The input to evaluate on
		/// 
		/// @return The response of the NN
		/// 
		/////////////////////////////////////////////////////////////
		matrix<1,O,T> eval(const matrix<1,I,T> &input) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Use the NN as a Functor
		/// 
		/// @param input The input to evaluate on
		/// 
		/// @return The response of the NN
		/// 
		/////////////////////////////////////////////////////////////
		matrix<1,O,T> operator()(const matrix<1,I,T> &input) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Train the NN on an input with given rate
		/// 
		/// @param input The input to train on
		/// @param target The expected output on the input
		/// @param learning_rate The rate at which the NN learns from the input
		/// 
		/////////////////////////////////////////////////////////////
		void train(const matrix<1,I,T> &input,const matrix<1,O,T> &target,T learning_rate);
	};
}
#endif //FRONTIER_NEURALNET_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/NeuralNet.inl>
#endif
