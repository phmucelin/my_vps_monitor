typedef struct{
  unsigned long total_kb;
  unsigned long free_kb;
  unsigned long available_kb; //total-free
  
  double usage_percent;
} memory_metrics;

int memory_get_metrics(memory_metrics *metrics);