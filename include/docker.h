typedef struct{
  char id[64];
  char name[128];
  char status[256];
  char image[256];
  
  int running;

}docker_container;

typedef struct {
    docker_container containers[100];
    int count;
} docker_metrics;


int docker_get_metrics(docker_metrics* metrics);