#ifndef _LIST_H
#define _LIST_H
#include "portmacro.h"

struct xLIST_ITEM
{
	TickType_t xItemValue;            /*����ֵ�����ڰ����ڵ���˳������*/
	struct xLIST_ITEM *pxNext;        /*ָ��������һ���ڵ�*/
	struct xLIST_ITEM *pxPrevious;    /*ָ������ǰһ���ڵ�*/
	void * pvOwner;                   /*ָ��ӵ�иýڵ���ں˶���ͨ����TCB**/
	void * pvContainer;               /*ָ��ýڵ����ڵ�����*/ 
};

typedef struct xLIST_ITEM ListItem_t;	/*�ڵ����������ض���*/

struct xMINI_LIST_ITEM
{
	TickType_t xItemValue;             /*����ֵ�����ڰ����ڵ�����������*/
	struct xLIST_ITEM * pxNext;        /*ָ��������һ���ڵ�*/
	struct xLIST_ITEM * pxPrevious;    /*ָ������ǰһ���ڵ�*/
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;

typedef struct xLIST
{
	UBaseType_t uxNumberOfItems;      /*����ڵ������*/
	ListItem_t * pxIndex;             /*����ڵ�����ָ��*/
	MiniListItem_t xListEnd;          /*�������һ���ڵ�*/
}List_t;


/*��ʼ���ڵ��ӵ����*/
#define listSET_LIST_ITEM_OWNER(pxListItem,pxOwner)\
        ((pxListItem)->pvOwner = (void *)(pxOwner))
/*��ȡ�ڵ�ӵ����*/
#define listGET_LIST_ITEM_OWNER(pxListItem)\
        ((pxListItem)->pvOwner)
/*��ʼ���ڵ�������ֵ*/
#define listSET_LIST_ITEM_VALUE(pxListItem,xValue)\
        ((pxListItem)->xItemValue = (xValue))
/*��ȡ�ڵ�������ֵ*/
#define listGET_LIST_ITEM_VALUE( pxListItem )\
        ( (pxListItem)->xItemValue )
/*��ȡ�������ڽڵ��������ֵ*/
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxList )\
        ( ( (pxList)->xListEnd).pxNext->xItemValue )
/*��ȡ�������ڽڵ�*/
#define listGET_HEAD_ENTRY( pxList )\
        ( ( ( pxList )->xListEnd ).pxNext ) 
/*��ȡ�ڵ����һ���ڵ�*/
#define listGET_NEXT( pxListItem )\
				( ( pxListItem )->pxNext )
/*��ȡ��������һ���ڵ�*/
#define listGET_END_MARKER( pxList )\
        ( ( ListItem_t const * ) ( &( (pxList)->xListEnd ) ) )
/*�ж������Ƿ�Ϊ��*/
#define listLIST_IS_EMPTY( pxList )\
        ( ( BaseType_t ) ( ( pxList )->uxNumberOfItems == ( UBaseType_t )0 ) )
/*��ȡ����ĸ���*/
#define listCURRENT_LIST_LENGTH( pxList )\
        ( (pxList)->uxNumberOfItems )
/*��ȡ�����һ���ڵ��OWNER,��TCB*/
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )    \
{                                                      \
	List_t * const pxConstList = (pxList);                \
	/*�ڵ�����ָ������ĵ�һ���ڵ�*/                        \
	( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;\
	/*ʹ(pxConstList)->pxIndex ָ�� �׽ڵ�*/                         \
	if((void *) (pxConstList)->pxIndex == (void *) &((pxConstList)->xListEnd))\
	{          \
		(pxConstList)->pxIndex = ( pxConstList )->pxIndex->pxNext;            \
		/*��ȡ�ڵ�� OWNER ,��TCB */    \
		( pxTCB ) = (pxConstList)->pxIndex->pvOwner;                  \
	}   \
}
/*��ȡ�����׸��ڵ��������ƿ�ĵ�ַ*/
#define listGET_OWNER_OF_HEAD_ENTRY( pxList )  ( (&( ( pxList )->xListEnd ))->pxNext->pvOwner )				
				
void vListInitialiseItem( ListItem_t * const pxItem );
void vListInitialise( List_t * const pxList );
void vListInsertEnd( List_t * const pxList, ListItem_t * const pxNewListItem);
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem);
UBaseType_t uxListRemove( ListItem_t * const pxItemToRemove);


#endif //_LIST_H

