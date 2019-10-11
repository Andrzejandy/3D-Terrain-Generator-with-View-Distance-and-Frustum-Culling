#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#ifdef ENVIRONMENT32
//__declspec(align(16))
//#define	_XM_NO_INTRINSICS_
#endif

extern const int wind_width;
extern const int wind_height;