#include <stdio.h>   // snprintf
#include <string.h>  // strlen, strcmp
#include "mocha.h"
#include "network_interface.h"

int input_null() {
    int num = network_interface_get(NULL, 16);
    assert(num == -1);
    return 0;
}

int input_size() {
    struct network_interface interface[16] = {};
    int num2 = network_interface_get(interface, 2);
    int num16 = network_interface_get(interface, 16);

    assert(num2  != -1);
    assert(num16 != -1);
    assert(num16 >= num2);
    return 0;
}

int interface_entry_number() {
    struct network_interface interface[16] = {};
    int num = network_interface_get(interface, 16);
    assert(num != -1);

    size_t i;
    for (i = 0; i < num; i++) {
        assert(strlen(interface[i].name) > 0);
        assert(strlen(interface[i].ip)   > 0);
        assert(interface[i].addr    != 0);
        assert(interface[i].netmask != 0);
    }

    for (i = num; i < 16; i++) {
        assert(strlen(interface[i].name) == 0);
        assert(strlen(interface[i].ip)   == 0);
        assert(interface[i].addr    == 0);
        assert(interface[i].netmask == 0);
    }
    return 0;
}

int ip_addr_should_be_same() {
    struct network_interface interface[16] = {};
    int num = network_interface_get(interface, 16);
    assert(num != -1);

    size_t i;
    for (i = 0; i < num; i++) {
        char ip[16] = {};
        snprintf(ip, 16, "%d.%d.%d.%d",
            (interface[i].addr >> 0)  & 0xff,
            (interface[i].addr >> 8)  & 0xff,
            (interface[i].addr >> 16) & 0xff,
            (interface[i].addr >> 24) & 0xff
        );
        assert(strcmp(ip, interface[i].ip) == 0);
    }
    return 0;
}

int main() {
    fclose(stderr); // disable error log

    describe("network_interface_get",
        input_null,
        input_size,
        interface_entry_number,
        ip_addr_should_be_same
    );
    return 0;
}
