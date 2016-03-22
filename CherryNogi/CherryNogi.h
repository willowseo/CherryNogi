
#ifdef CHERRYNOGI_EXPORTS
#define CHERRYNOGI_API __declspec(dllexport)
#else
#define CHERRYNOGI_API __declspec(dllimport)
#endif

int init();
