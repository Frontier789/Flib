////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/macros/OS.h>

#ifdef FRONTIER_OS_WINDOWS
#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>

#define FRONTIER_INVALID_SOCKET ((fn::SocketID)(INVALID_SOCKET))
#define FRONTIER_SOCKET_ERROR SOCKET_ERROR
#define FRONTIER_CLOSE_SOCKET(soc) closesocket(soc)
#endif


#ifdef FRONTIER_OS_LINUX
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define FRONTIER_INVALID_SOCKET ((fn::SocketID)(-1))
#define FRONTIER_SOCKET_ERROR -1
#define FRONTIER_CLOSE_SOCKET(soc) ::close(soc)
#endif
