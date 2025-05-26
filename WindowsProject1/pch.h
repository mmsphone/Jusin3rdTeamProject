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
	Count
};

enum class RenderType : int
{
	Rect,
	Ellipse,
};