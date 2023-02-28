#include <grpcpp/grpcpp.h>

#include <string>

#include "protobuf/classes/P2r.grpc.pb.h"

#include "api/P2r_client_api.h"

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
  ConnectionId_ProtocolVersion *protoVersion;
  ConnectionId *connId;

  P2rClient(std::shared_ptr<Channel> channel, int id)
      : stub_(p2r::P2R::NewStub(channel)) {
    std::cout << "constructor\n";
    connId = new ConnectionId();
    connId->set_fp_id(id);
    protoVersion = new ConnectionId_ProtocolVersion();
    protoVersion->set_major(1);
    protoVersion->set_minor(0);
    connId->set_allocated_protocol_version(protoVersion);
  }

  ::grpc::Status SendP2RSpeedLevelNotification(double speed)
  {
    SpeedNotification speed_notify;
    speed_notify.set_allocated_connection_id(connId);
    speed_notify.set_speed((float)speed);

    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    ::grpc::Status status = stub_->P2rSpeedLevelNotification(&context, speed_notify, &response);
    speed_notify.release_connection_id();
    return status;
  }

  ::grpc::Status SendP2RSessionTerminationWarning( long time, long warning_id, int reason) {
    TerminateWarning terminate;
    terminate.set_warning_id(warning_id);
    terminate.set_timeout(time);    
    terminate.set_reason((TerminateReason)reason);
    terminate.set_allocated_connection_id(connId);
    ::grpc::ClientContext context;
    Response response;
    std::cout << "client SendP2RSessionTerminationWarning\n";
    ::grpc::Status status = stub_->P2rTerminateWarning(&context, terminate, &response);
    std::cout << "client after SendP2RSessionTerminationWarning: " << status.error_message() << " \n";
    terminate.release_connection_id();
    if (status.ok())
    {
      std::cout << "client SendP2RSessionTerminationWarning status OK\n";
      if (response.cause() == p2r::Cause::SUCCESS)
      {
        std::cout << "client SendP2RSessionTerminationWarning cause OK\n";
        rm_id = response.connection_id().rm_id();
        return grpc::Status::OK;
      }
    }
    std::cout << "client SendP2RSessionTerminationWarning return\n";
    return status;
  }

  ::grpc::Status SendP2RSessionTerminationWarningCancel(long warning_id )
  {
    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    TerminateCancel cancel;
    cancel.set_allocated_connection_id(connId);
    cancel.set_warning_id(warning_id);
    std::cout << "client SendP2RSessionTerminationWarningCancel\n";
    ::grpc::Status status = stub_->P2rTerminateWarningCancel(&context, cancel, &response);
    cancel.release_connection_id();
    return status;
  }

  ::grpc::Status SendP2RSessionTerminationWarningCancel(long warning_id, const TerminateCancel &cancel, ::google::protobuf::Empty *response)
  {
    ::grpc::ClientContext context;
    std::cout << "client SendP2RSessionTerminationWarningCancel\n";
    ::grpc::Status status = stub_->P2rTerminateWarningCancel(&context, cancel, response);
    std::cout << "client after SendP2RSessionTerminationWarningCancel: " << status.error_message() << " \n";
    return status;
  }

  ::grpc::Status SendP2RSessionRestoreWarning(long time) {
    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    RestoreWarning restore;
    restore.set_timeout(time);
    restore.set_allocated_connection_id(connId);
    ::grpc::Status status = stub_->P2rRestoreWarning(&context, restore, &response);
    restore.release_connection_id();
    return status;
  }

private:
  std::unique_ptr<p2r::P2R::Stub> stub_;
  //int fp_id;
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
    std::string target_address{server};
    
    target_address += ":" + std::to_string(port);
    try
    {
      std::cout << "Connect to: " << target_address << "\n";
      std::shared_ptr<Channel> channel = grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials());
      std::cout << "Channel created\n";
      client = new P2rClient(channel, id);
      std::cout << "client\n";
    }
    catch (...)
    {
      std::cout << "exception\n";
      return ret_val::Error;
    }
    return ret_val::Success;
  }

  ret_val P2rClientRelease()
  {
    std::cout << "Release clien\n";
    if (client)
    {      
      if( client->connId ) {
        client->connId->release_protocol_version(); 
        delete client->connId;
      }
      if (client->protoVersion)
      {
        delete client->protoVersion;
      }
      std::cout << "Deleting client\n";
      delete client;
    }
    client = NULL;
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
    std::cout << "SendP2RSessionTerminationWarning\n";
    if (!client)
    {
      return ret_val::Error;
    }
    //::grpc::Status status = ((P2rClient *)context)->SendTermination(termination);
    ::grpc::Status status = client->SendP2RSessionTerminationWarning( time, warning_id, reason);
    std::cout << "api SendP2RSessionTerminationWarning: " << status.error_message() << " \n";
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

    /*ConnectionId connId;
    connId.set_fp_id(1);
    ConnectionId_ProtocolVersion protoVersion;
    protoVersion.set_major(1);
    protoVersion.set_minor(0);
    connId.set_allocated_protocol_version(&protoVersion);*/
    
    /*::google::protobuf::Empty response;
    TerminateCancel cancel;
    cancel.set_allocated_connection_id(client->connId);
    cancel.set_warning_id(warning_id);
    std::cout << "client SendP2RSessionTerminationWarningCancel\n";*/
    std::cout << "client SendP2RSessionTerminationWarningCancel\n";

    ::grpc::Status status = client->SendP2RSessionTerminationWarningCancel(warning_id);
    //::grpc::Status status = client->SendP2RSessionTerminationWarningCancel(warning_id, cancel, &response );
    std::cout << "api SendP2RSessionTerminationWarningCancel: " << status.error_message() << " \n";
    //connId.release_protocol_version();
    //cancel.release_connection_id();
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
