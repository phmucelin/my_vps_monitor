typedef struct{
  unsigned long long total_bytes;
  unsigned long long used_bytes;
  unsigned long long available_bytes;
  
  double usage_percent;
} disk_metrics;

int disk_get_metrics(const char *path, disk_metrics *metrics);