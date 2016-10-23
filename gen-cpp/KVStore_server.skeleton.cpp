// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "KVStore.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::kvstore;

class KVStoreHandler : virtual public KVStoreIf {
 public:
  KVStoreHandler() {
    // Your initialization goes here
  }

  void kvset(Result& _return, const std::string& key, const std::string& value) {
    // Your implementation goes here
    printf("kvset\n");
  }

  void kvget(Result& _return, const std::string& key) {
    // Your implementation goes here
    printf("kvget\n");
  }

  void kvdelete(Result& _return, const std::string& key) {
    // Your implementation goes here
    printf("kvdelete\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<KVStoreHandler> handler(new KVStoreHandler());
  shared_ptr<TProcessor> processor(new KVStoreProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

