#ifndef _P2R_API_H
#define _P2R_API_H

typedef enum enum_TerminateReason
{
    UNKNOWN = 0,
    PLANNED = 1,
    UNPLANNED = 2
} enum_TerminateReason;

typedef enum _ret
{
    Success = 0,
    NotEnoughMemory = 1,
    TransportInitError = 2,
    ServerIsUnreachable = 3,
    EncodingError = 4,
    Error = 5
} ret_val;

#endif