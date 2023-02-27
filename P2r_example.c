#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "api/P2r_api.h"

ApiCallTable callbacks;

int SendSpeedNotification(struct_SpeedNotification speed, void *context);
void *RunClient(char *addr, int fp_id);

int RunServer(char *addr, ApiCallTable *callbacks, int id);

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


void* ServerThread(void* param) {
    callbacks.speed_notify_callback = NotificationCallback;
    callbacks.terminate_callback = TerminationCallback;
    callbacks.terminate_cancel_callback = TerminationCancelCallback;
    callbacks.restore_callback = RestoreCallback;
    RunServer("0.0.0.0:50051", &callbacks, 1);
}

void *ClientThread(void *param)
{
    void *context = RunClient("0.0.0.0:50051", 1);
    if (!context)
    {
        return 0;
    }
    struct_SpeedNotification speed;
    speed.speed = 100;
    SendSpeedNotification(speed, context);
    speed.speed = 200;
    SendSpeedNotification(speed, context);
}

int main(int argc, char *argv[])
{

    ////////////////////////
    {
        pthread_t server_thread;
        unsigned short port = 6666;
        if (pthread_create(&server_thread, NULL, ServerThread, &port) < 0)
        {
            perror("could not create thread");
        }
    }
    sleep(1);
    {
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, ClientThread, NULL) < 0)
        {
            perror("could not create thread");
        }
        else
        {
            pthread_join(client_thread, 0);
        }
    }
    return 0;
    ///////////////////////
}