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
#ifndef FRONTIER_NEURALNET_INL_INCLUDED
#define FRONTIER_NEURALNET_INL_INCLUDED
#include <random>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T>
	inline T sigmoid(T x)
	{
		return 1.0/(1.0 + exp(-x));
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline T dsigmoidds(T sx)
	{
		return sx*(1-sx);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T>
	inline NeuralLayer<I,O,T>::NeuralLayer()
	{
		std::random_device rd;
		std::mt19937_64 mt(rd() + 42.69);
		std::uniform_real_distribution<double> dst(0,1);
		
		auto w = [&](){return dst(mt);};
		
		weights.apply(w);
		biases.apply(w);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T>
	inline NeuralLayer<I,O,T>::NeuralLayer(fm::Delegate<T,vec2i> w) : weights(w), biases(w) {}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T>
	inline void NeuralLayer<I,O,T>::feedForw(const matrix<1,I,T> &input) const
	{
		in  = input;
		out = (input * weights + biases)(sigmoid<T>);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T>
	inline void NeuralLayer<I,O,T>::backProp(const matrix<1,O,T> &dE_dOut,T learn_rate)
	{
		matrix<1,O,T> dOut_dNet = out.apply(dsigmoidds<T>);
		
		const auto &dNet_dw = in;
		matrix<1,O,T> dNet_db(1);
		
		matrix<1,O,T> dE_dNet = dE_dOut.byComp(dOut_dNet);
		
		matrix<I,O,T> dE_dw = (dE_dNet.t() * dNet_dw).t();
		matrix<1,O,T> dE_db = dE_dNet.byComp(dNet_db);
		
		dE_dIn = dE_dNet * weights.t();
		
		weights -= dE_dw * learn_rate;
		biases  -= dE_db * learn_rate;
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size N,Size H,Size O,class T>
	inline matrix<1,O,T> NeuralNet<I,N,H,O,T>::eval(const matrix<1,I,T> &input) const
	{
		m_layerIn.feedForw(input);
		
		const matrix<1,H,T> *lastOut = &m_layerIn.out;
		for (const auto &layer : m_layersHidden) {
			layer.feedForw(*lastOut);
			lastOut = &layer.out;
		}
		
		m_layerOut.feedForw(*lastOut);
		
		return m_layerOut.out;
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size N,Size H,Size O,class T>
	inline matrix<1,O,T> NeuralNet<I,N,H,O,T>::operator()(const matrix<1,I,T> &input) const
	{
		return eval(input);
	}
		
	/////////////////////////////////////////////////////////////
	template<Size I,Size N,Size H,Size O,class T>
	inline void NeuralNet<I,N,H,O,T>::train(const matrix<1,I,T> &input,const matrix<1,O,T> &target,T learning_rate)
	{
		matrix<1,O,T> output = eval(input);
		
		m_layerOut.backProp(output - target,learning_rate);
		
		const matrix<1,H,T> *last_dE_dIn = &m_layerOut.dE_dIn;
		for (auto it = m_layersHidden.rbegin();it != m_layersHidden.rend();++it) {
			it->backProp(*last_dE_dIn,learning_rate);
			last_dE_dIn = &it->dE_dIn;
		}
		
		m_layerIn.backProp(*last_dE_dIn,learning_rate);
	}

	/////////////////////////////////////////////////////////////
	template<Size I,Size H,Size O,class T>
	inline matrix<1,O,T> NeuralNet<I,0,H,O,T>::eval(const matrix<1,I,T> &input) const
	{
		m_layer.feedForw(input);
		
		return m_layer.out;
	}

	/////////////////////////////////////////////////////////////
	template<Size I,Size H,Size O,class T>
	inline matrix<1,O,T> NeuralNet<I,0,H,O,T>::operator()(const matrix<1,I,T> &input) const
	{
		return eval(input);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size H,Size O,class T>
	inline void NeuralNet<I,0,H,O,T>::train(const matrix<1,I,T> &input,const matrix<1,O,T> &target,T learning_rate)
	{
		matrix<1,O,T> output = eval(input);
		
		m_layer.backProp(output - target,learning_rate);
	}
}

#endif // FRONTIER_NEURALNET_INL_INCLUDED
