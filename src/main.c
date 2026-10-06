#include <stdio.h>
#include <unistd.h>

#include "../include/cpu.h"
#include "../include/memory.h"
#include "../include/disk.h"

int main(void)
{
    cpu_sample prev;
    cpu_sample curr;

    memory_metrics memory;
    disk_metrics disk;

    // CPU - primeira leitura
    if (cpu_read_sample(&prev) != 0) {
        printf("Erro lendo CPU\n");
        return 1;
    }

    sleep(1);

    // CPU - segunda leitura
    if (cpu_read_sample(&curr) != 0) {
        printf("Erro lendo CPU\n");
        return 1;
    }

    double cpu_usage = cpu_calculate_usage_cpu(&prev, &curr);

    // Memoria RAM
    if (memory_get_metrics(&memory) != 0) {
        printf("Erro lendo memoria\n");
        return 1;
    }

    // Disco
    if (disk_get_metrics("/", &disk) != 0) {
        printf("Erro lendo disco\n");
        return 1;
    }

    // Resultados
    printf("CPU Usage:    %.2f%%\n", cpu_usage);
    printf("Memory Usage: %.2f%%\n", memory.usage_percent);
    printf("Disk Usage:   %.2f%%\n", disk.usage_percent);

    return 0;
}