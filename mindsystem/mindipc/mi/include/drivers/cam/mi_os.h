/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/

///////////////////////////////////////////////////////////////////////////////
/// @file      mi_os.h
/// @brief     MI OS Wrapper Header File for
///            1. RTK OS
///            2. Linux User Space
///            3. Linux Kernel Space
///////////////////////////////////////////////////////////////////////////////

#ifndef _MI_OS_H_
#define _MI_OS_H_

#include "mi_os_datatype.h"

//=============================================================================
// Description:
//      Writes the C string pointed by format to the standard output.
// Parameters:
//      [in]  szFmt: C string that contains the text to be written, it can
//                   optionally contain embedded format specifiers.
// Return:
//      N/A
//=============================================================================
MI_S32 MI_OS_Print(const char *szFmt, ...);

//=============================================================================
// Description:
//      Suspend execution for millisecond intervals.
// Parameters:
//      [in]  nMsec: Millisecond to suspend.
// Return:
//      N/A
//=============================================================================
MI_S32 MI_OS_Sleep(MI_U32 u32Msec);

//=============================================================================
// Description:
//      Get the number of seconds and nanoseconds since the Epoch.
// Parameters:
//      [out] ptRes: A pointer to a MI_OS_Timespec_t structure where
//                   MI_OS_GetTimeOfDay() can store the time.
// Return:
//      N/A
//=============================================================================
MI_S32 MI_OS_GetTimeOfDay(MI_OS_TimeSpec_t *pstTimeSpec);

//=============================================================================
// Description:
//      Set the number of seconds and nanoseconds since the Epoch.
// Parameters:
//      [in] ptRes: A pointer to a MI_OS_Timespec_t structure.
// Return:
//      N/A
//=============================================================================
MI_S32 MI_OS_SetTimeOfDay(const MI_OS_TimeSpec_t *pstTimeSpec);

//=============================================================================
// Description:
//      Gets the current time of the clock specified, and puts it into the
//      buffer pointed to by ptRes.
// Parameters:
//      [out] ptRes: A pointer to a MI_OS_Timespec_t structure where
//                   MI_OS_GetMonotonicTime() can store the time.
// Return:
//      N/A
//=============================================================================
MI_S32 MI_OS_GetMonotonicTime(MI_OS_TimeSpec_t *pstTimeSpec);

//=============================================================================
// Description:
//      The MI_OS_ThreadCreate() function is used to create a new thread/task,
//      with attributes specified by ptAttrb. If ptAttrb is NULL, the default
//      attributes are used.
// Parameters:
//      [out] pThread: A successful call to MI_OS_ThreadCreate() stores the handle
//                     of the new thread in the buffer pointed to by pThread.
//      [in]  ptAttrb: Argument points to a MI_OS_ThreadAttrb_t structure whose
//                     contents are used at thread creation time to determine
//                     thread priority, stack size and thread name. Thread
//                     priority range from 1(lowest) to 99(highest), use OS
//                     default priority if set 0.
//      [in]  pfnStartRoutine(): The new thread starts execution by invoking it.
//      [in]  pArg: It is passed as the sole argument of pfnStartRoutine().
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_ThreadCreate(MI_OS_Thread_t *pThread,
                             MI_OS_ThreadAttr_t *pstAttr,
                             void (*pfnStartRoutine)(void *),
                             void *pArg);

//=============================================================================
// Description:
//      Stop a thread created by MI_OS_ThreadCreate in Linux kernel space. This
//      function is not applicable to RTK and Linux user space.
// Parameters:
//      [in]  thread: Handle of target thread.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_ThreadStop(MI_OS_Thread_t thread);

//=============================================================================
// Description:
//      When someone calls MI_OS_ThreadStop, it will be woken and this will
//      return true. You should then return from the thread. This function is
//      not applicable to RTK and Linux user space.
// Parameters:
//      N/A
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_ThreadShouldStop(MI_BOOL *bStop);

