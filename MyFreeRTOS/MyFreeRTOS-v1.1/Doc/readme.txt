ʵ��������ʱ�б�

�޸�list.h�еĴ���
/* ��ȡ����ڵ��OWNER����TCB */

#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )										\

{																							\

	List_t * const pxConstList = ( pxList );											    \

	/* �ڵ�����ָ�������һ���ڵ�����ڵ�����ָ�룬ָ����һ���ڵ㣬
    �����ǰ������N���ڵ㣬����N�ε��øú���ʱ��pxInedex��ָ���N���ڵ� */\

	( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;							\

	/* ��ǰ����Ϊ�� */                                                                       \

	if( ( void * ) ( pxConstList )->pxIndex == ( void * ) &( ( pxConstList )->xListEnd ) )	\

	{																						\


		( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;						\
	}																						\

	/* ��ȡ�ڵ��OWNER����TCB */                                                             \
	( pxTCB ) = ( pxConstList )->pxIndex->pvOwner;											 \

}