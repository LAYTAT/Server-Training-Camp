#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#if defined(WIN32) || defined(_WIN32)
#include <winsock2.h>
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <signal.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/select.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/epoll.h>
#endif


#define DEFAULT_SERVER_PORT 3000
#define DEFAULT_SERVER_ADDR "10.0.150.52"
#define MAX_LINK_COUNT 1024
#define BUFFER_SIZE 1024*1024    //0x200000
#define HEADBUFFER_SIZE 1024*1024   //0x100000
#define MAX_BAG_SIZE 48  // bag size
#define MAX_ITEM_BAGOVERLY 99
#define MAX_ITEM_CURRENCYOVERLY 999999999

typedef int32_t INT32;
typedef uint32_t UINT32;
typedef int64_t INT64;
typedef uint64_t UINT64;


enum COMPONENTS_ENUM
{
    COMPONENTS_POSITION = 1,
    COMPONENTS_ORIENT = 2,
    COMPONENTS_SPEED = 3,
    COMPONENTS_ITEM = 4,
    COMPONENTS_BAG = 5,
    COMPONENTS_EQUIP = 6,
    COMPONENTS_CURRENCY = 7,

};

enum ENTITY_ENUM
{
    ENTITY_PLAYER = 1,
    ENTITY_BAG = 2,
    ENTITY_EQUIP = 3,
    ENTITY_CURRENCY = 4,
};

#endif
