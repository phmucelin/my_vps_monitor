#ifndef MONITOR_H
#define MONITOR_H

#include "../include/disk.h"
#include "../include/memory.h"
#include "../include/docker.h"

typedef struct {
    int cpu_high_count;
    int memory_high_count;
    int disk_high_count;
} MonitorState;

int monitor_evaluate(
    double cpu_usage,
    const memory_metrics *memory,
    const disk_metrics *disk
);

#endif