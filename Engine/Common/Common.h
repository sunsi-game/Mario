#pragma once
#pragma warning(disable: 4251)	// 미봉책(방법 없음).
#pragma warning(disable: 4172)	// 위험성 인지한 상태로 사용.

//#define ENGINE_BUILD_DLL 1

#define DLLEXPORT   __declspec(dllexport)
#define DLLIMPORT   __declspec(dllimport)

// ENGINE_BUILD_DLL이 선언되어 있는지에 따라 분기.
#if ENGINE_BUILD_DLL
#define WANTED_API DLLEXPORT
#else
#define WANTED_API DLLIMPORT
#endif