//=============================================================================
// Description:
//      Set the name of a thread. The thread name is a meaningful C language
//      string, whose length is restricted to 16 characters, including the
//      terminating null byte ('\0').
// Parameters:
//      [in]  thread: Handle of target thread.
//      [in]  szName: specifies the new name.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_ThreadSetName(const char *szName);

//=============================================================================
// Description:
//      Get the name of a thread. The buffer specified by name should be at
//      least 16 characters in length.
// Parameters:
//      [in]  thread: Handle of target thread.
//      [out] szName: Buffer used to return the thread name.
//      [in]  nLen: Specifies the number of bytes available in szName
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_ThreadGetName(char *szName, MI_U32 u32Len);

//=============================================================================
// Description:
//      Get thread identification.
// Parameters:
//      N/A
// Return:
//      On success, returns the thread ID of the calling process.
//=============================================================================
MI_S32 MI_OS_ThreadGetId(MI_U32 *u32Id);

//=============================================================================
// Description:
//      Initializes the mutex.
// Parameters:
//      [in]  ptMutex: The mutex to initialize.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_MutexInit(MI_OS_Mutex_t *pstMutex);

//=============================================================================
// Description:
//      Destroys the mutex.
// Parameters:
//      [in]  ptMutex: The mutex to destroy.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_MutexDestroy(MI_OS_Mutex_t *pstMutex);

//=============================================================================
// Description:
//      Lock the mutex.
// Parameters:
//      [in]  ptMutex: The mutex to lock.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_MutexLock(MI_OS_Mutex_t *pstMutex);

//=============================================================================
// Description:
//      Unlock the mutex.
// Parameters:
//      [in]  ptMutex: The mutex to unlock.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_MutexUnlock(MI_OS_Mutex_t *pstMutex);

//=============================================================================
// Description:
//      Initializes the semaphore at a given value.
// Parameters:
//      [in]  ptTsem: The semaphore to initialize.
//      [in]  nVal: the initial value of the semaphore.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_SemaphoreInit(MI_OS_Semaphore_t *pstSemaphore, MI_U32 u32Value);

//=============================================================================
// Description:
//      Destroy the semaphore.
// Parameters:
//      [in]  ptTsem: The semaphore to destroy.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_SemaphoreDestroy(MI_OS_Semaphore_t *pstSemaphore);

//=============================================================================
// Description:
//      Increases the value of the semaphore.
// Parameters:
//      [in]  ptTsem: The semaphore to increase.
// Return:
//      N/A
//=============================================================================
MI_S32 MI_OS_SemaphoreUp(MI_OS_Semaphore_t *pstSemaphore);

//=============================================================================
// Description:
//      Decreases the value of the semaphore. Blocks if the semaphore value is
//      zero.
// Parameters:
//      [in]  ptTsem: The semaphore to decrease.
// Return:
//      N/A
//=============================================================================
MI_S32 MI_OS_SemaphoreDown(MI_OS_Semaphore_t *pstSemaphore);

//=============================================================================
// Description:
//      Decreases the value of the semaphore. Blocks if the semaphore value is
//      zero.
// Parameters:
//      [in]  ptTsem: The semaphore to decrease
// Return:
//      If the timeout is reached the function exits with error CAM_OS_TIMEOUT.
//      CAM_OS_OK is returned if down successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_SemaphoreTimedDown(MI_OS_Semaphore_t *pstSemaphore, MI_U32 u32Msec);

//=============================================================================
// Description:
//      Get the value of the semaphore.
// Parameters:
//      [in]  ptTsem: The semaphore to reset.
// Return:
//      The value of the semaphore.
//=============================================================================
MI_S32 MI_OS_SemaphoreGetValue(MI_OS_Semaphore_t *pstSemaphore, MI_U32 *pu32Value);

//=============================================================================
// Description:
//      Initializes the semaphore at a given value.
// Parameters:
//      [in]  ptTsem: The semaphore to initialize.
//      [in]  nVal: the initial value of the semaphore.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_WaitCondInit(MI_OS_WaitCond_t *pstCond);

//=============================================================================
// Description:
//      Destroy the semaphore.
// Parameters:
//      [in]  ptTsem: The semaphore to destroy.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_WaitCondDestroy(MI_OS_WaitCond_t *pstCond);

