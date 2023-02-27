#include <stdio.h>
#include "api/P2r_api.h"

ApiCallTable callbacks;

int SendSpeedNotification(struct_SpeedNotification speed, void *context);
void *RunClient(char *addr);

int RunServer(char *addr, ApiCallTable *callbacks);

void NotificationCallback(struct_SpeedNotification speed)
{
    printf("Got speed notification: %2f\n", speed.speed);
}

int TerminationCallback(struct_TerminateWarning termination) {

}

void TerminationCancelCallback(struct_TerminateCancel cancelation) {

}

void RestoreCallback(struct_RestoreWarning restoration) {

}

int main(int argc, char *argv[])
{
    callbacks.speed_notify_callback = NotificationCallback;
    callbacks.terminate_callback = TerminationCallback;
    callbacks.terminate_cancel_callback = TerminationCancelCallback;
    callbacks.restore_callback = RestoreCallback;
    RunServer("0.0.0.0:50051", &callbacks);

    void *context = RunClient("0.0.0.0:50051");
    if (!context)
    {
        return -1;
    }
    struct_SpeedNotification speed;
    speed.speed = 100;
    SendSpeedNotification(speed, context);
    speed.speed = 200;
    SendSpeedNotification(speed, context);
    return 0;
}