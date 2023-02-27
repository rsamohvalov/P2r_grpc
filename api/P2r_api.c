#include "P2r_api.h"

int SendSpeedNotification(float speed, void *context);
int RunServer(char *addr, ApiCallTable *callbacks);
void *RunClient(char *addr);