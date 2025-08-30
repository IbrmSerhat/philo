#include "pthread.h"
#include "stdio.h"

int g_global = 0;
pthread_mutex_t g_mutex;


void *func()
{
    int i;

    i = 0;
    while (i < 500000)
    {
        pthread_mutex_lock(&g_mutex);
        i++;
        g_global++;
        pthread_mutex_unlock(&g_mutex);
    }
}

int main()
{
    pthread_t th[2];
    int i;

    i = 0;
    pthread_mutex_init(&g_mutex, NULL);
    while (i < 2)
    {
        pthread_create(&th[i], NULL, func, NULL);
        i++;
    }
    i = 0;
    while (i < 2)
    {
        pthread_join(th[i], NULL);
        i++;
    }
    pthread_mutex_destroy(&g_mutex);
    printf("%d\n", g_global);

}