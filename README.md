# network-interface [![Build Status](https://travis-ci.org/zlargon/network-interface.svg?branch=master)](https://travis-ci.org/zlargon/network-interface)

Get IPv4 addr and netmask

## Support Platform

- Linux
- OSX
- Android
- iOS

## Usage

```c
struct network_interface interface[16] = {};
int num = network_interface_get(interface, 16);

printf("Network Interface (%d):\n", num);
int i;
for (i = 0; i < num; i++) {
    printf("%s: %s\n", interface[i].name, interface[i].ip);
}

/* Output:
 *
 * Network Interface (2):
 * lo0: 127.0.0.1
 * en1: 192.168.31.167
 */
```

## License

MIT