//=============================================================================
// Description:
//      Signal the condition, if waiting.
// Parameters:
//      [in]  ptTsem: The semaphore to signal
// Return:
//      N/A
//=============================================================================
MI_S32 MI_OS_WaitCondSignal(MI_OS_WaitCond_t *pstCond);

//=============================================================================
// Description:
//      Wait on the condition.
// Parameters:
//      [in]  ptTsem: The semaphore to wait.
// Return:
//      N/A
//=============================================================================
MI_S32 MI_OS_WaitCond(MI_OS_WaitCond_t *pstCond);

//=============================================================================
// Description:
//      Wait on the condition.
// Parameters:
//      [in]  ptTsem: The semaphore to wait.
//      [in]  nMsec: The value of delay for the timeout.
// Return:
//      If the timeout is reached the function exits with error CAM_OS_TIMEOUT.
//      CAM_OS_OK is returned if wait successful; otherwise, returns MI_OS_Ret_e.
//=============================================================================
MI_S32 MI_OS_WaitCondTimed(MI_OS_WaitCond_t *pstCond, MI_U32 u32Msec);

//=============================================================================
// Description:
//      Allocates a block of nSize bytes of memory, returning a pointer to the
//      beginning of the block.
// Parameters:
//      [in]  nSize: Size of the memory block, in bytes.
// Return:
//      On success, a pointer to the memory block allocated by the function. If
//      failed to allocate, a null pointer is returned.
//=============================================================================
MI_S32 MI_OS_MemAlloc(MI_U32 u32Size, void **ppPtr);

//=============================================================================
// Description:
//      Allocates a block of memory for an array of nNum elements, each of them
//      nSize bytes long, and initializes all its bits to zero.
// Parameters:
//      [in]  nNum: Number of elements to allocate.
//      [in]  nSize: Size of each element.
// Return:
//      On success, a pointer to the memory block allocated by the function. If
//      failed to allocate, a null pointer is returned.
//=============================================================================
MI_S32 MI_OS_MemCalloc(MI_U32 u32Num, MI_U32 u32Size, void **ppPtr);

//=============================================================================
// Description:
//      A block of memory previously allocated by a call to MI_OS_MemAlloc,
//      MI_OS_MemCalloc or MI_OS_MemRealloc is deallocated, making it available
//      again for further allocations. If pPtr is a null pointer, the function
//      does nothing.
// Parameters:
//      [in]  pPtr: Pointer to a memory block previously allocated with
//                  MI_OS_MemAlloc, MI_OS_MemCalloc or MI_OS_MemRealloc.
// Return:
//      N/A
//=============================================================================
MI_S32 MI_OS_MemRelease(void* pPtr);

//=============================================================================
// Description:
//      Unsigned 64bit divide with Unsigned 64bit divisor with remainder.
// Parameters:
//      [in]  nDividend: Dividend.
//      [in]  nDivisor: Divisor.
//      [out]  pRemainder: Pointer to the remainder.
// Return:
//      Quotient of division.
//=============================================================================
MI_S32 MI_OS_MathDivU64(MI_U64 u64Dividend, MI_U64 u64Divisor, MI_U64 *pu64Remainder, MI_U64 *pu64Quot);

//=============================================================================
// Description:
//      Signed 64bit divide with signed 64bit divisor with remainder.
// Parameters:
//      [in]  nDividend: Dividend.
//      [in]  nDivisor: Divisor.
//      [out]  pRemainder: Pointer to the remainder.
// Return:
//      Quotient of division.
//=============================================================================
MI_S32 MI_OS_MathDivS64(MI_S64 s64Dividend, MI_S64 s64Divisor, MI_S64 *ps64Remainder, MI_S64 *ps64Quot);

//=============================================================================
// Description:
//      Read atomic variable.
// Parameters:
//      [in]  pstAtomic: pointer of type MI_OS_Atomic_t.
//      [out] ps32Result: result
// Return:
//      SUCCESS
//=============================================================================
MI_S32 MI_OS_AtomicRead(MI_OS_Atomic_t *pstAtomic, MI_S32 *ps32Result);

