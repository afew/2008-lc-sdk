// Implementation of the Tree class.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>

#include "LcTree.h"



CLcTree::CLcTree() : pP(0), pC(0), pB(0), pN(0){}
CLcTree::CLcTree(CLcTree *pNod) : pP(0), pC(0), pB(0), pN(0)
{
	AttachTo(pNod);
}


CLcTree::~CLcTree()
{	
	Detach();																	// �θ��� �ο��� ����

	if (0 == pC)
		return;
	
	while(pC)																	// �� �ؿ� �ڽ��� �ֳ�?
	{
		pC->Detach();
	}
}

BOOL CLcTree::HasParent()		{	return (0 != pP);	}						// �θ� �ֳ�?
BOOL CLcTree::HasNotParent()	{	return (0 == pP);	}
BOOL CLcTree::HasChild()		{	return (0 != pC);	}						// �ڽ��� �ֳ�?
BOOL CLcTree::HasNotChild()		{	return (0 == pC);	}
BOOL CLcTree::IsSiblingF()		{	return (pP && pP->pC == this);			}	// ���� ù��° �Ƶ��̳�



BOOL CLcTree::IsSiblingL()		{	return (pP && 0 == pN)	;				}	// ���� ������ �Ƶ��̳�
BOOL CLcTree::IsRoot()			{	return (0 == pP);						}	// ���� ��Ʈ�ΰ�?
BOOL CLcTree::IsChild()			{	return (0 != pP);						}	// ���� �ڽ��ΰ�?



CLcTree* CLcTree::GetSiblingF()	{	return (pB)? pB->GetSiblingF(): this;	}	// �ڸ��� ó�� �����͸� ã�´�.
CLcTree* CLcTree::GetSiblingL()	{	return (pN)? pN->GetSiblingL(): this;	}	// �ڸ��� ������ �����͸� ã�´�
CLcTree* CLcTree::FindRoot()	{	return (pP)? pP->FindRoot():	this;	}	// �ֻ��� ��Ʈ



void CLcTree::AttachTo(CLcTree *pPrn)				// �θ�  �Ծ�Ǳ�^^
{
	if (pP)											// ���࿡ ���� �ٸ� ���� Attach�Ǿ� �ִٸ�
		Detach();									// ���� ������.
	
	pP = pPrn;										// ���� �θ� ����
	
	CLcTree *pParentChild =	pP->pC; 
	
	if (!pP->pC)									// �θ��� �ڽ��� ���ٸ�
	{
		pP->pC = this;								// ���� �θ��� �ڽ����� �����.
	}
	
	else											// �θ� �ؿ� �ڽ��� �ִٸ�
	{
		CLcTree * pSiblingLast;
		pSiblingLast =
			pParentChild->GetSiblingL();

		pB = pSiblingLast;							// �� �տ��� �θ��ڽ��� ���δ�.
		pSiblingLast->pN = this;					// ���� �θ� �ڽ��� �������� ����Ǿ� �ִ� ���� ���δ�.
	}
}

void CLcTree::Attach(CLcTree *pCh)					// �Ծ��ϱ�
{
	if (pCh->HasParent())							// �Ծ�Ǵ� �ڽ��� �ٸ� �θ� �ؿ� ���� ���
		pCh->Detach();
	
	pCh->pP = this;									// �ϴ� �θ� ���� �������ش�.
	
	if (pC)											// �� �ؿ� �ڽ��� ���� ���
	{
		CLcTree * pSiblingLast;
		pSiblingLast = pC->GetSiblingL();			// �ڸŵ��� �� �������� ����
		
		pSiblingLast->pN = pCh;
		pCh->pB = pSiblingLast;						// �ڽ��� �ڸŵ��߿� �� ���� ���δ�.
	}
	
	else											// �ڽ��� ���� ��� �ٷ� ���δ�.
	{
		pC = pCh;
	}
}

void CLcTree::Detach()
{
	if (pP && pP->pC == this)						// �θ� �ְ� ���� �θ��� �ڽ��̶��
	{
		if (pN)										// ���� �ڸŰ� �ִٸ�
		{
			pP->pC = pN;							// �θ��� �ڽ��� �� �ڸŷ� �Ѵ�.
			pP->pC->pB = 0;
		}
		
		else										// �ڽ��� �ٷ� �����
			pP->pC = 0;								// �ٷ� ���� ������.
	}
	
	if(pB)
		pB->pN = pN;								// get rid of links
	
	if(pN)
		pN->pB = pB;
	
	pP = 0;											// �θ�
	pB = 0;											// 
	pN = 0;
}

int CLcTree::CountNodes()
{
	if (pC)
		return pC->CountNodes() + 1;
	else
		return 1;
}



