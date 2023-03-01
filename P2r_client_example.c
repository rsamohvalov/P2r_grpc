#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "api/P2r_client_api.h"

ret_val StartStopClient() {
    printf("StartStopClient\n");
    ret_val ret = P2rClientInit(55, "127.0.0.1", 50051);
    printf("P2rClientInit ret = %d\n", ret);
    if (ret != Success )
    {
        return ret;
    }  

    ret = P2rClientRelease();
    printf("P2rClientRelease ret = %d\n", ret);
    return ret;
}

ret_val SendAllMessages() {
    printf("SendAllMessages\n");
    ret_val ret = P2rClientInit(1, "127.0.0.1", 50051);
    printf("P2rClientInit ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    ret = SendP2RSessionTerminationWarning(10, 555, (int)PLANNED);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    ret = SendP2RSessionTerminationWarningCancel(555);
    printf("SendP2RSessionTerminationWarningCancel ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    ret = SendP2RSessionTerminationWarning(0, 556, (int)UNPLANNED);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    ret = SendP2RSessionRestoreWarning(100);
    printf("SendP2RSessionRestoreWarning ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    ret = SendP2RSpeedLevelNotification(123.4);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    ret = SendP2RSpeedLevelNotification(155.6);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    ret = SendP2RSpeedLevelNotification(255.4);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    ret = SendP2RSpeedLevelNotification(100.7);
    printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    ret = SendP2RSessionTerminationWarning(10, 557, (int)PLANNED);
    printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    } 
    ret = P2rClientRelease();
    printf("P2rClientRelease ret = %d\n", ret);
    return ret;
}

ret_val Send100Times()
{
    ret_val ret = Error;
    printf("Send100Times\n");
    ret = P2rClientInit(1, "127.0.0.1", 50051);
    printf("P2rClientInit ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    for (int i = 0; i < 100; i++)
    {
        printf("Sen100Times %d\n", i );
        
        ret = SendP2RSessionTerminationWarning(10, 555, (int)PLANNED);
        printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
        if (ret != Success)
        {
            return ret;
        }
        ret = SendP2RSessionTerminationWarningCancel(555);
        printf("SendP2RSessionTerminationWarningCancel ret = %d\n", ret);
        if (ret != Success)
        {
            return ret;
        }
        ret = SendP2RSessionTerminationWarning(0, 556, (int)UNPLANNED);
        printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
        if (ret != Success)
        {
            return ret;
        }
        ret = SendP2RSessionRestoreWarning(100);
        printf("SendP2RSessionRestoreWarning ret = %d\n", ret);
        if (ret != Success)
        {
            return ret;
        }
        ret = SendP2RSpeedLevelNotification(123.4);
        printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
        if (ret != Success)
        {
            return ret;
        }
        ret = SendP2RSpeedLevelNotification(155.6);
        printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
        if (ret != Success)
        {
            return ret;
        }
        ret = SendP2RSpeedLevelNotification(255.4);
        printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
        if (ret != Success)
        {
            return ret;
        }
        ret = SendP2RSpeedLevelNotification(100.7);
        printf("SendP2RSpeedLevelNotification ret = %d\n", ret);
        if (ret != Success)
        {
            return ret;
        }
        ret = SendP2RSessionTerminationWarning(10, 557, (int)PLANNED);
        printf("SendP2RSessionTerminationWarning ret = %d\n", ret);
        if (ret != Success)
        {
            return ret;
        }        
    }
    ret = P2rClientRelease();
    printf("P2rClientRelease ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    return ret;
}

ret_val MemoryTest() {
    ret_val ret = Error;
    printf("MemoryTest\n");
    for (int i = 0; i < 10000; i++)
    {
        printf("MemoryTest %d\n", i );
        ret = Send100Times();
        if (ret != Success)
        {
            return ret;
        }
    }
    return ret;
}

ret_val SendWithTimeout() {
    ret_val ret = Error;
    ret = SendP2RSpeedLevelNotification(987654321);
    printf("SendP2RSessionRestoreWarning ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    sleep(120);
    ret = SendP2RSpeedLevelNotification(100500);
    printf("SendP2RSessionRestoreWarning ret = %d\n", ret);
    if (ret != Success)
    {
        return ret;
    }
    return ret;
}

int main(int argc, char *argv[])
{
    ret_val ret;
    
    ret = StartStopClient();
    if (ret != Success)
    {
        printf("error\n");
        getc(stdin);
        return -1;
    }
    ret = SendAllMessages();
    if (ret != Success)
    {
        printf("error\n");
        getc(stdin);
        return -1;
    }
    ret = Send100Times();
    if (ret != Success)
    {
        printf("error\n");
        getc(stdin);
        return -1;
    }

    ret = MemoryTest();
    if (ret != Success)
    {
        printf("error\n");
        getc(stdin);
        return -1;
    }

    ret = P2rClientInit(1, "127.0.0.1", 50051);
    printf("P2rClientInit ret = %d\n", ret);
    if (ret != Success)
    {
        printf("error\n");
        getc(stdin);
        return -1;
    }

    ret = SendWithTimeout();
    if (ret != Success)
    {
        printf("error\n");
        getc(stdin);
        return -1;
    }

    ret = P2rClientRelease();
    printf("P2rClientRelease ret = %d\n", ret);
    if (ret != Success)
    {
        printf("error\n");
        getc(stdin);
        return -1;
    }
    printf("finished\n");
    getc(stdin);
    return 0;
}