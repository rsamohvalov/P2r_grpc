#include <bits/stdc++.h>
#include <grpcpp/grpcpp.h>

#include <string>

#include "protobuf/classes/P2r.grpc.pb.h"

#include "api/P2r_server_api.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using p2r::ConnectionId;
using p2r::ConnectionId_ProtocolVersion;
using p2r::Response;
using p2r::RestoreWarning;
using p2r::SpeedNotification;
using p2r::TerminateCancel;
using p2r::TerminateWarning;

class P2rServer final : public p2r::P2R::Service
{

  private:
    ApiCallTable api_callbacks;
    int rm_id;
    int proto_major = 1;
    int proto_minor = 0;

  public:
    void SetApiCallbacks(ApiCallTable *callbacks)
    {
      if (callbacks->speed_notify_callback)
      {
        api_callbacks.speed_notify_callback = callbacks->speed_notify_callback;
      }
      if (callbacks->terminate_callback)
      {
        api_callbacks.terminate_callback = callbacks->terminate_callback;
      }
      if (callbacks->terminate_cancel_callback)
      {
        api_callbacks.terminate_cancel_callback = callbacks->terminate_cancel_callback;
      }
      if (callbacks->restore_callback)
      {
        api_callbacks.restore_callback = callbacks->restore_callback;
      }
  }
  void SetId( int id ) {
      rm_id = id;
  }
        
  Status P2rTerminateWarning(ServerContext *context, const TerminateWarning *request, Response *response) override
  {
    struct_TerminateWarning terminate;
    terminate.fp_id = request->connection_id().fp_id();
    terminate.warning_id = request->warning_id();
    terminate.timeout = request->timeout();
    // std::cout << "server P2rTerminateWarning enter\n";

    if( !api_callbacks.terminate_callback(terminate) ) {
      response->set_cause(p2r::SUCCESS);
      ConnectionId* id = new ConnectionId();
      ConnectionId_ProtocolVersion* version = new ConnectionId_ProtocolVersion();
      version->set_major(this->proto_major);
      version->set_minor(this->proto_minor);
      id->set_allocated_protocol_version(version);
      id->set_rm_id(this->rm_id);
      id->set_fp_id(request->connection_id().fp_id());
      response->set_allocated_connection_id(id);
      //std::cout << "Server P2rTerminateWarning response\n";
    }
    else {
      response->set_cause(p2r::ERROR);
    }
    //std::cout << "Server P2rTerminateWarning exit\n";

    return Status::OK;
  }
  Status P2rTerminateWarningCancel(ServerContext *context, const TerminateCancel *request, ::google::protobuf::Empty *response) override
  {
    struct_TerminateCancel cancel;
    //std::cout << "Server P2rTerminateWarningCancel\n";

    cancel.fp_id = request->connection_id().fp_id();
    cancel.warning_id = request->warning_id();
    api_callbacks.terminate_cancel_callback(cancel);
    return Status::OK;
  }
  Status P2rRestoreWarning(ServerContext *context, const RestoreWarning *request, ::google::protobuf::Empty *response) override
  {
    struct_RestoreWarning restore;
    //std::cout << "Server P2rRestoreWarning\n";

    restore.fp_id = request->connection_id().fp_id();
    restore.timeout = request->timeout();
    api_callbacks.restore_callback(restore);
    return Status::OK;
  }
  Status P2rSpeedLevelNotification(ServerContext *context, const SpeedNotification *request, ::google::protobuf::Empty *response) override
  {
    struct_SpeedNotification speed_notify;
    //std::cout << "Server P2rSpeedLevelNotification\n";

    speed_notify.fp_id = request->connection_id().fp_id();
    speed_notify.speed = (float)request->speed();
    api_callbacks.speed_notify_callback(speed_notify);
    return Status::OK;
  }
};

extern "C" 
{
  int InitServer(char *addr, unsigned short port, int id, ApiCallTable *callbacks)
  {
    if (!addr)
    {
      return -1;
    }
    if (!callbacks || !callbacks->speed_notify_callback || !callbacks->terminate_callback || !callbacks->terminate_cancel_callback || !callbacks->restore_callback)
    {
      return -2;
    }
    std::string server_address(addr);
    server_address += ":" + std::to_string(port);
    
    P2rServer service;
    service.SetApiCallbacks(callbacks);
    service.SetId(id);

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on: " << server_address << std::endl;

    server->Wait();
    return 0;
  }
}
