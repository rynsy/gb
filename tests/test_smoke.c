#include "gb/gb_core.h"
#include <stdio.h>

int main(void) {
    if (gb_core_version() != 1) {
        return 1;
    }
    printf("gb_core_version ok\n");
    return 0;
}
