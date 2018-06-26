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
	inline T activation(T x)
	{
		//return std::atan(x);
		return 1.0 / (1.0 + std::exp(-x));
		//return std::max<T>(x,0);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline T dactivation(T x)
	{
		//return 1/(x*x+1); 
		return activation(x) * (1-activation(x));
		//return x > 0 ? 1 : 0;
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T>
	inline NeuralLayer<I,O,T>::NeuralLayer() : weights(0), biases(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T>
	inline NeuralLayer<I,O,T>::NeuralLayer(fm::Delegate<T,vec2i> w) 
	{
		fm::Delegate<T,T,vec2i> wf = [w](T,vec2i v){return w(v);};
		
		setWeights(wf);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T>
	inline void NeuralLayer<I,O,T>::setWeights(fm::Delegate<T,T,vec2i> w)
	{
		weights.apply(w);
		biases.apply(w);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T>
	inline void NeuralLayer<I,O,T>::feedForw(const matrix<1,I,T> &input) const
	{
		in  = input;
		out = (input * weights + biases)(activation<T>);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T>
	inline void NeuralLayer<I,O,T>::backProp(const matrix<1,O,T> &dE_dOut,T learn_rate)
	{
		matrix<1,O,T> dOut_dNet = (in * weights + biases).apply(dactivation<T>);
		
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
	template<Size I,Size O,class T>
	inline void NeuralLayer<I,O,T>::mutate(T rate,fm::Delegate<T> rnd)
	{
		fm::Delegate<T,vec2i> rnd2 = rnd;
		
		weights += matrix<I,O,T>(rnd2)*2*rate - matrix<I,O,T>(rate);
		biases  += matrix<1,O,T>(rnd2)*2*rate - matrix<1,O,T>(rate);
	}
	
	namespace {
		template<Size A,Size B,class T>
		void matrix_cross(matrix<A,B,T> &child,const matrix<A,B,T> &p1,const matrix<A,B,T> &p2,fm::Delegate<T> rnd)
		{
			Cx(A)Cy(B) {
				T r = rnd();
				child[x][y] = p1[x][y] * r + p2[x][y] * (1-r);
			}
		}
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size O,class T>
	void NeuralLayer<I,O,T>::cross(NeuralLayer<I,O,T> &child,const NeuralLayer<I,O,T> &parent1,const NeuralLayer<I,O,T> &parent2,fm::Delegate<T> rnd)
	{
		matrix_cross(child.weights,parent1.weights,parent2.weights,rnd);
		matrix_cross(child.biases,parent1.biases,parent2.biases,rnd);
	}
		
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size N,Size H,Size O,class T>
	inline NeuralNet<I,N,H,O,T>::NeuralNet() :
		m_mt(std::random_device()() + 42.69),
		m_dst(0,1)
	{
		fm::Delegate<T> rnd = [&](){return m_dst(m_mt);};
		
		m_layerIn.setWeights(rnd);
		m_layerOut.setWeights(rnd);
		
		for (auto &layer : m_layersHidden)
			layer.setWeights(rnd);
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
	template<Size I,Size N,Size H,Size O,class T>
	inline void NeuralNet<I,N,H,O,T>::mutate(T rate)
	{
		fm::Delegate<T> rnd = [&](){return m_dst(m_mt);};
		
		m_layerIn.mutate(rate,rnd);
		for (auto &layer : m_layersHidden) layer.mutate(rate,rnd);
		m_layerOut.mutate(rate,rnd);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size N,Size H,Size O,class T>
	void NeuralNet<I,N,H,O,T>::cross(NeuralNet<I,N,H,O,T> &child,const NeuralNet<I,N,H,O,T> &parent1,const NeuralNet<I,N,H,O,T> &parent2)
	{
		fm::Delegate<T> rnd = [&](){return child.m_dst(child.m_mt);};
		
		decltype(child.m_layerIn)::cross(child.m_layerIn,parent1.m_layerIn,parent2.m_layerIn,rnd);
		
		C(N-1)
			NeuralLayer<H,H,T>::cross(child.m_layersHidden[i],parent1.m_layersHidden[i],parent2.m_layersHidden[i],rnd);
		
		decltype(child.m_layerOut)::cross(child.m_layerOut,parent1.m_layerOut,parent2.m_layerOut,rnd);
	}
	
	namespace priv
	{
		fm::Result save_NN_to_file(const std::string &f,Size I,Size N,Size H,Size O,Size TS,Size LayerSizeIH,Size LayerSizeHH,Size LayerSizeHO,const void *data);
		fm::Result load_NN_from_file(const std::string &f,Size I,Size N,Size H,Size O,Size TS,Size LayerSizeIH,Size LayerSizeHH,Size LayerSizeHO,void *data);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size N,Size H,Size O,class T>
	fm::Result NeuralNet<I,N,H,O,T>::saveToFile(const std::string &file) const
	{
		return priv::save_NN_to_file(file,I,N,H,O,sizeof(T),sizeof(m_layerIn),sizeof(m_layersHidden[0]),sizeof(m_layerOut),(const void*)&m_layerIn);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size N,Size H,Size O,class T>
	fm::Result NeuralNet<I,N,H,O,T>::loadFromFile(const std::string &file) const
	{
		return priv::load_NN_from_file(file,I,N,H,O,sizeof(T),sizeof(m_layerIn),sizeof(m_layersHidden[0]),sizeof(m_layerOut),(void*)&m_layerIn);
	}
	
	
	
	
	/////////////////////////////////////////////////////////////
	template<Size I,Size H,Size O,class T>
	inline NeuralNet<I,0,H,O,T>::NeuralNet() :
		m_mt(std::random_device()() + 42.69),
		m_dst(0,1)
	{
		fm::Delegate<T> rnd = [&](){return m_dst(m_mt);};
		
		m_layer.setWeights(rnd);
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
		
	/////////////////////////////////////////////////////////////
	template<Size I,Size H,Size O,class T>
	inline void NeuralNet<I,0,H,O,T>::mutate(T rate)
	{
		fm::Delegate<T> rnd = [&](){return m_dst(m_mt);};
		
		m_layer.mutate(rate,rnd);
	}
}

#endif // FRONTIER_NEURALNET_INL_INCLUDED
