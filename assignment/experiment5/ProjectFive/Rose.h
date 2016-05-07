// Rose.h: interface for the CRose class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROSE_H__36999FF4_25D6_467F_AC6A_82AD25D7A4A9__INCLUDED_)
#define AFX_ROSE_H__36999FF4_25D6_467F_AC6A_82AD25D7A4A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "3DObject.h"

class CRose : public C3DObject  
{ 
public:
	CRose();
	virtual ~CRose();

	void ReadData();
	void Draw();
};

#endif // !defined(AFX_ROSE_H__36999FF4_25D6_467F_AC6A_82AD25D7A4A9__INCLUDED_)
