#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
//#include "api/P2r_api.h"
#include "api/P2r_client_api.h"
#include "api/P2r_server_api.h"

ApiCallTable callbacks;

void NotificationCallback(struct_SpeedNotification speed)
{
    printf("Server Got speed notification: %2f\n", speed.speed);
}

int TerminationCallback(struct_TerminateWarning termination) {
    printf("Server Got TerminationCallback: %d\n", termination.warning_id);
    return 0;
}

void TerminationCancelCallback(struct_TerminateCancel cancelation) {
    printf("Server Got TerminationCancelCallback: %d\n", cancelation.warning_id);
}

void RestoreCallback(struct_RestoreWarning restoration) {
    printf("Server Got RestoreCallback: %d\n", restoration.timeout);
}


void* ServerThread(void* param) {
    callbacks.speed_notify_callback = NotificationCallback;
    callbacks.terminate_callback = TerminationCallback;
    callbacks.terminate_cancel_callback = TerminationCancelCallback;
    callbacks.restore_callback = RestoreCallback;
    InitServer("0.0.0.0", 50051, 1, &callbacks );
    return 0;
}

void *ClientThread(void *param)
{
    //void *context = NULL;
    ret_val ret = P2rClientInit(1, "127.0.0.1", 50051 );
    if (ret != Success )
    {
        return 0;
    }
    ret = SendP2RSessionTerminationWarning(10, 555, 1);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSessionTerminationWarningCancel(555);
    printf("SendP2RSessionTerminationWarningCancel ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSessionTerminationWarning(0, 556, 2);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSessionRestoreWarning(100);
    printf("SendP2RSessionRestoreWarning ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSpeedLevelNotification(123.4);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSpeedLevelNotification(155.6);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSpeedLevelNotification(255.4);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSpeedLevelNotification(100.7);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSessionTerminationWarning(10, 557, 1);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    // sleep(1);
    ret = P2rClientRelease();
    printf("P2rClientRelease ret = %d\n", ret);

    ret = SendP2RSpeedLevelNotification(100.4);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    return 0;
}

int main(int argc, char *argv[])
{

    ////////////////////////
    
        pthread_t server_thread;
        unsigned short port = 6666;
        if (pthread_create(&server_thread, NULL, ServerThread, &port) < 0)
        {
            perror("could not create thread");
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
            pthread_join(server_thread, 0);
        }
    }
    return 0;
    ///////////////////////
}