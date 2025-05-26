// pch.h: �̸� �����ϵ� ��� �����Դϴ�.
// �Ʒ� ������ ������ �� ���� �����ϵǾ�����, ���� ���忡 ���� ���� ������ ����մϴ�.
// �ڵ� ������ �� ���� �ڵ� �˻� ����� �����Ͽ� IntelliSense ���ɿ��� ������ ��Ĩ�ϴ�.
// �׷��� ���⿡ ������ ������ ���� �� ������Ʈ�Ǵ� ��� ��� �ٽ� �����ϵ˴ϴ�.
// ���⿡ ���� ������Ʈ�� ������ �߰����� ������. �׷��� ������ ���ϵ˴ϴ�.

#ifndef PCH_H
#define PCH_H

// ���⿡ �̸� �������Ϸ��� ��� �߰�
#define NOMINMAX
#include <Windows.h>
#include "Math.h"

//Data Structure & Algorithm
#include <list>
#include <vector>
#include <unordered_map>
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

#include <fmod.hpp>
#include <fmod_errors.h>
#pragma comment(lib, "../fMod/core/lib/x64/fmod_vc.lib")

#include <vfw.h>
#pragma comment(lib, "vfw32.lib")

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif


#endif //PCH_H

enum class ObjectType : int
{
	Neutral,
	Alliance,
	Player,
	Enemy,
	ABullet,
	PBullet,
	EBullet,
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