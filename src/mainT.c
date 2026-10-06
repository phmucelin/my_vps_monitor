#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/alerts.h"
#include "../include/config.h"
#include "../include/cpu.h"
#include "../include/disk.h"
#include "../include/docker.h"
#include "../include/email.h"
#include "../include/memory.h"
#include "../include/monitor.h"

int main(void)
{
    MonitorState state = {0};

    while (1) {

        // coleta
        cpu_metrics cpu;
        memory_metrics memory;
        disk_metrics disk;

        get_cpu_metrics(&cpu);
        memory_get_metrics(&memory);
        disk_get_metrics("/", &disk);

        // avalia
        monitor_evaluate(
            &state,
            &cpu,
            &memory,
            &disk
        );

        sleep(10);
    }
}