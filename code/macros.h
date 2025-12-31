#define sizeof(type) (int64)sizeof(type)
#define countof(array) (int64)(sizeof(array) / sizeof(*(array)))
#define lengthof(string) (countof(string) - 1)

#if DEBUG
#define assert(expression) if (!(expression)) { *(int32*)0 = 0; }
#else
#define assert(expression) expression
#endif

#define KILOBYTES(value) ((int64)(value) * 1024)
#define MEGABYTES(value) (KILOBYTES(value) * 1024)
#define GIGABYTES(value) (MEGABYTES(value) * 1024)
#define TERABYTES(value) (GIGABYTES(value) * 1024)
