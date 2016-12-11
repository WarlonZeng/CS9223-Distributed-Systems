#include "KVStore.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

//#include <boost/program_options.hpp>

#include <typeinfo>

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <utility>
#include <functional>

#include <stdio.h>       // perror, snprintf
#include <stdlib.h>      // exit
#include <unistd.h>      // close, write
#include <string.h>      // strlen
#include <strings.h>     // bzero
#include <time.h>        // time, ctime
#include <sys/socket.h>  // socket, AF_INET, SOCK_STREAM, bind, listen, accept
#include <netinet/in.h>  // servaddr, INADDR_ANY, htons

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

// using namespace boost::program_options;
// namespace po = boost::program_options;

// using namespace std;
using namespace kvstore;

int main(int argc, char **argv) {


  // =============================
  // COMMAND LINE PARSING 
  // =============================


  // Finding arguments.
  std::vector<std::string> args( argv + 1, argv + argc );
  std::cout << argv[ 0 ] << " has " << args.size() << " arguments.\n";

  for (size_t n = 0; n < args.size(); n++)
    std::cout << n << ": \"" << args[ n ] << "\"\n";

  // This argument is fixed. 
  size_t n = find( args.begin(), args.end(), "-server" ) - args.begin();
  
  std::string server = args[n + 1].substr(0, args[n + 1].find(":")); // initialize host
  std::string str_port = args[n + 1].substr(args[n + 1].find(":") + 1);

  std::stringstream convert(str_port);
  int port = 0; // initialize port
  convert >> port;

  // Confirming configuration.
  std::cout << server << std::endl;
  std::cout << port << std::endl;

  std::string key;
  std::string value;

  boost::shared_ptr<TTransport> socket(new TSocket(server, port));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  KVStoreClient client(protocol);


  // =============================
  // GET, SET, DEL FUNCTIONS
  // =============================


  try {
    transport->open();
    Result res;
    
    // Find and perform -set if available.
    size_t j = find( args.begin(), args.end(), "-set" ) - args.begin();
    if (find(args.begin(), args.end(), "-set") != args.end()) {
      key = args[j + 1].substr(0);
      value = args[j + 2].substr(0);

      client.kvset(res, key, value);
      std::cout << key << std::endl;
      std::cout << value << std::endl;
      std::cout << res << std::endl;
    }

    // Find and perform -get if available.
    // Do not mind the coded out block below.. 
    size_t l = find( args.begin(), args.end(), "-get" ) - args.begin();
    if (find(args.begin(), args.end(), "-get") != args.end()) {
      key = args[l + 1].substr(0);
      std::string filename = args[l + 2].substr(0);

      client.kvget(res, key);
      std::cout << res << std::endl;
      if (res.error == 0) {
        std::ofstream outfile(filename.c_str());
        outfile << res.value << std::endl;
        outfile.close();
      }
    }

    // Find and perform -del if available.
    size_t k = find( args.begin(), args.end(), "-del" ) - args.begin();
    if (find(args.begin(), args.end(), "-del") != args.end()) {
      key = args[k + 1].substr(0);
      std::cout << key << std::endl;

      client.kvdelete(res, key);
      std::cout << res << std::endl;
    }

    transport->close();
  } 
  catch (TException& tx) {
    std::cerr << "ERROR: " << tx.what() << std::endl;
    exit(1);
  }
  exit(0);
}