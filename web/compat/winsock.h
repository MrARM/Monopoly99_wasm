#ifndef MONOPOLY_WEB_COMPAT_WINSOCK_H
#define MONOPOLY_WEB_COMPAT_WINSOCK_H

#include "windows.h"

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef uintptr_t SOCKET;

#define INVALID_SOCKET ((SOCKET)(~0u))
#define SOCKET_ERROR (-1)
#define AF_INET 2
#define SOCK_STREAM 1
#define SOL_SOCKET 0xffff
#define SO_DONTROUTE 0x0010
#define IPPROTO_TCP 6
#define INADDR_NONE 0xffffffffu
#define INADDR_ANY 0x00000000u
#define FIONBIO 0x8004667e
#define FD_READ 0x01
#define FD_ACCEPT 0x08
#define FD_CLOSE 0x20
#define WSAEWOULDBLOCK 10035

struct in_addr {
    union {
        struct { u_char s_b1, s_b2, s_b3, s_b4; } S_un_b;
        u_long S_addr;
    } S_un;
};
#define s_addr S_un.S_addr

struct sockaddr {
    u_short sa_family;
    char sa_data[14];
};
typedef struct sockaddr SOCKADDR, *PSOCKADDR, *LPSOCKADDR;

struct sockaddr_in {
    short sin_family;
    u_short sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};
typedef struct sockaddr_in SOCKADDR_IN, *PSOCKADDR_IN, *LPSOCKADDR_IN;

struct hostent {
    char *h_name;
    char **h_aliases;
    short h_addrtype;
    short h_length;
    char **h_addr_list;
};
typedef struct hostent HOSTENT, *PHOSTENT, *LPHOSTENT;
#define h_addr h_addr_list[0]

typedef struct WSAData {
    WORD wVersion;
    WORD wHighVersion;
    char szDescription[257];
    char szSystemStatus[129];
    unsigned short iMaxSockets;
    unsigned short iMaxUdpDg;
    char *lpVendorInfo;
} WSADATA, *LPWSADATA;

static inline SOCKET socket(int, int, int) { return INVALID_SOCKET; }
static inline int closesocket(SOCKET) { return 0; }
static inline int connect(SOCKET, const struct sockaddr *, int) { return SOCKET_ERROR; }
static inline int bind(SOCKET, const struct sockaddr *, int) { return SOCKET_ERROR; }
static inline int listen(SOCKET, int) { return SOCKET_ERROR; }
static inline SOCKET accept(SOCKET, struct sockaddr *, int *) { return INVALID_SOCKET; }
static inline int recv(SOCKET, char *, int, int) { return SOCKET_ERROR; }
static inline int send(SOCKET, const char *, int len, int) { return len; }
static inline int setsockopt(SOCKET, int, int, const char *, int) { return 0; }
static inline int ioctlsocket(SOCKET, long, u_long *) { return 0; }
static inline int WSAAsyncSelect(SOCKET, HWND, u_int, long) { return SOCKET_ERROR; }
static inline int WSAStartup(WORD, LPWSADATA data) { if (data) { memset(data, 0, sizeof(*data)); data->wVersion = 0x0101; } return 0; }
static inline int WSACleanup(void) { return 0; }
static inline int WSAGetLastError(void) { return WSAEWOULDBLOCK; }
static inline u_short htons(u_short value) { return (u_short)((value << 8) | (value >> 8)); }
static inline u_short ntohs(u_short value) { return htons(value); }
static inline u_long inet_addr(const char *) { return INADDR_NONE; }
static inline char *inet_ntoa(struct in_addr) { static char value[] = "0.0.0.0"; return value; }
static inline struct hostent *gethostbyname(const char *) { return NULL; }
static inline int gethostname(char *name, int len) { if (name && len > 0) { strncpy(name, "web", (size_t)len); name[len - 1] = '\0'; } return 0; }

#endif
