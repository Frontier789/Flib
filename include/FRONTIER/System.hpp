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
#ifndef FRONTIER_SYSTEM_MODULE_HPP_INCLUDED
#define FRONTIER_SYSTEM_MODULE_HPP_INCLUDED

////////////////////////////////////////////////////////////
/// @defgroup System System module
///
/// This module defines <a href="http://en.wikipedia.org/wiki/Euclidean_vector">euclidean vector</a>,
///						<a href="http://en.wikipedia.org/wiki/Matrix_%28mathematics%29">matrix</a>,
///						<a href="http://en.wikipedia.org/wiki/Quaternion">quaternion</a>,
///						 angle, time, clock, rectangle, vertex, nonCopyable and other classes
///
////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
/// @brief Namespace used to hold mathematical classes
///
/////////////////////////////////////////////////////////////
namespace fm
{

}

#include <FRONTIER/System/Ice.hpp>
#include <FRONTIER/System/Ref.hpp>
#include <FRONTIER/System/Box.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/System/Time.hpp>
#include <FRONTIER/System/Math.hpp>
#include <FRONTIER/System/Clock.hpp>
#include <FRONTIER/System/Sleep.hpp>
#include <FRONTIER/System/Angle.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Camera.hpp>
#include <FRONTIER/System/Polar2.hpp>
#include <FRONTIER/System/Polar3.hpp>
#include <FRONTIER/System/KDTree.hpp>
#include <FRONTIER/System/Random.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/CmdArgs.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/NeuralNet.hpp>
#include <FRONTIER/System/Quaternion.hpp>
#include <FRONTIER/System/Transition.hpp>
#include <FRONTIER/System/BezierCurve.hpp>
#include <FRONTIER/System/MatrixStack.hpp>
#include <FRONTIER/System/HeavyToCopy.hpp>

#endif // FRONTIER_SYSTEM_MODULE_HPP_INCLUDED
