////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
#ifndef FRONTIER_THREAD_INL_INCLUDED
#define FRONTIER_THREAD_INL_INCLUDED

namespace fm
{
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		class ThreadFuntionCaller
		{
		public:
			fm::Thread *m_owner;
			fm::TlsPtr<fm::Thread> *m_currentThreadPtr;
			
			inline ThreadFuntionCaller(fm::Thread *m_owner) : m_owner(m_owner),
															  m_currentThreadPtr(&m_owner->m_currentThread)
			{
				
			}
			
			/// virtual destructor enables us to destruct the derived object from a ThreadFuntionCaller*
			inline virtual ~ThreadFuntionCaller() {}
			
			/// we'll implement this function in the templated sepcifications
			virtual void callFunc() = 0;
		};
		
		/////////////////////////////////////////////////////////////
		template<class R>
		class ParamStorage0 : public ThreadFuntionCaller
		{
			R (*m_func)();
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorage0(R (*m_func)(),fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																	  m_func(m_func)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				m_func();
			}
		};
		
		/////////////////////////////////////////////////////////////
		template<class R,class P1>
		class ParamStorage1 : public ThreadFuntionCaller
		{
			R (*m_func)(P1);
			P1 m_param1;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorage1(R (*m_func)(P1),P1 m_param1,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																					m_func(m_func),
																					m_param1(m_param1)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				m_func(m_param1);
			}
		};
		
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2>
		class ParamStorage2 : public ThreadFuntionCaller
		{
			R (*m_func)(P1,P2);
			P1 m_param1;
			P2 m_param2;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorage2(R (*m_func)(P1,P2),P1 m_param1,P2 m_param2,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																								   m_func(m_func),
																								   m_param1(m_param1),
																								   m_param2(m_param2)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				m_func(m_param1,m_param2);
			}
		};
		
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3>
		class ParamStorage3 : public ThreadFuntionCaller
		{
			R (*m_func)(P1,P2,P3);
			P1 m_param1;
			P2 m_param2;
			P3 m_param3;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorage3(R (*m_func)(P1,P2,P3),P1 m_param1,P2 m_param2,P3 m_param3,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																												  m_func(m_func),
																												  m_param1(m_param1),
																												  m_param2(m_param2),
																												  m_param3(m_param3)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				m_func(m_param1,m_param2,m_param3);
			}
		};
		
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3,class P4>
		class ParamStorage4 : public ThreadFuntionCaller
		{
			R (*m_func)(P1,P2,P3,P4);
			P1 m_param1;
			P2 m_param2;
			P3 m_param3;
			P4 m_param4;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorage4(R (*m_func)(P1,P2,P3,P4),P1 m_param1,P2 m_param2,P3 m_param3,P4 m_param4,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																																 m_func(m_func),
																																 m_param1(m_param1),
																																 m_param2(m_param2),
																																 m_param3(m_param3),
																																 m_param4(m_param4)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				m_func(m_param1,m_param2,m_param3,m_param4);
			}
		};
		
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3,class P4,class P5>
		class ParamStorage5 : public ThreadFuntionCaller
		{
			R (*m_func)(P1,P2,P3,P4,P5);
			P1 m_param1;
			P2 m_param2;
			P3 m_param3;
			P4 m_param4;
			P5 m_param5;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorage5(R (*m_func)(P1,P2,P3,P4,P5),P1 m_param1,P2 m_param2,P3 m_param3,P4 m_param4,P5 m_param5,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																																				m_func(m_func),
																																				m_param1(m_param1),
																																			    m_param2(m_param2),
																																			    m_param3(m_param3),
																																			    m_param4(m_param4),
																																			    m_param5(m_param5)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				m_func(m_param1,m_param2,m_param3,m_param4,m_param5);
			}
		};
		
		
		
