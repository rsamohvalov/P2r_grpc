#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "api/P2r_client_api.h"

void *ClientThread1(void *param)
{
    // void *context = NULL;
    ret_val ret = SendP2RSessionTerminationWarning(10, 555, (int)PLANNED);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);

    ret = P2rClientInit(1, "127.0.0.1", 50051);
    if (ret != Success)
    {
        return 0;
    }
    ret = SendP2RSessionTerminationWarning(10, 555, (int)PLANNED);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSessionTerminationWarningCancel(555);
    printf("SendP2RSessionTerminationWarningCancel ret = %d\n", ret);
    // sleep(1);
    ret = SendP2RSessionTerminationWarning(0, 556, (int)UNPLANNED);
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
    ret = SendP2RSessionTerminationWarning(10, 557, (int)PLANNED);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    // sleep(1);
    // ret = P2rClientRelease();
    // printf("P2rClientRelease ret = %d\n", ret);

    ret = SendP2RSpeedLevelNotification(100.4);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);

    ret = P2rClientInit(2, "127.0.0.1", 50051);
    if (ret != Success)
    {
        printf("P2rClientInit ret = %d\n", ret);
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
    ClientThread1(NULL);
    
    return 0;
}