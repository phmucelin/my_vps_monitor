#include <stdio.h>
#include <sys/statvfs.h>
#include "../include/disk.h"

int disk_get_metrics(const char *path, disk_metrics *metrics){
      
  if (!path || !metrics)
      return -1;

  struct statvfs info;

  if(statvfs(path, &info) != 0)
      return -1;
        
    metrics->total_bytes =
        (unsigned long long)info.f_blocks *
        (unsigned long long)info.f_frsize;

    metrics->available_bytes =
        (unsigned long long)info.f_bavail *
        (unsigned long long)info.f_frsize;

    metrics->used_bytes =
        ((unsigned long long)info.f_blocks -
         (unsigned long long)info.f_bfree) *
        (unsigned long long)info.f_frsize;

    if (metrics->total_bytes == 0)
        return -1;

    metrics->usage_percent =
        ((double)metrics->used_bytes /
         (double)metrics->total_bytes) * 100.0;

  
  return 0;
}