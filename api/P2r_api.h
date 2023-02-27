#ifndef _P2R_API_H
#define _P2R_API_H

typedef enum enum_TerminateReason
{
    UNKNOWN = 0,
    PLANNED = 1,
    UNPLANNED = 2
} enum_TerminateReason;

typedef struct _ConnectionId
{
    struct _ProtocolVersion
    {
        int major;
        int minor;
    } ProtocolVersion;
    int fp_id;
    int rm_id;
} struct_ConnectionId;

typedef struct _TerminateWarning
{
    struct_ConnectionId connection_id;
    enum_TerminateReason reason;
    int warning_id;
    int timeout;
} struct_TerminateWarning;

typedef struct _TerminateCancel
{
    struct_ConnectionId connection_id;
    int warning_id;
} struct_TerminateCancel;

typedef struct _RestoreWarning
{
    struct_ConnectionId connection_id;
    int timeout;
} struct_RestoreWarning;

typedef void (*SpeedNotificationCallback)(float speed);
typedef void (*SessionTerminationCallback)(struct_TerminateWarning termination);
typedef void (*SessionTerminationCancelCallback)(struct_TerminateCancel cancelation);
typedef void (*SessionRestoreCallback)(struct_RestoreWarning restoration);

typedef struct _ApiCallTable
{
    SpeedNotificationCallback speed_notify_callback;
    SessionTerminationCallback terminate_callback;
    SessionTerminationCancelCallback terminate_cancel_callback;
    SessionRestoreCallback restore_callback;
} ApiCallTable;

#endif