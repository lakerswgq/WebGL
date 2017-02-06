<<<<<<< HEAD
// Rose.cpp: implementation of the CRose class.
//
//////////////////////////////////////////////////////////////////////

#include "Rose.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRose::CRose()
{

}

CRose::~CRose()
{

}
void CRose::ReadData()
{
	if (!m_pModel) 
	{
        m_pModel = glmReadOBJ("data/rose+vase.obj");
        if (!m_pModel) exit(0);
        glmUnitize(m_pModel);
        glmFacetNormals(m_pModel);
        glmVertexNormals(m_pModel, 90.0);
    }
}

void CRose::Draw()
{
	if (m_pModel)
		glmDraw(m_pModel, GLM_SMOOTH | GLM_MATERIAL);
=======
// Rose.cpp: implementation of the CRose class.
//
//////////////////////////////////////////////////////////////////////

#include "Rose.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRose::CRose()
{

}

CRose::~CRose()
{

}
void CRose::ReadData()
{
	if (!m_pModel) 
	{
        m_pModel = glmReadOBJ("data/rose+vase.obj");
        if (!m_pModel) exit(0);
        glmUnitize(m_pModel);
        glmFacetNormals(m_pModel);
        glmVertexNormals(m_pModel, 90.0);
    }
}

void CRose::Draw()
{
	if (m_pModel)
		glmDraw(m_pModel, GLM_SMOOTH | GLM_MATERIAL);
>>>>>>> dd82df2a6f75602c93d298985876b7aa6a44ba65
}