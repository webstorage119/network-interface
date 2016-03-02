#include <stdio.h>   // size_t, printf
#include <unistd.h>  // sleep
#include <string.h>  // memcmp, memcpy
#include "network_interface.h"

#define INTERFACE_NUM 16

/*
 * show network interface when it is changed.
 */
int main() {
    struct network_interface last[INTERFACE_NUM] = {};
    const size_t MEM_SIZE = sizeof(struct network_interface) * INTERFACE_NUM;

    for (;; sleep(1)) {
        // 1. get network interface
        struct network_interface interface[INTERFACE_NUM] = {};
        int num = network_interface_get(interface, INTERFACE_NUM);
        if (num == -1) {
            return -1;  // Error
        }

        // 2. compare with the old one
        if (memcmp(last, interface, MEM_SIZE) == 0) {
            // no changed
            puts(".");
            continue;
        }

        // 3. network interface is changed, display the result
        printf("\nNetwork Interface (%d):\n", num);
        size_t i;
        for (i = 0; i < num; i++) {
            printf("%zu. %-6s: %-15s (%d.%d.%d.%d)\n",
                i + 1,
                interface[i].name,
                interface[i].ip,
                (interface[i].netmask >> 0)  & 0xff,
                (interface[i].netmask >> 8)  & 0xff,
                (interface[i].netmask >> 16) & 0xff,
                (interface[i].netmask >> 24) & 0xff
            );
        }
        printf("%s\n", i == 0 ? "Empty\n" : "");

        // 4. copy result to last
        memcpy(last, interface, MEM_SIZE);
    }

    return 0;
}
