#include <bits/stdc++.h>
#include <grpcpp/grpcpp.h>

#include <string>

#include "protobuf/classes/cpp/P2r.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using p2r::ConnectionId;
using p2r::ReleaseRequest;
using p2r::Response;
using p2r::RestoreWarning;
using p2r::SetupRequest;
using p2r::TerminateCancel;
using p2r::TerminateWarning;

class P2rServer final : public p2r::P2R::Service {

  Status P2rSetupConnection(ServerContext *context, const SetupRequest *request, Response *response) override  {
    p2r::ConnectionId id( request->connection_id() );
    id.set_rm_id(1);
    response->set_cause(p2r::SUCCESS);
    response->set_allocated_connection_id(&id);
    return Status::OK;
  }
  Status P2rReleaseConnection(ServerContext *context, const ReleaseRequest *request, Response *response) override  {
    response->set_cause(p2r::SUCCESS);
    return Status::OK;
  }
  Status P2rTerminateWarning(ServerContext *context, const TerminateWarning *request, Response *response) override  {
    response->set_cause(p2r::SUCCESS);
    return Status::OK;
  }
  Status P2rTerminateWarningCancel(ServerContext *context, const TerminateCancel *request, Response *response) override {
    response->set_cause(p2r::SUCCESS);
    return Status::OK;
  }
  Status P2rRestoreWarning(ServerContext *context, const RestoreWarning *request, Response *response) override  {
    response->set_cause(p2r::SUCCESS);
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  P2rServer service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on port: " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}