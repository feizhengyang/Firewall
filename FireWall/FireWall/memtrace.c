#include<ntddk.h>

#include"memtrace.h"

#if DBG

static  KSPIN_LOCK  lock;
static  PMEM_NODE   pHead,pTail;
static  int         cBytes;

VOID    DbgMemTraceInit()
/*++
--*/

{
    KeInitializeSpinLock( &lock );
    pHead = pTail = NULL;
    cBytes = 0;

}
PVOID   DbgExAllocatePool( IN ULONG size,IN const char *file,IN ULONG line )
/*++
--*/
{
    PMEM_NODE   node;
    KIRQL       irql;

    if( size == 0 )return NULL;


    node = ExAllocatePoolWithTag( NonPagedPool,
                                  sizeof(MEM_NODE) + size,
                                  MEM_TAG );
    if( node == NULL)return NULL;

    node->magic = MEM_MAGIC;
    node->file = file;
    node->line = line;
    node->size = size;
    node->prev = NULL;
    node->next = NULL;
    RtlZeroMemory( node->data,size );

    KeAcquireSpinLock( &lock,&irql );

    if( pTail == NULL )
    {
        pHead = pTail = node;
    }
    else
    {
        pHead->prev = node;
        node->next = pHead;
        pHead = node;
    }
    cBytes += size;
    KeReleaseSpinLock( &lock,irql );
    KdPrint(("alloc %x\n",node ));
    return node->data;
}


VOID    DbgExFreePool( IN PVOID ptr )
/*++
--*/
{
    PMEM_NODE   p = (PMEM_NODE)((ULONG)ptr - sizeof(MEM_NODE));
    KIRQL       irql;

    if( pHead == NULL )return;
    KdPrint(("Free %x\n",p ));
    ASSERT( p->magic == MEM_MAGIC );

    KeAcquireSpinLock( &lock,&irql );

    if( p->prev == NULL ){
        pHead = p->next;
        //pHead->prev = NULL;
    }
    else
    {
        p->prev->next = p->next;
    }

    if( p->next == NULL )
    {
        pTail = p->prev;
        //pTail->next = NULL;
    }
    else
    {
        p->next->prev = p->prev;
    }
    cBytes -= p->size;
    KeReleaseSpinLock( &lock,irql );

    ExFreePoolWithTag( p,MEM_TAG ); 
    
}

BOOLEAN DbgIsMemLeak()
/*++
--*/
{
    if( cBytes == 0 && pHead == NULL && pTail == NULL)
        return FALSE;
    else
    {
        KdPrint(("cBytes = %x,pHead = %x,pTail=%x\n",
            cBytes,pHead,pTail ));
        return TRUE;
    }
}

#endif
                                                                                                                                                                                                                                                                                                                                                                                                                  