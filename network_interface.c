#include <stdio.h>      // fprintf, stderr, size_t
#include <string.h>     // memset, strcpy, strerror
#include <errno.h>      // errno
#include <unistd.h>     // close
#include <sys/ioctl.h>  // ioctl, SIOCGIFCONF
#include <net/if.h>     // struct ifconf, struct ifreq
#include <sys/socket.h> // socket, AF_INET, SOCK_DGRAM
#include <netinet/in.h> // struct sockaddr_in, also include <sys/socket.h>
#include <arpa/inet.h>  // inet_ntop, also include <netinet/in.h>
#include "network_interface.h"

#define BUFFER_LEN  2048
#define IP_LEN      16

// http://stackoverflow.com/a/8007079
#ifndef _SIZEOF_ADDR_IFREQ
#define _SIZEOF_ADDR_IFREQ sizeof
#endif

int network_interface_get(struct network_interface * interface, const size_t interface_size) {
    if (interface == NULL) {
        fprintf(stderr, "ERROR: interface should not be NULL\n");
        return -1;
    }

    // memset interface
    const size_t SIZE_OF_INTERFACE_LIST = sizeof(struct network_interface) * interface_size;
    memset(interface, 0, SIZE_OF_INTERFACE_LIST);

    int result = -1;
    size_t interface_num = 0;

    // 1. create UDP socket
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        fprintf(stderr, "ERROR: create UDP socket failed, errno = %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    // 2. get ifconfig
    char buffer[BUFFER_LEN] = {};
    struct ifconf ifc = {
        .ifc_len = sizeof(buffer),
        .ifc_buf = (caddr_t) buffer
    };

    if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) {
        fprintf(stderr, "ERROR: ioctl SIOCGIFCONF failed, errno = %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    // 3. set interface
    size_t i;
    struct ifreq * ifr;
    for (i = 0; i < ifc.ifc_len && interface_num < interface_size; i += _SIZEOF_ADDR_IFREQ(*ifr)) {
        ifr = (struct ifreq *)(buffer + i);
        if (ifr->ifr_addr.sa_family != AF_INET) {
            // parse IPv4 only
            continue;
        }

        // get interface ip string
        char ip[IP_LEN] = {};
        struct sockaddr_in * addr = (struct sockaddr_in *) &ifr->ifr_addr;
        if (inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip)) == NULL) {
            fprintf(stderr, "ERROR: inet_ntop failed, errno = %s (%d)\n", strerror(errno), errno);
            goto end;
        }

        // get network mask
        struct ifreq netmask = {};
        strcpy(netmask.ifr_name, ifr->ifr_name);
        if (ioctl(fd, SIOCGIFNETMASK, &netmask) != 0) {
            fprintf(stderr, "ERROR: ioctl SIOCGIFNETMASK failed, errno = %s (%d)\n", strerror(errno), errno);
            goto end;
        }

        // set name, ip, addr
        strcpy(interface[interface_num].name, ifr->ifr_name);       // name
        strcpy(interface[interface_num].ip, ip);                    // ip string
        interface[interface_num].addr = addr->sin_addr.s_addr;      // address in network byte order

        // set netmask
        addr = (struct sockaddr_in *) &netmask.ifr_addr;
        interface[interface_num].netmask = addr->sin_addr.s_addr;   // mask in network byte order

        // increase interface number
        interface_num++;
    }

    result = 0;
end:
    // close socket
    if (fd >= 0 && close(fd) != 0) {
        fprintf(stderr, "ERROR: close fd %d failed, errno = %s (%d)\n", fd, strerror(errno), errno);
    }

    // failure
    if (result == -1) {
        memset(interface, 0, SIZE_OF_INTERFACE_LIST);
        return -1;
    }

    // success
    return interface_num;
}
