#include <stdio.h>
#include <stdlib.h>
#include "../include/cpu.h"

int cpu_read_sample(cpu_sample *sample){
  FILE *fp = fopen("/proc/stat", "r");
  if(!fp) {
    return -1;
  }
  
  unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
  int read_infos = fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
  if(read_infos != 8) {
    return -1;
  }
  sample->total = user + nice + system + idle + iowait + irq + softirq + steal;
  sample->idle = idle + iowait;
  
  
  fclose(fp);
  return 0;
}


double cpu_calculate_usage_cpu(const cpu_sample *prev, const cpu_sample *curr) {
    unsigned long long total_delta = curr->total - prev->total;
    unsigned long long idle_delta = curr->idle - prev->idle;

    if (total_delta == 0) {
        return 0.0;
    }

    double usage = 100 * (1.0 - (double)(idle_delta) / total_delta);
    return usage;
}