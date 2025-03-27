#include <tirpc/rpc/rpc.h>
#include <time.h>
#include <sys/types.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "project.h"

#define MAX_LEN 100

char **get_date_svc(long *arg, struct svc_req *req)
{
    struct tm *timeptr;
    time_t clock;
    static char *ptr;
    static char err[] = "Invalid Response \0";
    static char s[MAX_LEN];

    clock = time(0);
    timeptr = localtime(&clock);
    
    strftime(s, MAX_LEN, "%A, %B %d, %Y", timeptr);
    ptr = s;
    
    return(&ptr);
}

double *get_memory_svc(void *arg, struct svc_req *req) {
    static double percent;
    struct sysinfo memInfo;
    (void)arg;
    (void)req;
    sysinfo(&memInfo);
    long long totalPhysMem = memInfo.totalram;
    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    percent = ((double)physMemUsed / (double)totalPhysMem) * 100;
    return &percent;
}

double *get_cpu_svc(void *arg, struct svc_req *req)
{
    (void)arg; 
    (void)req; 
    static double percent = 0.0;

    FILE *file;
    unsigned long long lastUser, lastNice, lastSystem, lastIdle;
    unsigned long long totalUser, totalNice, totalSystem, totalIdle;
    unsigned long long total, totalIdleDelta;

    file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Failed to open /proc/stat");
        return &percent;
    }
    fscanf(file, "cpu %llu %llu %llu %llu", &lastUser, &lastNice, &lastSystem, &lastIdle);
    fclose(file);

    sleep(2);

    file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Failed to open /proc/stat on second read");
        return &percent;
    }
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalNice, &totalSystem, &totalIdle);
    fclose(file);
    totalUser -= lastUser;
    totalNice -= lastNice;
    totalSystem -= lastSystem;
    totalIdle -= lastIdle;
    total = totalUser + totalNice + totalSystem + totalIdle;
    if (total > 0) {
        totalIdleDelta = totalIdle;
        percent = 100.0 * (total - totalIdleDelta) / total;
    } else {
        percent = 0.0;
    }

    return &percent;
}

double *get_load_procs_svc(void *arg, struct svc_req *req) {
    static double result = 0.0;
    struct sysinfo memInfo;
    (void)arg;
    (void)req;
    sysinfo(&memInfo);
    result = (double)memInfo.loads[0];
    return &result;
}

