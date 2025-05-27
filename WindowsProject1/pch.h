
#ifndef PCH_H // PCH
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX 
#include <Windows.h> //window
#include "Math.h"

//Data Structure & Algorithm
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <queue>
#include <algorithm>

//C++ std
#include <iostream>
#include <memory>
#include <typeindex>
#include <functional>
#include <fstream>
#include <sstream>
#include <chrono>

#include <fmod.hpp> // Sound
#include <fmod_errors.h>
#pragma comment(lib, "../fMod/core/lib/x64/fmod_vc.lib")

#include <vfw.h> // Video
#pragma comment(lib, "vfw32.lib")

#include <d3d9.h> // DirectX
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h> // Debug
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif
#endif

//enum
enum class ObjectType : int
{
	Neutral,
	Back,
	Mid,
	Front,
	Effect,
	UI,
	Cursor,
	Count
};

enum class RenderType : int
{
	Rect,
	Ellipse,
};