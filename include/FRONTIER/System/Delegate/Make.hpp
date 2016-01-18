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
#define FRONTIER_DELEGATE_TEMPLATE_PARAMS ,class P0 = priv::NoClass,class P1 = priv::NoClass,class P2 = priv::NoClass,class P3 = priv::NoClass,class P4 = priv::NoClass,class P5 = priv::NoClass
#define FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL ,class P0,class P1,class P2,class P3,class P4,class P5
#define FRONTIER_DELEGATE_TEMPLATE_SPEC
#define FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ
#define FRONTIER_DELEGATE_TEMPLATE_LIST ,P0,P1,P2,P3,P4,P5
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONV P0,P1,P2,P3,P4,P5
#define FRONTIER_DELEGATE_CALL_PARAMS P0 p0,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5
#define FRONTIER_DELEGATE_CALL_LIST p0,p1,p2,p3,p4,p5

#define FRONTIER_DELEGATE_TEMPLATE_PARAMSc ,class P0c,class P1c,class P2c,class P3c,class P4c,class P5c
#define FRONTIER_DELEGATE_TEMPLATE_SPECc
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONVc P0c,P1c,P2c,P3c,P4c,P5c

#include FRONTIER_DELEGATE_TO_INCLUDE



#define FRONTIER_DELEGATE_TEMPLATE_PARAMS ,class P0,class P1,class P2,class P3,class P4
#define FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL ,class P0,class P1,class P2,class P3,class P4
#define FRONTIER_DELEGATE_TEMPLATE_SPEC <R,P0,P1,P2,P3,P4,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ <Object,R,P0,P1,P2,P3,P4,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_LIST ,P0,P1,P2,P3,P4
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONV P0,P1,P2,P3,P4
#define FRONTIER_DELEGATE_CALL_PARAMS P0 p0,P1 p1,P2 p2,P3 p3,P4 p4
#define FRONTIER_DELEGATE_CALL_LIST p0,p1,p2,p3,p4

#define FRONTIER_DELEGATE_TEMPLATE_PARAMSc ,class P0c,class P1c,class P2c,class P3c,class P4c
#define FRONTIER_DELEGATE_TEMPLATE_SPECc <Rc,P0c,P1c,P2c,P3c,P4c,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONVc P0c,P1c,P2c,P3c,P4c

#include FRONTIER_DELEGATE_TO_INCLUDE



#define FRONTIER_DELEGATE_TEMPLATE_PARAMS ,class P0,class P1,class P2,class P3
#define FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL ,class P0,class P1,class P2,class P3
#define FRONTIER_DELEGATE_TEMPLATE_SPEC <R,P0,P1,P2,P3,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ <Object,R,P0,P1,P2,P3,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_LIST ,P0,P1,P2,P3
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONV P0,P1,P2,P3
#define FRONTIER_DELEGATE_CALL_PARAMS P0 p0,P1 p1,P2 p2,P3 p3
#define FRONTIER_DELEGATE_CALL_LIST p0,p1,p2,p3

#define FRONTIER_DELEGATE_TEMPLATE_PARAMSc ,class P0c,class P1c,class P2c,class P3c
#define FRONTIER_DELEGATE_TEMPLATE_SPECc <Rc,P0c,P1c,P2c,P3c,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONVc P0c,P1c,P2c,P3c

#include FRONTIER_DELEGATE_TO_INCLUDE



#define FRONTIER_DELEGATE_TEMPLATE_PARAMS ,class P0,class P1,class P2
#define FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL ,class P0,class P1,class P2
#define FRONTIER_DELEGATE_TEMPLATE_SPEC <R,P0,P1,P2,priv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ <Object,R,P0,P1,P2,priv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_LIST ,P0,P1,P2
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONV P0,P1,P2
#define FRONTIER_DELEGATE_CALL_PARAMS P0 p0,P1 p1,P2 p2
#define FRONTIER_DELEGATE_CALL_LIST p0,p1,p2

#define FRONTIER_DELEGATE_TEMPLATE_PARAMSc ,class P0c,class P1c,class P2c
#define FRONTIER_DELEGATE_TEMPLATE_SPECc <Rc,P0c,P1c,P2c,priv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONVc P0c,P1c,P2c

#include FRONTIER_DELEGATE_TO_INCLUDE



#define FRONTIER_DELEGATE_TEMPLATE_PARAMS ,class P0,class P1
#define FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL ,class P0,class P1
#define FRONTIER_DELEGATE_TEMPLATE_SPEC <R,P0,P1,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ <Object,R,P0,P1,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_LIST ,P0,P1
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONV P0,P1
#define FRONTIER_DELEGATE_CALL_PARAMS P0 p0,P1 p1
#define FRONTIER_DELEGATE_CALL_LIST p0,p1

#define FRONTIER_DELEGATE_TEMPLATE_PARAMSc ,class P0c,class P1c
#define FRONTIER_DELEGATE_TEMPLATE_SPECc <Rc,P0c,P1c,priv::NoClasspriv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONVc P0c,P1c

#include FRONTIER_DELEGATE_TO_INCLUDE



#define FRONTIER_DELEGATE_TEMPLATE_PARAMS ,class P0
#define FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL ,class P0
#define FRONTIER_DELEGATE_TEMPLATE_SPEC <R,P0,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ <Object,R,P0,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_LIST ,P0
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONV P0
#define FRONTIER_DELEGATE_CALL_PARAMS P0 p0
#define FRONTIER_DELEGATE_CALL_LIST p0

#define FRONTIER_DELEGATE_TEMPLATE_PARAMSc ,class P0c
#define FRONTIER_DELEGATE_TEMPLATE_SPECc <Rc,P0c,priv::NoClass,priv::NoClasspriv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONVc P0c

#include FRONTIER_DELEGATE_TO_INCLUDE



#define FRONTIER_DELEGATE_TEMPLATE_PARAMS
#define FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL
#define FRONTIER_DELEGATE_TEMPLATE_SPEC <R,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ <Object,R,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_LIST
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONV
#define FRONTIER_DELEGATE_CALL_PARAMS
#define FRONTIER_DELEGATE_CALL_LIST

#define FRONTIER_DELEGATE_TEMPLATE_PARAMSc
#define FRONTIER_DELEGATE_TEMPLATE_SPECc <Rc,priv::NoClass,priv::NoClass,priv::NoClasspriv::NoClass,priv::NoClass,priv::NoClass>
#define FRONTIER_DELEGATE_TEMPLATE_CALL_CONVc

#include FRONTIER_DELEGATE_TO_INCLUDE
