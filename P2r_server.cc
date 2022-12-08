#include <bits/stdc++.h>
#include <grpcpp/grpcpp.h>

#include <string>

#include "../protobuf/classes/cpp/P2r.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using p2r::CauseResponse;
using p2r::ReleaseRequest;
using p2r::SetupRequest;
using p2r::SetupResponse;
using p2r::TerminateWarning;
using p2r::Timeout;

class P2rServer final : public p2r::P2R::Service {
  Status SetupConnection(ServerContext *context, const SetupRequest *request, SetupResponse *response) override {
    return Status::OK;
  }
  Status ReleaseConnection(ServerContext *context, const ReleaseRequest *request, CauseResponse *response) override
  {
    return Status::OK;
  }
  Status TerminationWarning(ServerContext *context, const TerminateWarning *request, CauseResponse *response) override
  {
    return Status::OK;
  }
  Status TerminateWarningCancel(ServerContext *context, const TerminateWarning *request, CauseResponse *response) override
  {
    return Status::OK;
  }
  Status RestorationWarning(ServerContext *context, const Timeout *request, CauseResponse *response) override
  {
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  P2rServer service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which
  // communication with client takes place
  builder.RegisterService(&service);

  // Assembling the server
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on port: " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}