//=============================================================================
// Description:
//      Set atomic variable.
// Parameters:
//      [in]  pstAtomic: pointer of type MI_OS_Atomic_t.
//      [in]  s32Value: required value.
// Return:
//      SUCCESS
//=============================================================================
MI_S32 MI_OS_AtomicSet(MI_OS_Atomic_t *pstAtomic, MI_S32 s32Value);

//=============================================================================
// Description:
//      Add to the atomic variable and return value.
// Parameters:
//      [in]  s32Value: integer value to add.
//      [in]  pstAtomic: pointer of type MI_OS_Atomic_t.
//      [out] ps32Result: result
// Return:
//      SUCCESS
//=============================================================================
MI_S32 MI_OS_AtomicAddReturn(MI_S32 s32Value, MI_OS_Atomic_t *pstAtomic, MI_S32 *ps32Result);

//=============================================================================
// Description:
//      Subtract the atomic variable and return value.
// Parameters:
//      [in]  s32Value: integer value to subtract.
//      [in]  pstAtomic: pointer of type MI_OS_Atomic_t.
//      [out] ps32Result: result
// Return:
//      SUCCESS
//=============================================================================
MI_S32 MI_OS_AtomicSubReturn(MI_S32 s32Value, MI_OS_Atomic_t *pstAtomic, MI_S32 *ps32Result);

//=============================================================================
// Description:
//      Subtract value from variable and test result.
// Parameters:
//      [in]  s32Value: integer value to subtract.
//      [in]  pstAtomic: pointer of type MI_OS_Atomic_t.
//      [out] pbResult: result equal zero or not
// Return:
//      SUCCESS
//=============================================================================
MI_S32 MI_OS_AtomicSubAndTest(MI_S32 s32Value, MI_OS_Atomic_t *pstAtomic, MI_BOOL *pbResult);

//=============================================================================
// Description:
//      Increment atomic variable and return value.
// Parameters:
//      [in]  pstAtomic: pointer of type MI_OS_Atomic_t.
//      [out] ps32Result: result
// Return:
//      SUCCESS
//=============================================================================
MI_S32 MI_OS_AtomicIncReturn(MI_OS_Atomic_t *pstAtomic, MI_S32 *ps32Result);

//=============================================================================
// Description:
//      decrement atomic variable and return value.
// Parameters:
//      [in]  pstAtomic: pointer of type MI_OS_Atomic_t.
//      [out] pbResult: result equal zero or not
// Return:
//      SUCCESS
//=============================================================================
MI_S32 MI_OS_AtomicDecReturn(MI_OS_Atomic_t *pstAtomic, MI_S32 *ps32Result);

//=============================================================================
// Description:
//      Increment and test result.
// Parameters:
//      [in]  pstAtomic: pointer of type MI_OS_Atomic_t.
//      [out] pbResult: result equal zero or not
// Return:
//      SUCCESS
//=============================================================================
MI_S32 MI_OS_AtomicIncAndTest(MI_OS_Atomic_t *pstAtomic, MI_BOOL *pbResult);

//=============================================================================
// Description:
//      Decrement and test result.
// Parameters:
//      [in]  pstAtomic: pointer of type MI_OS_Atomic_t.
//      [out] pbResult: result equal zero or not
// Return:
//      SUCCESS
//=============================================================================
MI_S32 MI_OS_AtomicDecAndTest(MI_OS_Atomic_t *pstAtomic, MI_BOOL *pbResult);

//=============================================================================
// Description:
//      Add to the atomic variable and test if negative.
// Parameters:
//      [in]  s32Value: integer value to subtract.
//      [in]  pstAtomic: pointer of type MI_OS_Atomic_t.
//      [out] pbResult: result is negative or not
// Return:
//      SUCCESS
//=============================================================================
MI_S32 MI_OS_AtomicAddNegative(MI_S32 s32Value, MI_OS_Atomic_t *pstAtomic, MI_BOOL *pbResult);

#endif /* _MI_OS_H_ */
