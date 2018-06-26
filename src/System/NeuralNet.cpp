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
#include <FRONTIER/System/NeuralNet.hpp>
#include <fstream>
#include <iostream>
using namespace std;

namespace fm
{
	namespace priv
	{
		namespace {
			template<class T>
			inline void writeBin(std::ostream &out,const T &t)
			{
				out.write((const char *)&t,sizeof(T));
			}
			
			void writeLayer(std::ostream &out,const void *data,Size I,Size O,Size TS)
			{
				out.write((const char *)data,I*O*TS + 1*O*TS);
			}
			
			void readLayer(std::istream &in,void *data,Size I,Size O,Size TS)
			{
				in.read((char *)data,I*O*TS + 1*O*TS);
			}
			
			template<class T>
			inline fm::Result readBinCheck(std::istream &in,T t,const std::string &d)
			{
				T rd;
				in.read((char*)&rd,sizeof(T));
				if (rd != t) {
					return fm::Result("IOError",fm::Result::OPFailed,"NNParamMismatch","readBinCheck",__FILE__,__LINE__,d);
				}
				
				return fm::Result();
			}
		}
		////////////////////////////////////////////////////////////
		fm::Result save_NN_to_file(const std::string &f,Size I,Size N,Size H,Size O,Size TS,Size LayerSizeIH,Size LayerSizeHH,Size LayerSizeHO,const void *data)
		{
			std::ofstream out(f.c_str(),std::ios::binary);
			
			if (!out)
				return fm::Result("IOError",fm::Result::OPFailed,"FileNotWritable","save_NN_to_file",__FILE__,__LINE__,f);
			
			writeBin(out,'F');
			writeBin(out,'F');
			writeBin(out,'N');
			writeBin(out,'N');
			writeBin(out,I);
			writeBin(out,N);
			writeBin(out,H);
			writeBin(out,O);
			writeBin(out,TS);
			writeBin(out,LayerSizeIH);
			writeBin(out,LayerSizeHH);
			writeBin(out,LayerSizeHO);
			if (N == 0) {
				writeLayer(out,data,I,O,TS);
			} else {	
				writeLayer(out,data,I,H,TS); data = ((const char *)data) + LayerSizeIH;
				for (Size i=1;i<N;++i) {
					writeLayer(out,data,H,H,TS); data = ((const char *)data) + LayerSizeHH;
				}
				writeLayer(out,data,H,O,TS);
			}
			
			return fm::Result();
		}
		
		////////////////////////////////////////////////////////////
		fm::Result load_NN_from_file(const std::string &f,Size I,Size N,Size H,Size O,Size TS,Size LayerSizeIH,Size LayerSizeHH,Size LayerSizeHO,void *data)
		{
			std::ifstream in(f.c_str(),std::ios::binary);
			
			if (!in)
				return fm::Result("IOError",fm::Result::OPFailed,"FileNotReadable","load_NN_from_file",__FILE__,__LINE__,f);
			
			fm::Result res;
			
			res += readBinCheck(in,'F',"MIME_BYTE_0");
			res += readBinCheck(in,'F',"MIME_BYTE_1");
			res += readBinCheck(in,'N',"MIME_BYTE_2");
			res += readBinCheck(in,'N',"MIME_BYTE_3");
			res += readBinCheck(in,I,"INPUT_SIZE");
			res += readBinCheck(in,N,"HIDDEN_LAYER_COUNT");
			res += readBinCheck(in,H,"HIDDEN_LAYER_SIZE");
			res += readBinCheck(in,O,"OUTPUT_SIZE");
			res += readBinCheck(in,TS,"TYPE_SIZE");
			res += readBinCheck(in,LayerSizeIH,"");
			res += readBinCheck(in,LayerSizeHH,"");
			res += readBinCheck(in,LayerSizeHO,"");
			if (N == 0) {
				readLayer(in,data,I,O,TS);
			} else {
				readLayer(in,data,I,H,TS); data = ((char *)data) + LayerSizeIH;
				for (Size i=1;i<N;++i) {
					readLayer(in,data,H,H,TS); data = ((char *)data) + LayerSizeHH;
				}
				readLayer(in,data,H,O,TS);
			}
			
			return res;
		}
	}
}
