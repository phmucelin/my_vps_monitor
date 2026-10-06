#include <stdio.h>
#include <unistd.h>

#include "cpu.h"

int main(void)
{
    cpu_sample prev;
    cpu_sample curr;

    if (cpu_read_sample(&prev) != 0) {
        printf("Erro lendo CPU\n");
        return 1;
    }

    sleep(1);

    if (cpu_read_sample(&curr) != 0) {
        printf("Erro lendo CPU\n");
        return 1;
    }

    double usage = cpu_calculate_usage_cpu(&prev, &curr);

    printf("CPU Usage: %.2f%%\n", usage);

    return 0;
}