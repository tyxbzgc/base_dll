//
// typedef.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2009, GuangZhu Wu  <guangzhuwu@gmail.com>
//
//This program is free software; you can redistribute it and/or modify it 
//under the terms of the GNU General Public License or any later version.
//
//This program is distributed in the hope that it will be useful, but 
//WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
//or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
//for more details.
//
//You should have received a copy of the GNU General Public License along 
//with this program; if not, contact <guangzhuwu@gmail.com>.
//
//#ifndef P2ENGINE_TYPEDEF_HPP
//#define P2ENGINE_TYPEDEF_HPP
#pragma once
//#include <boost/cstdint.hpp>
//
//#ifndef USING_BOOST_INTTYPE
//#define 	USING_BOOST_INTTYPE		//类型定义作为一个整体，可在此添加
//		typedef boost::int64_t  int64;\
//		typedef boost::uint64_t uint64;\
//		typedef boost::int32_t  int32;\
//		typedef boost::uint32_t uint32;\
//		typedef boost::uint32_t uint32_t;\
//		typedef boost::int16_t  int16;\
//		typedef boost::uint16_t uint16;\
//		typedef boost::int8_t   int8;\
//		typedef boost::uint8_t  uint8;
//#endif
////inttype
//USING_BOOST_INTTYPE;

typedef __int64  int64;
typedef unsigned __int64 uint64;
typedef int  int32;
typedef unsigned int uint32;
typedef unsigned int uint32_t;
typedef short  int16;
typedef unsigned short uint16;
typedef char   int8;
typedef unsigned char  uint8;

#define MAX_UINT8		0xFF
#define MAX_INT8		0x0F
#define MAX_UINT16		0xFFFF
#define MAX_INT16		0x0FFF
#define MAX_UINT32		0xFFFFFFFF
#define MAX_INT32		0x0FFFFFFF
#define MAX_UINT64		0xFFFFFFFFFFFFFFFF
#define MAX_INT64		0x0FFFFFFFFFFFFFFF

//定义tstring
#ifndef tstring
#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif 
#endif 

#ifndef TString
#ifdef _UNICODE
#define TString std::wstring
#else
#define TString std::string
#endif 
#endif 