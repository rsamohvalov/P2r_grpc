#include <grpcpp/grpcpp.h>

#include <string>

#include "protobuf/classes/P2r.grpc.pb.h"

#include "api/P2r_api.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using p2r::ConnectionId;
using p2r::Response;
using p2r::RestoreWarning;
using p2r::SpeedNotification;
using p2r::TerminateCancel;
using p2r::TerminateWarning;

class P2rClient
{
public:
  P2rClient(std::shared_ptr<Channel> channel)
      : stub_(p2r::P2R::NewStub(channel)) {}
  
  ::grpc::Status SendSpeedNotification(const float speed)
  {
    SpeedNotification speed_notify;
    speed_notify.set_speed(speed);
    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    return stub_->P2rSpeedLevelNotification(&context, speed_notify, &response );
  }

private:
  std::unique_ptr<p2r::P2R::Stub> stub_;
};

extern "C" 
{
  P2rClient *client;

  void *RunClient(char *addr)
  {
    std::string target_address(addr);
    try
    {
      client = new P2rClient(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));
    }
    catch (...)
    {
      return 0;
    }
    return client;
  }

  int SendSpeedNotification(float speed, void *context)
  {
    if (!context)
    {
      return -1;
    }
    ::grpc::Status status = ((P2rClient *)context)->SendSpeedNotification(speed);
    if (1)
    {
      return 0;
    }
    return -2;
  }
}
  /*
  int main(int argc, char *argv[])
  {
    void* context = RunClient("0.0.0.0:50051");
    if( !context ) {
      return -1;
    }
    SendSpeedNotification(100, context);
    return 0;
  }
  */