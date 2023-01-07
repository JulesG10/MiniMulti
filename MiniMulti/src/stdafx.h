#pragma once

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<ostream>
#include<fstream>
#include<sstream> 
#include <chrono>
#include<mutex>
#include<string>
#include<map>
#include<unordered_map>

#include<raylib.h>
#include<rlgl.h>
#include<raymath.h>

#if defined(_WIN32)           
#define NOGDI
#define NOUSER
#endif
#include <CNetClient.h>
#include <CNetServer.h>
using namespace cnet;
#if defined(_WIN32)        
#undef near
#undef far
#endif

#include "netdata.h"