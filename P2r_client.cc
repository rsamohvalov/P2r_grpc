#include <grpcpp/grpcpp.h>

#include <string>

#include "protobuf/classes/P2r.grpc.pb.h"

#include "api/P2r_api.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using p2r::ConnectionId;
using p2r::ConnectionId_ProtocolVersion;
using p2r::Response;
using p2r::RestoreWarning;
using p2r::SpeedNotification;
using p2r::TerminateCancel;
using p2r::TerminateReason;
using p2r::TerminateWarning;

class P2rClient
{
public:
  P2rClient(std::shared_ptr<Channel> channel, int id)
      : stub_(p2r::P2R::NewStub(channel)) {
    connId.set_fp_id(id);
    protoVersion.set_major(1);
    protoVersion.set_minor(0);
    connId.set_allocated_protocol_version(&protoVersion);
  }

  ::grpc::Status SendP2RSpeedLevelNotification(double speed)
  {
    SpeedNotification speed_notify;
    speed_notify.set_allocated_connection_id(&connId);
    speed_notify.set_speed((float)speed);

    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    return stub_->P2rSpeedLevelNotification(&context, speed_notify, &response);
  }

  ::grpc::Status SendP2RSessionTerminationWarning( long time, long warning_id, int reason) {
    TerminateWarning terminate;
    terminate.set_warning_id(warning_id);
    terminate.set_timeout(time);    
    terminate.set_reason((TerminateReason)reason);
    terminate.set_allocated_connection_id(&connId);
    ::grpc::ClientContext context;
    Response response;
    ::grpc::Status status = stub_->P2rTerminateWarning(&context, terminate, &response);
    if (status.ok())
    {
      if (response.cause() == p2r::Cause::SUCCESS)
      {
        rm_id = response.connection_id().rm_id();
        return grpc::Status::OK;
      }
    }
    return status;
  }

  ::grpc::Status SendP2RSessionTerminationWarningCancel(long warning_id)
  {
    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    TerminateCancel cancel;
    cancel.set_allocated_connection_id(&connId);
    cancel.set_warning_id(warning_id);
    return stub_->P2rTerminateWarningCancel(&context, cancel, &response);
  }

  ::grpc::Status SendP2RSessionRestoreWarning(long time) {
    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    RestoreWarning restore;
    restore.set_timeout(time);
    restore.set_allocated_connection_id(&connId);
    return stub_->P2rRestoreWarning(&context, restore, &response);
  }

private:
  std::unique_ptr<p2r::P2R::Stub> stub_;
  ConnectionId_ProtocolVersion protoVersion;
  ConnectionId connId;
  int fp_id;
  int rm_id;
};

P2rClient *client = NULL;
extern "C" 
{

  ret_val P2rClientInit(long id, const char *server, unsigned short port )
  //void *RunClient(char *addr, int fp_id)
  {
    if( client ) {
      return ret_val::TransportInitError;
    }
    std::string target_address(server);
    try
    {
      client = new P2rClient(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()), id);
    }
    catch (...)
    {
      return ret_val::Error;
    }
    return ret_val::Success;
  }

  ret_val P2rClientRelease()
  {
    if (client)
    {
      delete client;
    }
    return ret_val::Success;
  }

  ret_val SendP2RSpeedLevelNotification(double speed)
  //  int SendSpeedNotification(struct_SpeedNotification speed, void *context)
  {
    if (!client)
    {
      return ret_val::Error;
    }
    //::grpc::Status status = ((P2rClient *)context)->SendSpeedNotification(speed);
    ::grpc::Status status = client->SendP2RSpeedLevelNotification(speed);
    if (status.ok())
    {
      return ret_val::Success;
    }
    return ret_val::ServerIsUnreachable;
  }

  ret_val SendP2RSessionTerminationWarning(long time, long warning_id, int reason)
  //int SendTermination(struct_TerminateWarning termination, void *context)
  {
    if (!client)
    {
      return ret_val::Error;
    }
    //::grpc::Status status = ((P2rClient *)context)->SendTermination(termination);
    ::grpc::Status status = client->SendP2RSessionTerminationWarning( time, warning_id, reason);
    if (status.ok())
    {
      return ret_val::Success;
    }
    if (status.error_code() == grpc::DEADLINE_EXCEEDED || status.error_code() == grpc::UNAVAILABLE)
    {
      return ret_val::ServerIsUnreachable;
    }
    return ret_val::Error;
  }

  ret_val SendP2RSessionTerminationWarningCancel(long warning_id)
  // int SendTerminationCancel(struct_TerminateCancel cancelation, void *context)
  {
    if (!client)
    {
      return ret_val::Error;
    }
    //::grpc::Status status = ((P2rClient *)context)->SendTerminationCancel(cancelation);
    ::grpc::Status status = client->SendP2RSessionTerminationWarningCancel(warning_id);
    if (status.ok())
    {
      return ret_val::Success;
    }
    return ret_val::ServerIsUnreachable;
  }

  ret_val SendP2RSessionRestoreWarning(long time) 
  //int SendRetore(struct_RestoreWarning restoration, void *context)
  {
    if (!client)
    {
      return ret_val::Error;
    }
    //::grpc::Status status = ((P2rClient *)context)->SendRetore(restoration);
    ::grpc::Status status = client->SendP2RSessionRestoreWarning(time);
    if (status.ok())
    {
      return ret_val::Success;
    }
    return ret_val::ServerIsUnreachable;
  }
}
