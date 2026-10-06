#include <stdio.h>
#include <sys/statvfs.h>
#include "../include/disk.h"

int disk_get_metrics(const char *path, disk_metrics *metrics){
      
  if (!path || !metrics)
      return -1;

  struct statvfs info;

  if(statvfs(path, &info) != 0)
      return -1;
        
  metrics->total_bytes = info.f_blocks * info.f_frsize;
  metrics->available_bytes = info.f_bavail * info.f_frsize;
  
  metrics->used_bytes = metrics->total_bytes - (info.f_bfree * info.f_frsize);
  
  metrics->usage_percent = (metrics->used_bytes / metrics->total_bytes) * 100;
  
  return 0;
}