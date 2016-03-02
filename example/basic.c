#include <stdio.h>
#include "network_interface.h"

int main() {
    struct network_interface interface[16] = {};
    int num = network_interface_get(interface, 16);
    if (num == -1) {
        return -1;  // Error
    }

    printf("\nIPv4 Network Interface (%d):\n", num);
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
    return 0;
}