		/////////////////////////////////////////////////////////////
		template<class R,class CLASS>
		class ParamStorageO0 : public ThreadFuntionCaller
		{
			R (CLASS::*m_func)();
			CLASS *m_object;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorageO0(R (CLASS::*m_func)(),CLASS *m_object,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																							  m_func(m_func),
																							  m_object(m_object)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				(m_object->*m_func)();
			}
		};
		
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class CLASS>
		class ParamStorageO1 : public ThreadFuntionCaller
		{
			R (CLASS::*m_func)(P1);
			P1 m_param1;
			CLASS *m_object;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorageO1(R (CLASS::*m_func)(P1),P1 m_param1,CLASS *m_object,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																										    m_func(m_func),
																										    m_param1(m_param1),
																											m_object(m_object)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				(m_object->*m_func)(m_param1);
			}
		};
		
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class CLASS>
		class ParamStorageO2 : public ThreadFuntionCaller
		{
			R (CLASS::*m_func)(P1,P2);
			P1 m_param1;
			P2 m_param2;
			CLASS *m_object;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorageO2(R (CLASS::*m_func)(P1,P2),P1 m_param1,P2 m_param2,CLASS *m_object,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																														   m_func(m_func),
																														   m_param1(m_param1),
																														   m_param2(m_param2),
																														   m_object(m_object)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				(m_object->*m_func)(m_param1,m_param2);
			}
		};
		
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3,class CLASS>
		class ParamStorageO3 : public ThreadFuntionCaller
		{
			R (CLASS::*m_func)(P1,P2,P3);
			P1 m_param1;
			P2 m_param2;
			P3 m_param3;
			CLASS *m_object;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorageO3(R (CLASS::*m_func)(P1,P2,P3),P1 m_param1,P2 m_param2,P3 m_param3,CLASS *m_object,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																																		  m_func(m_func),
																																		  m_param1(m_param1),
																																		  m_param2(m_param2),
																																		  m_param3(m_param3),
																																		  m_object(m_object)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				(m_object->*m_func)(m_param1,m_param2,m_param3);
			}
		};
		
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3,class P4,class CLASS>
		class ParamStorageO4 : public ThreadFuntionCaller
		{
			R (CLASS::*m_func)(P1,P2,P3,P4);
			P1 m_param1;
			P2 m_param2;
			P3 m_param3;
			P4 m_param4;
			CLASS *m_object;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorageO4(R (CLASS::*m_func)(P1,P2,P3,P4),P1 m_param1,P2 m_param2,P3 m_param3,P4 m_param4,CLASS *m_object,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																																						 m_func(m_func),
																																						 m_param1(m_param1),
																																						 m_param2(m_param2),
																																						 m_param3(m_param3),
																																						 m_param4(m_param4),
																																						 m_object(m_object)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				(m_object->*m_func)(m_param1,m_param2,m_param3,m_param4);
			}
		};
		
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3,class P4,class P5,class CLASS>
		class ParamStorageO5 : public ThreadFuntionCaller
		{
			R (CLASS::*m_func)(P1,P2,P3,P4,P5);
			P1 m_param1;
			P2 m_param2;
			P3 m_param3;
			P4 m_param4;
			P5 m_param5;
			CLASS *m_object;
		public:
			/////////////////////////////////////////////////////////////
			inline ParamStorageO5(R (CLASS::*m_func)(P1,P2,P3,P4,P5),P1 m_param1,P2 m_param2,P3 m_param3,P4 m_param4,P5 m_param5,CLASS *m_object,fm::Thread *m_owner) : ThreadFuntionCaller(m_owner),
																																										m_func(m_func),
																																										m_param1(m_param1),
																																										m_param2(m_param2),
																																										m_param3(m_param3),
																																										m_param4(m_param4),
																																										m_param5(m_param5),
																																										m_object(m_object)
			{
				
			}
			
			/////////////////////////////////////////////////////////////
			inline void callFunc()
			{
				(m_object->*m_func)(m_param1,m_param2,m_param3,m_param4,m_param5);
			}
		};
	}
		
	/////////////////////////////////////////////////////////////
	template<class R>
	bool Thread::create(R (*func)())
	{
		return init(new typename priv::ParamStorage0<R>(func,this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class P1>
	bool Thread::create(R (*func)(P1),P1 param1)
	{
		return init(new typename priv::ParamStorage1<R,P1>(func,param1,this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class P1,class P2>
	bool Thread::create(R (*func)(P1,P2),P1 param1,P2 param2)
	{
		return init(new typename priv::ParamStorage2<R,P1,P2>(func,param1,param2,this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class P1,class P2,class P3>
	bool Thread::create(R (*func)(P1,P2,P3),P1 param1,P2 param2,P3 param3)
	{
		return init(new typename priv::ParamStorage3<R,P1,P2,P3>(func,param1,param2,param3,this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class P1,class P2,class P3,class P4>
	bool Thread::create(R (*func)(P1,P2,P3,P4),P1 param1,P2 param2,P3 param3,P4 param4)
	{
		return init(new typename priv::ParamStorage4<R,P1,P2,P3,P4>(func,param1,param2,param3,param4,this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class P1,class P2,class P3,class P4,class P5>
	bool Thread::create(R (*func)(P1,P2,P3,P4,P5),P1 param1,P2 param2,P3 param3,P4 param4,P5 param5)
	{
		return init(new typename priv::ParamStorage5<R,P1,P2,P3,P4,P5>(func,param1,param2,param3,param4,param5,this));
	}
		
		
		
	/////////////////////////////////////////////////////////////
	template<class R,class CLASS>
	bool Thread::create(CLASS *object,R (CLASS::*func)())
	{
		return init(new priv::ParamStorageO0<R,CLASS>(func,object,this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class P1,class CLASS>
	bool Thread::create(CLASS *object,R (CLASS::*func)(P1),P1 param1)
	{
		return init(new typename priv::ParamStorageO1<R,P1,CLASS>(func,param1,object,this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class P1,class P2,class CLASS>
	bool Thread::create(CLASS *object,R (CLASS::*func)(P1,P2),P1 param1,P2 param2)
	{
		return init(new typename priv::ParamStorageO2<R,P1,P2,CLASS>(func,param1,param2,object,this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class P1,class P2,class P3,class CLASS>
	bool Thread::create(CLASS *object,R (CLASS::*func)(P1,P2,P3),P1 param1,P2 param2,P3 param3)
	{
		return init(new typename priv::ParamStorageO3<R,P1,P2,P3,CLASS>(func,param1,param2,param3,object,this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class P1,class P2,class P3,class P4,class CLASS>
	bool Thread::create(CLASS *object,R (CLASS::*func)(P1,P2,P3,P4),P1 param1,P2 param2,P3 param3,P4 param4)
	{
		return init(new typename priv::ParamStorageO4<R,P1,P2,P3,P4,CLASS>(func,param1,param2,param3,param4,object,this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class P1,class P2,class P3,class P4,class P5,class CLASS>
	bool Thread::create(CLASS *object,R (CLASS::*func)(P1,P2,P3,P4,P5),P1 param1,P2 param2,P3 param3,P4 param4,P5 param5)
	{
		return init(new typename priv::ParamStorageO5<R,P1,P2,P3,P4,P5,CLASS>(func,param1,param2,param3,param4,param5,object,this));
	}
}

#endif // FRONTIER_THREAD_INL_INCLUDED