#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "api/P2r_server_api.h"

ApiCallTable callbacks;

void NotificationCallback(struct_SpeedNotification speed)
{
    printf("Server Got speed notification from %d: %2f\n", speed.fp_id, speed.speed);
}

int TerminationCallback(struct_TerminateWarning termination)
{
    printf("Server Got TerminationCallback from %d: %d\n", termination.fp_id, termination.warning_id);
    return 0;
}

void TerminationCancelCallback(struct_TerminateCancel cancelation)
{
    printf("Server Got TerminationCancelCallback from %d: %d\n", cancelation.fp_id, cancelation.warning_id);
}

void RestoreCallback(struct_RestoreWarning restoration)
{
    printf("Server Got RestoreCallback from %d: %d\n", restoration.fp_id, restoration.timeout);
}

void *ServerThread(void *param)
{
    if (!param) {
        return 0;
    }
    InitServer(((ServerParameters *)param)->addr, ((ServerParameters *)param)->port, ((ServerParameters *)param)->id, ((ServerParameters *)param)->callbacks);
    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t server_thread;
    ServerParameters params;
    params.id = 111;
    params.port = 50051;
    strcpy(params.addr, "0.0.0.0");
    callbacks.speed_notify_callback = NotificationCallback;
    callbacks.terminate_callback = TerminationCallback;
    callbacks.terminate_cancel_callback = TerminationCancelCallback;
    callbacks.restore_callback = RestoreCallback;
    params.callbacks = &callbacks;
    if (pthread_create(&server_thread, NULL, ServerThread, &params) < 0)
    {
        perror("could not create thread");
    }
    
    //Какой-то код   
    /////////////

    //Стоп
    //ret_val ret = StopServer();
    //printf("StopServer ret = %d\n", ret);

    pthread_join(server_thread, 0);
    return 0;
}