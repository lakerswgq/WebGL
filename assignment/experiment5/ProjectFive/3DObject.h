<<<<<<< HEAD
// 3DObject.h: interface for the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DOBJECT_H__1782FD41_CE26_46A8_9630_05CB026AB512__INCLUDED_)
#define AFX_3DOBJECT_H__1782FD41_CE26_46A8_9630_05CB026AB512__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "glm.h"

class C3DObject  
{
public:
	C3DObject();
	virtual ~C3DObject();

	virtual void Draw() {};
protected:
	GLMmodel* m_pModel;

};

#endif // !defined(AFX_3DOBJECT_H__1782FD41_CE26_46A8_9630_05CB026AB512__INCLUDED_)
=======
// 3DObject.h: interface for the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DOBJECT_H__1782FD41_CE26_46A8_9630_05CB026AB512__INCLUDED_)
#define AFX_3DOBJECT_H__1782FD41_CE26_46A8_9630_05CB026AB512__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "glm.h"

class C3DObject  
{
public:
	C3DObject();
	virtual ~C3DObject();

	virtual void Draw() {};
protected:
	GLMmodel* m_pModel;

};

#endif // !defined(AFX_3DOBJECT_H__1782FD41_CE26_46A8_9630_05CB026AB512__INCLUDED_)
>>>>>>> dd82df2a6f75602c93d298985876b7aa6a44ba65
