#include <stdio.h>
#include "api/P2r_api.h"

ApiCallTable callbacks;

int SendSpeedNotification(float speed, void *context);
int RunServer(char *addr, ApiCallTable *callbacks);
void *RunClient(char *addr);

void NotificationCallback(float speed)
{
    printf("Got speed notification: %2f\n", speed);
}

int main(int argc, char *argv[])
{
    callbacks.speed_notify_callback = NotificationCallback;
    RunServer("0.0.0.0:50051", &callbacks);
     

    void *context = RunClient("0.0.0.0:50051");
    if (!context)
    {
        return -1;
    }
    SendSpeedNotification(100, context);
    return 0;
}