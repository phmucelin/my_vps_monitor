#ifndef CONFIG_H
#define CONFIG_H

#define CHECK_INTERVAL 20

#define CPU_THRESHOLD 87.0
#define DISK_THRESHOLD 80
#define MEMORY_THRESHOLD 90

#define FAILURE_THRESHOLD 4

#define ALERT_COOLDOWN 250

typedef enum {
    RISK_LOW = 1,
    RISK_MEDIUM = 2,
    RISK_HIGH = 3,
    RISK_CRITICAL = 4
} risk_level;

#endif