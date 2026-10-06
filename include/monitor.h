#ifndef MONITOR_H
#define MONITOR_H

typedef struct {
    int cpu_high_count;
    int memory_high_count;
    int disk_high_count;
} MonitorState;

#endif