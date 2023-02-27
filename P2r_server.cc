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

  public:
    void SetApiCallbacks(ApiCallTable callbacks)
    {
      if (callbacks.speed_notify_callback)
      {
        api_callbacks.speed_notify_callback = callbacks.speed_notify_callback;
      }
  }
  void SetId( int id ) {
      rm_id = id;
  }
  void FillHeader(struct_ConnectionId* s_id, ConnectionId id ) {
    s_id->ProtocolVersion.major = id.protocol_version().major();
    s_id->ProtocolVersion.minor = id.protocol_version().minor();
    s_id->fp_id = id.fp_id();
    s_id->rm_id = rm_id;
  }
      
  Status P2rTerminateWarning(ServerContext *context, const TerminateWarning *request, Response *response) override
  {
    struct_TerminateWarning terminate;
    FillHeader(&(terminate.connection_id), request->connection_id());
    if( !api_callbacks.terminate_callback(terminate) ) {
      response->set_cause(p2r::SUCCESS);
      ConnectionId id;
      ConnectionId_ProtocolVersion version;
      version.set_major(1);
      version.set_minor(0);
      id.set_allocated_protocol_version(&version);
      id.set_rm_id(rm_id);
      id.set_fp_id(request->connection_id().fp_id());
      response->set_allocated_connection_id(&id);
    }
    else {
      response->set_cause(p2r::ERROR);
    }
    return Status::OK;
  }
  Status P2rTerminateWarningCancel(ServerContext *context, const TerminateCancel *request, ::google::protobuf::Empty *response) override
  {
    struct_TerminateCancel cancel;
    FillHeader(&(cancel.connection_id), request->connection_id());
    api_callbacks.terminate_cancel_callback(cancel);
    return Status::OK;
  }
  Status P2rRestoreWarning(ServerContext *context, const RestoreWarning *request, ::google::protobuf::Empty *response) override
  {
    struct_RestoreWarning restore;
    FillHeader(&(restore.connection_id), request->connection_id());
    api_callbacks.restore_callback(restore);
    return Status::OK;
  }
  Status P2rSpeedLevelNotification(ServerContext *context, const SpeedNotification *request, ::google::protobuf::Empty *response) override
  {
    struct_SpeedNotification speed_notify;
    FillHeader(&(speed_notify.connection_id), request->connection_id());
    api_callbacks.speed_notify_callback(speed_notify);
    return Status::OK;
  }
};

extern "C" 
{
  int RunServer(char *addr, ApiCallTable *callbacks, int id)
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
    service.SetId(id);

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on port: " << server_address << std::endl;

    server->Wait();
    return 0;
  }
}
