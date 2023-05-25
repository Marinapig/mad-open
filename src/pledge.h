#ifdef __linux__
#define pledge(request, paths) 0
#define unveil(path, permissions) 0
#endif
