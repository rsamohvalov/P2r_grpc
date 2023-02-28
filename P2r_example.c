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
    printf("Server Got speed notification from %d: %2f\n", speed.fp_id, speed.speed);
}

int TerminationCallback(struct_TerminateWarning termination) {
    printf("Server Got TerminationCallback from %d: %d\n", termination.fp_id, termination.warning_id);
    return 0;
}

void TerminationCancelCallback(struct_TerminateCancel cancelation) {
    printf("Server Got TerminationCancelCallback from %d: %d\n", cancelation.fp_id, cancelation.warning_id);
}

void RestoreCallback(struct_RestoreWarning restoration) {
    printf("Server Got RestoreCallback from %d: %d\n", restoration.fp_id, restoration.timeout);
}


void* ServerThread(void* param) {
    callbacks.speed_notify_callback = NotificationCallback;
    callbacks.terminate_callback = TerminationCallback;
    callbacks.terminate_cancel_callback = TerminationCancelCallback;
    callbacks.restore_callback = RestoreCallback;
    InitServer("0.0.0.0", 50051, 1, &callbacks );
    return 0;
}

void *ClientThread1(void *param)
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

    ret = P2rClientInit(2, "127.0.0.1", 50051);
    if (ret != Success)
    {
        return 0;
    }
    ret = SendP2RSessionTerminationWarning(100, 333, 1);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSessionTerminationWarningCancel(333);
    printf("SendP2RSessionTerminationWarningCancel ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSessionTerminationWarning(0, 4444, 2);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSessionRestoreWarning(222);
    printf("SendP2RSessionRestoreWarning ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSpeedLevelNotification(111.1);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSpeedLevelNotification(22.22);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSpeedLevelNotification(3.333);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSpeedLevelNotification(134.44);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSessionTerminationWarning(111, 222, 1);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    // sleep(1);
    ret = P2rClientRelease();
    printf("P2rClientRelease ret = %d\n", ret);

    ret = SendP2RSpeedLevelNotification(200.4);
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
        pthread_t client_thread1;
        if (pthread_create(&client_thread1, NULL, ClientThread1, NULL) < 0)
        {
            perror("could not create 1 thread");
        }
        else
        {
            pthread_join(server_thread, 0);
        }
    }
    return 0;
    ///////////////////////
}