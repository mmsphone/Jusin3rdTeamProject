#pragma once
#include "NoteYu.h"

class NoteMgr
{
private:
	NoteMgr();
	~NoteMgr();
public:

public:
	static NoteMgr* Get_Instance() {
		if (!m_pInstance) {
			m_pInstance = new NoteMgr;
		}
		return m_pInstance;
	}
	static void Destroy_Instance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
	


public:
	static NoteMgr* m_pInstance;
	std::vector<NoteYu>		vecNote;
	std::vector<NoteYu*>	vecActiveNote;
};

