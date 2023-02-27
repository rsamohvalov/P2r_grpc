#ifndef _P2R_API_H
#define _P2R_API_H

typedef void (*SpeedNotificationCallback)(float speed);

typedef struct _ApiCallTable
{
    SpeedNotificationCallback speed_notify_callback;
} ApiCallTable;

#endif