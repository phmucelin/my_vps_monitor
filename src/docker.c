#include <stdio.h>
#include "../include/docker.h"

int docker_get_metrics(docker_metrics *metrics)
{
    if (!metrics)
        return -1;
        
    metrics->count = 0;

    FILE *fp = popen(
        "docker ps -a --format \"{{.ID}}|{{.Names}}|{{.Status}}|{{.Image}}\"",
        "r"
    );

    if (!fp)
        return -1;

    char line[512];

    while (
        metrics->count < 100 &&
        fgets(line, sizeof(line), fp)
    ) {
        docker_container *container =
            &metrics->containers[metrics->count];
        int fields = sscanf(
            line,
            "%63[^|]|%127[^|]|%255[^|]|%255[^\n]",
            container->id,
            container->name,
            container->status,
            container->image
        );
        if (fields == 4) {
            metrics->count++;
        }
    }
    pclose(fp);
    return 0;
}