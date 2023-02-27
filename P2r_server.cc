#include <bits/stdc++.h>
#include <grpcpp/grpcpp.h>

#include <string>

#include "protobuf/classes/P2r.grpc.pb.h"

#include "api/P2r_api.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using p2r::ConnectionId;
using p2r::Response;
using p2r::RestoreWarning;
using p2r::SpeedNotification;
using p2r::TerminateCancel;
using p2r::TerminateWarning;


class P2rServer final : public p2r::P2R::Service
{

  private:
    ApiCallTable api_callbacks;

  public:
  void SetApiCallbacks( ApiCallTable callbacks ) {
    if( callbacks.speed_notify_callback ) {
      api_callbacks.speed_notify_callback = callbacks.speed_notify_callback;
    }
  }

  Status P2rTerminateWarning(ServerContext *context, const TerminateWarning *request, Response *response) override  {
    response->set_cause(p2r::SUCCESS);
    return Status::OK;
  }
  Status P2rTerminateWarningCancel(ServerContext *context, const TerminateCancel *request, ::google::protobuf::Empty *response) override
  {
    return Status::OK;
  }
  Status P2rRestoreWarning(ServerContext *context, const RestoreWarning *request, ::google::protobuf::Empty *response) override
  {
    return Status::OK;
  }
  Status P2rSpeedLevelNotification(ServerContext *context, const SpeedNotification *request, ::google::protobuf::Empty *response) override
  {
    api_callbacks.speed_notify_callback(request->speed());
    return Status::OK;
  }
};

extern "C" 
{
  int RunServer(char *addr, ApiCallTable *callbacks)
  {
    if (!addr)
    {
      return -1;
    }
    if (!callbacks || !callbacks->speed_notify_callback)
    {
      return -2;
    }
    std::string server_address(addr);

    P2rServer service;
    service.SetApiCallbacks(*callbacks);

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on port: " << server_address << std::endl;

    server->Wait();
    return 0;
  }
}
  /*ApiCallTable callbacks;
  void NotificationCallback(float speed) {
    printf("Got speed notification: %2f\n", speed);
  }

  int main(int argc, char** argv) {
    callbacks.speed_notify_callback = NotificationCallback;
    RunServer("0.0.0.0:50051", &callbacks );
    return 0;
  }*/