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
using p2r::ConnectionId_ProtocolVersion;

    class P2rClient
{
public:
  P2rClient(std::shared_ptr<Channel> channel)
      : stub_(p2r::P2R::NewStub(channel)) {}

  void FillHeader(ConnectionId& id, struct_ConnectionId& s_id, ConnectionId_ProtocolVersion* version)
  {
    id.set_fp_id(s_id.fp_id);
    id.set_rm_id(s_id.rm_id);
    version->set_major(s_id.ProtocolVersion.major);
    version->set_minor(s_id.ProtocolVersion.minor);
    id.set_allocated_protocol_version(version);
  }

  ::grpc::Status SendSpeedNotification(struct_SpeedNotification speed)
  {
    SpeedNotification speed_notify;    
    ConnectionId id;
    p2r::ConnectionId_ProtocolVersion version;
    FillHeader(id, speed.connection_id, &version );
    speed_notify.set_allocated_connection_id(&id);
    speed_notify.set_speed(speed.speed);

    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    return stub_->P2rSpeedLevelNotification(&context, speed_notify, &response );
  }

  ::grpc::Status SendTermination(struct_TerminateWarning termination) {
    ::grpc::ClientContext context;
    TerminateWarning terminate;
    ConnectionId id;
    p2r::ConnectionId_ProtocolVersion version;
    FillHeader(id, termination.connection_id, &version);
    terminate.set_allocated_connection_id(&id);
    Response response;
    return stub_->P2rTerminateWarning(&context, terminate, &response);
  }
  ::grpc::Status SendTerminationCancel(struct_TerminateCancel cancelation ) {
    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    TerminateCancel cancel;
    ConnectionId id;
    p2r::ConnectionId_ProtocolVersion version;
    FillHeader(id, cancelation.connection_id, &version);
    cancel.set_allocated_connection_id(&id);
    return stub_->P2rTerminateWarningCancel(&context, cancel, &response);
  }
  ::grpc::Status SendRetore(struct_RestoreWarning restoration) {
    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    RestoreWarning restore;
    ConnectionId id;
    p2r::ConnectionId_ProtocolVersion version;
    FillHeader(id, restoration.connection_id, &version);
    restore.set_allocated_connection_id(&id);
    return stub_->P2rRestoreWarning(&context, restore, &response);
  }

private:
  std::unique_ptr<p2r::P2R::Stub> stub_;
};

P2rClient *client;
extern "C" 
{ 

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

  int SendSpeedNotification(struct_SpeedNotification speed, void *context)
  {
    if (!context)
    {
      return -1;
    }
    ::grpc::Status status = ((P2rClient *)context)->SendSpeedNotification(speed);
    if (status.ok())
    {
      return 0;
    }
    return -2;
  }

  int SendTermination(struct_TerminateWarning termination, void *context)
  {
    if (!context)
    {
      return -1;
    }
    ::grpc::Status status = ((P2rClient *)context)->SendTermination(termination);
    if (status.ok())
    {
      return 0;
    }
    return -2;
  }

  int SendTerminationCancel(struct_TerminateCancel cancelation, void *context)
  {
    if (!context)
    {
      return -1;
    }
    ::grpc::Status status = ((P2rClient *)context)->SendTerminationCancel(cancelation);
    if (status.ok())
    {
      return 0;
    }
    return -2;
  }

  int SendRetore(struct_RestoreWarning restoration, void *context)
  {
    if (!context)
    {
      return -1;
    }
    ::grpc::Status status = ((P2rClient *)context)->SendRetore(restoration);
    if (status.ok())
    {
      return 0;
    }
    return -2;
  }
}
