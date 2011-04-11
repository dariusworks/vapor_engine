/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "CuTest.h"

#include "Core/API.h"
#include "Core/Concurrency.h"
#include "Core/Memory.h"

using namespace vapor;

static int value = 20;

static void Run(void* data)
{
	int* num = (int*) data;
	value = *num;
}

void TestThreads(CuTest* tc)
{
	ThreadPtr thread( pThreadCreate(AllocatorGetHeap()) );

	int data = 42;
	ThreadStart(thread, Run, &data);
	ThreadJoin(thread);

	CuAssertIntEquals(tc, 42, value);
}

static Condition* cond;
static Mutex* mutex;

static void Run2(void* data)
{
	MutexLock(mutex);
	ConditionWait(cond, mutex);
	MutexUnlock(mutex);
	
	value = 30;
}

void TestCondition(CuTest* tc)
{
	value = 20;

	ThreadPtr thread( pThreadCreate(AllocatorGetHeap()) );

	cond = ConditionCreate( AllocatorGetHeap() );
	mutex = MutexCreate( AllocatorGetHeap() );
	
	ThreadStart(thread, Run2, nullptr);
	CuAssertIntEquals(tc, 20, value);
	
	// Prevents deadlock.
	while(value == 20)
	{
		ConditionWakeAll(cond);
	}

	ThreadJoin(thread);
	CuAssertIntEquals(tc, 30, value);

	MutexDestroy( mutex, AllocatorGetHeap() );
	ConditionDestroy( cond, AllocatorGetHeap() );
}

void TestAtomics(CuTest* tc)
{	
	Atomic atomic = 17;
	CuAssertIntEquals(tc, 17, atomic);

	AtomicIncrement(&atomic);
	CuAssertIntEquals(tc, 18, atomic);

	AtomicDecrement(&atomic);
	AtomicDecrement(&atomic);
	CuAssertIntEquals(tc, 16, atomic);

	AtomicAdd(&atomic, 10);
	CuAssertIntEquals(tc, 26, atomic);

	AtomicWrite(&atomic, 33);
	CuAssertIntEquals(tc, 33, AtomicRead(&atomic));
}

CuSuite* GetSuiteConcurrency() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestThreads);
	SUITE_ADD_TEST(suite, TestCondition);
	SUITE_ADD_TEST(suite, TestAtomics);
    return suite;
}