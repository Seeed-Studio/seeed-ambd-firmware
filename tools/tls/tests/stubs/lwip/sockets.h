#ifndef TEST_LWIP_SOCKETS_H
#define TEST_LWIP_SOCKETS_H
#include <stddef.h>
int lwip_send(int fd, const void *buffer, size_t length, int flags);
int lwip_recv(int fd, void *buffer, size_t length, int flags);
#endif
