
#ifdef _WIN32
 // we will not use these functions from the winsock
 #define htons _none_1
 #define htonl _none_2
 #include <windows.h>
#endif

#ifdef htons
#undef htons
#endif
#ifdef ntohs
#undef ntohs
#endif
#ifdef htonl
#undef htonl
#endif
#ifdef ntohl
#undef ntohl
#endif

unsigned short htons(unsigned short input);
unsigned int htonl(unsigned int input);

#define ntohs htons
#define ntohl htonl
