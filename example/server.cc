#include <string>
#include "example.pb.h"
#include "example.grpc.pb.h"
#include <grpcpp/grpcpp.h>

using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::Server;
//virtual ::grpc::Status Call(::grpc::ServerContext* context, const ::Request* request, ::Response* response);

class ExampleServiceImpl final : public Example::Service {
  ::grpc::Status Call(::grpc::ServerContext* context, const Request* request,
                  Response* response) override {
    std::string prefix("Hello ");
    response->set_msg(prefix + request->msg());
    return ::grpc::Status::OK;
  }
};

int main(void){
  std::string server_address("0.0.0.0:50001");
  ExampleServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
  return 0;
}