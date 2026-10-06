#include <stdio.h>
#include "../include/memory.h"

int memory_get_metrics(memory_metrics *metrics){
  
  FILE *fp = fopen("/proc/meminfo", "r");
  
  if(!fp){
    return -1;
  }
  
  char line[256];

  while (fgets(line, sizeof(line), fp)) {
      if (sscanf(line, "MemTotal: %lu kB", &metrics->total_kb) == 1) {
          continue;
      }

      if (sscanf(line, "MemAvailable: %lu kB", &metrics->available_kb) == 1) {
          continue;
      }
  }
  
  long used = metrics->total_kb - metrics->available_kb;
  
  metrics->usage_percent= ((double)used/metrics->total_kb) * 100;
  
  fclose(fp);
  return 0;
}