// Interface for the LcTree class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcTree_H_
#define _LcTree_H_


class CLcTree								// Lc tree class
{
public:
	CLcTree*	pP;							// �θ���
	CLcTree*	pC;							// �ڽĳ��
	CLcTree*	pB;							// �ڸų�� ����
	CLcTree*	pN;							// �ڸų�� ����
	
public:
	CLcTree();
	CLcTree(CLcTree *pNod);

	virtual ~CLcTree();

	BOOL		HasParent();				// �θ� �ֳ�?
	BOOL		HasNotParent();									
	BOOL		HasChild();					// �ڽ��� �ֳ�?
	BOOL		HasNotChild();									
	BOOL		IsSiblingF();				// ���� ù��° �Ƶ��̳�

	BOOL		IsSiblingL();				// ���� ������ �Ƶ��̳�
	BOOL		IsRoot();					// ���� ��Ʈ�ΰ�?
	BOOL		IsChild();					// ���� �ڽ��ΰ�?

	CLcTree*	GetSiblingF();				// �ڸ��� ó�� �����͸� ã�´�.
	CLcTree*	GetSiblingL();				// �ڸ��� ������ �����͸� ã�´�
	CLcTree*	FindRoot();					// �ֻ��� ��Ʈ

	void		AttachTo(CLcTree *pPrn);	// �θ�  �Ծ�Ǳ�
	void		Attach(CLcTree *pCh);		// �Ծ��ϱ�
	void		Detach();
	
	int			CountNodes();	
};


#endif


