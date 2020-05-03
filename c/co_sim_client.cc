/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <ctime>
#include <iostream>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "co_sim.grpc.pb.h"

using grpc::Channel;
using grpc::ChannelArguments;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using cosim::CoSim;
using cosim::SimRequest;
using cosim::SimResponse;

class CoSimClient {
 public:
  CoSimClient(std::shared_ptr<Channel> channel)
      : stub_(CoSim::NewStub(channel)) {
  }

  void RMSim(const std::string& type, int ch_size, int buf_size) {
    cosim::SimRequest req;
    cosim::SimResponse resp;
    ClientContext context;

    req.set_type(type);
    // populates channels & buffer
    populate(req, ch_size, buf_size);
    std::cout << time_now() << "Run rm for (type="
              << type
              << ",ch_size="
              << ch_size
              << ",buf_size="
              << buf_size
              << ")"
              << std::endl;

    Status status = stub_->RMVal(&context, req, &resp);
    if (status.ok()) {
	    std::cout << "\tstatus is ok" << std::endl;
    } else {
	    std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }
    std::cout << time_now() << "\tRM response validity: "
              << resp.valid() << " "
	      << resp.buffer_size() << " "
	      << resp.buffer(resp.buffer_size()-1)
	      << "\n"
              << std::endl;
  }


 private:

  void populate(cosim::SimRequest& req, int ch_size, int buf_size) {
    std::string ch;
    for (int i=0; i<ch_size; i++) {
      ch = "ch-" + std::to_string(i);
      req.add_channels(ch);
    }
    for (int i=0; i<buf_size; i++) {
      req.add_buffer(i);
    }
  }

  char* time_now() {
    time_t now = time(0);
    return ctime(&now);
  }

  std::unique_ptr<CoSim::Stub> stub_;
};

int main(int argc, char** argv) {
  ChannelArguments args;
  args.SetMaxReceiveMessageSize(500*1024*1024);
  args.SetMaxSendMessageSize(500*1024*1024);
  CoSimClient client(
      grpc::CreateCustomChannel("localhost:50051",
                          grpc::InsecureChannelCredentials(), args)
      );

  std::cout << "-------------- RunSim --------------" << std::endl;
  client.RMSim("type1", 2, 100);
  client.RMSim("typex", 1, 100*1024); // 100KB buffer
  client.RMSim("typex", 1, 1024*1024); // 4MB buffer
  client.RMSim("type2", 3, 1024*1024*50); // 200MB buffer

  return 0;
}
