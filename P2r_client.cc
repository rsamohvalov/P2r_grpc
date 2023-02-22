#include <grpcpp/grpcpp.h>

#include <string>

#include "protobuf/classes/P2r.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using p2r::ConnectionId;
using p2r::ReleaseRequest;
using p2r::Response;
using p2r::RestoreWarning;
using p2r::SetupRequest;
using p2r::TerminateCancel;
using p2r::TerminateWarning;

class P2rClient {
  public:
    P2rClient(std::shared_ptr<Channel> channel)
        : stub_(p2r::P2R::NewStub(channel)) {}

  private:
    std::unique_ptr<p2r::P2R::Stub> stub_;
};

void RunClient() {
  std::string target_address("0.0.0.0:50051");
  P2rClient client(
      grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));
}

int main(int argc, char* argv[]) {
  RunClient();
  return 0;
}