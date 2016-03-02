#ifndef __NETWORK_INTERFACE_H
#define __NETWORK_INTERFACE_H

#include <stdio.h>    // size_t
#include <stdint.h>   // uint32_t

struct network_interface {
    char        name[16];
    char        ip[16];     // ip[16] = "xxx.xxx.xxx.xxx"
    uint32_t    addr;       // address in network byte order
    uint32_t    netmask;    // mask in network byte order
};

/*
 * @param interface         network_interface array
 * @param interface_size    array size
 * @return -1               failure
 *         >= 0             number of network interface
 */
int network_interface_get(struct network_interface * interface, size_t interface_size);

#endif
