#pragma once

#define TS_PROCESS_ERROR(Condition)     if (!(Condition))   goto Exit0
#define TS_PROCESS_SUCCESS(Condition)   if (Condition)      goto Exit1
#define T_PROCESS_ERROR(Condition)     if (!(Condition))   goto Exit0
#define T_PROCESS_SUCCESS(Condition)   if (Condition)      goto Exit1
#define T_COM_PROCESS_ERROR(Condition) if (FAILED(Condition))  goto Exit0
#define T_COM_PROC_ERR_RET_CONDITION(Condition) do { if (FAILED(Condition)) { hrRetResult = Condition; goto Exit0; } } while (0)
#define T_COM_PROCESS_SUCCESS(Condition)   if (SUCCEEDED(Condition))   goto Exit1
#define T_COM_RELEASE(pInterface) do { if (pInterface) { (pInterface)->Release(); ((pInterface)) = NULL; } } while (0)
#define T_DELETE_ARRAY(pArray) do { if (pArray) { delete[] (pArray); ((pArray)) = NULL; } } while (0)
#define T_DELETE(p) do { if (p) { delete (p); (p) = NULL; } } while (0)
#define T_TRACE_BEXIT(content) do { TRACE(content" %s\n", !(bRetResult) ? "Exit0": "Exit1"); } while (0)
#define T_TRACE_HREXIT(content) do { TRACE(content" %s\n", FAILED(hrRetResult) ? "Exit0": "Exit1"); } while (0)

#define T_ARRAY_SIZE(x)  (sizeof(x) / sizeof(x[0]))

#ifdef BASEASSERT
#undef BASEASSERT
#endif


#include <assert.h>
#define BASEASSERT 	assert

