

#ifndef PERFORMANCE_AMD
#define PERFORMANCE_AMD

#include <GPUPerfAPI.h>
#include <GPUPerfAPIFunctionTypes.h>


typedef GPA_Status(*f_GPA_Initialize)();
typedef GPA_Status(*f_GPA_OpenContext)(void*);

typedef GPA_Status(*f_GPA_GetNumCounters)(gpa_uint32*);
typedef GPA_Status(*f_GPA_GetCounterName)(gpa_uint32, const char**);
typedef GPA_Status(*f_GPA_GetCounterIndex)(const char*, gpa_uint32);
typedef GPA_Status(*f_GPA_GetCounterDescription)(gpa_uint32 index, const char** ppDescription);

typedef GPA_Status(*f_GPA_EnableCounter)(gpa_uint32);
typedef GPA_Status(*f_GPA_EnableAllCounters)();
typedef GPA_Status(*f_GPA_DisableCounter)(gpa_uint32);
typedef GPA_Status(*f_GPA_DisableAllCounters)();
typedef GPA_Status(*f_GPA_DisableCounterStr)(const char* pCounter);
typedef GPA_Status(*f_GPA_GetCounterDataType)(gpa_uint32 index, GPA_Type* pDataType);
typedef GPA_Status(*f_GPA_GetCounterUsageType)(gpa_uint32 index, GPA_Usage_Type usageType);

typedef GPA_Status(*f_GPA_GetPassCount)(gpa_uint32*);

typedef GPA_Status(*f_GPA_BeginSession)(gpa_uint32*);
typedef GPA_Status(*f_GPA_EndSession)();

typedef GPA_Status(*f_GPA_BeginPass)();
typedef GPA_Status(*f_GPA_EndPass)();

typedef GPA_Status(*f_GPA_BeginSample)(gpa_uint32);
typedef GPA_Status(*f_GPA_EndSample)();

typedef GPA_Status(*f_GPA_IsSessionReady)(bool* pReadyResult, gpa_uint32 sessionID);
typedef GPA_Status(*f_GPA_IsSampleReady)(bool* pReadyResult, gpa_uint32 sessionID, gpa_uint32 sampleID);

typedef GPA_Status(*f_GPA_GetSampleUInt32)(gpa_uint32 sessionID, gpa_uint32 sampleID, gpa_uint32 counterID, gpa_uint32* pResult);
typedef GPA_Status(*f_GPA_GetSampleUInt64)(gpa_uint32 sessionID, gpa_uint32 sampleID, gpa_uint32 counterID, gpa_uint64* pResult);
typedef GPA_Status(*f_GPA_GetSampleFloat32)(gpa_uint32 sessionID, gpa_uint32 sampleID, gpa_uint32 counterID, gpa_float32* pResult);
typedef GPA_Status(*f_GPA_GetSampleFloat64)(gpa_uint32 sessionID, gpa_uint32 sampleID, gpa_uint32 counterID, gpa_float64* pResult);

typedef GPA_Status(*f_GPA_CloseContext)();
typedef GPA_Status(*f_GPA_Destroy)();

class PerformanceAMD {
public:
	static f_GPA_Initialize GPA_Initialize;
	static f_GPA_BeginSession GPA_BeginSession;
	static f_GPA_GetNumCounters GPA_GetNumCounters;
	static f_GPA_GetCounterName  GPA_GetCounterName;
	static f_GPA_GetCounterIndex GPA_GetCounterIndex;
	static f_GPA_GetCounterDescription GPA_GetCounterDescription;
	static f_GPA_EnableCounter GPA_EnableCounter;
	static f_GPA_GetPassCount GPA_GetPassCount;
	static f_GPA_BeginPass GPA_BeginPass;
	static f_GPA_BeginSample GPA_BeginSample;
	static f_GPA_EndSample GPA_EndSample;
	static f_GPA_EndPass GPA_EndPass;
	static f_GPA_EndSession GPA_EndSession;
	static f_GPA_IsSessionReady GPA_IsSessionReady;
	static f_GPA_GetCounterDataType GPA_GetCounterDataType;
	static f_GPA_GetCounterUsageType GPA_GetCounterUsageType;
	static f_GPA_GetSampleUInt32 GPA_GetSampleUInt32;
	static f_GPA_GetSampleUInt64 GPA_GetSampleUInt64;
	static f_GPA_GetSampleFloat32 GPA_GetSampleFloat32;
	static f_GPA_GetSampleFloat64 GPA_GetSampleFloat64;
	static f_GPA_DisableCounter GPA_DisableCounter;
	static f_GPA_CloseContext GPA_CloseContext;
	static f_GPA_Destroy GPA_Destroy;
	static f_GPA_OpenContext GPA_OpenContext;
	static f_GPA_DisableAllCounters GPA_DisableAllCounters;
	static f_GPA_EnableAllCounters GPA_EnableAllCounters;
	static f_GPA_DisableCounterStr GPA_DisableCounterStr;
};




#endif