typedef struct {
  unsigned long long total;
  unsigned long long idle;
} cpu_sample;

typedef struct {
  double usage_percent;
} cpu_metrics;

int cpu_read_sample(cpu_sample *sample);

double cpu_calculate_usage_cpu(const cpu_sample *prev, const cpu_sample *curr);