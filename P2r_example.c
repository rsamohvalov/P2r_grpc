#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "api/P2r_api.h"
#include "api/P2r_client_api.h"

ApiCallTable callbacks;

//int SendSpeedNotification(struct_SpeedNotification speed, void *context);
//void *RunClient(char *addr, int fp_id);

/*ret_val P2rClientInit(long id, const char *server, unsigned short port);
ret_val P2rClientRelease();
ret_val SendP2RSpeedLevelNotification(double speed);
ret_val SendP2RSessionTerminationWarning(long time, long warning_id, int reason);
ret_val SendP2RSessionTerminationWarningCancel(long warning_id);
ret_val SendP2RSessionRestoreWarning(long time);
*/

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
    SendP2RSessionTerminationWarning(10, 555, 1);
    // sleep(1);
    SendP2RSessionTerminationWarningCancel(555);
    // sleep(1);
    SendP2RSessionTerminationWarning(0, 556, 2);
    // sleep(1);
    SendP2RSessionRestoreWarning(100);
    // sleep(1);
    SendP2RSpeedLevelNotification(123.4);
    // sleep(1);
    SendP2RSpeedLevelNotification(155.6);
    // sleep(1);
    SendP2RSpeedLevelNotification(255.4);
    // sleep(1);
    SendP2RSpeedLevelNotification(100.7);
    // sleep(1);
    SendP2RSessionTerminationWarning(10, 557, 1 );
    // sleep(1);
    P2rClientRelease();
    
    ret = SendP2RSpeedLevelNotification(100.4);
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