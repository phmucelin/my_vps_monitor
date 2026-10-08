#include <stdio.h>

#include "../include/monitor.h"
#include "../include/config.h"

int monitor_evaluate(
    double cpu_usage,
    const memory_metrics *memory,
    const disk_metrics *disk,
    const docker_metrics *docker,
    MonitorState* state
)
{
  if(cpu_usage || memory || disk == NULL) return -1;
  
  // Validacoa se a esta atingindo o limite estipulado em config.h para validar 
  // se sera necessario atencao ou nao dentro de cada servico.
  
  if(cpu_usage >= CPU_THRESHOLD){
    state->cpu_high_count++;
  }else {
    state->cpu_high_count=0;
  }
  if(memory->usage_percent >= MEMORY_THRESHOLD) {
        state->memory_high_count++;
    } else {
        state->memory_high_count = 0;
    }
    if (disk->usage_percent >= DISK_THRESHOLD) {
        state->disk_high_count++;
    } else {
        state->disk_high_count = 0;
    }
    
    // Aqui vai o for validando os containers tambem, para enviar avisos caso esteja parado.
    for(int i = 0; i < docker->count; i++)
    {
      const docker_container *container = &docker->containers[i];
      
      if (!container->running) {
        switch (container->criticality) {
            case RISK_LOW:
                printf("[LOW] %s parou\n", container->name);
                break;

            case RISK_MEDIUM:
                printf("[MEDIUM] %s parou\n", container->name);
                break;

            case RISK_HIGH:
                printf("[HIGH] %s parou\n", container->name);
                break;

            case RISK_CRITICAL:
                printf("[CRITICAL] %s parou\n", container->name);
                break;

            default:
                printf("[UNKNOWN] %s parou\n", container->name);
                break;
        }
      }
    }
    
    // Alertas de falhas disparados apenas se atingir 3x FAILURE_THRESHOLD
    if (state->cpu_high_count == FAILURE_THRESHOLD) {
        printf("[WARNING] CPU acima do limite!\n");
    }

    if (state->memory_high_count == FAILURE_THRESHOLD) {
        printf("[WARNING] Memoria acima do limite!\n");
    }

    if (state->disk_high_count == FAILURE_THRESHOLD) {
        printf("[WARNING] Disco acima do limite!\n");
    }

    return 0;
}