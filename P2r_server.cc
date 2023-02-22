#include <bits/stdc++.h>
#include <grpcpp/grpcpp.h>

#include <string>

#include "protobuf/classes/P2r.grpc.pb.h"

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
  Status P2rSpeedLevelNotification(ServerContext *context, ::grpc::ServerReader<SpeedNotification> *reader, Response *response) override
  {
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