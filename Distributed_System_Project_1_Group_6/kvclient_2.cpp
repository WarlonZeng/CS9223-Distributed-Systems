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

  boost::shared_ptr<TTransport> socket(new TSocket(server, port));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  KVStoreClient client(protocol);


  // =============================
  // GET, SET, DEL FUNCTIONS
  // =============================

  std::string key;
  std::string value;
  std::string filename;

  try { transport->open(); }
  catch (TException& tx) {
    std::cerr << "ERROR: " << tx.what() << std::endl;
    exit(2);
  }

  while (true) {
    std::string str;
    std::getline(std::cin, str);

    std::string buf; // Have a buffer string
    std::stringstream ss(str); // Insert the string into a stream
    
    std::vector<std::string> tokens; // Create vector to hold our words

  char chars[] = "'";
    while (ss >> buf) {
      for (unsigned int i = 0; i < strlen(chars); ++i) {
        buf.erase(std::remove(buf.begin(), buf.end(), chars[i]), buf.end());
      }
        tokens.push_back(buf);
    }

    for (size_t n = 0; n < tokens.size(); n++)
      std::cout << n << ": \"" << tokens[n] << "\"\n";

    try {
      Result res;

      // Find and perform -set if available.
      if (find(tokens.begin(), tokens.end(), "-set") != tokens.end()) {
        key = tokens[1];
        value = tokens[2];

        std::cout << key << std::endl;
        std::cout << value << std::endl;

        client.kvset(res, key, value);
        std::cout << res << std::endl;
      }

      // Find and perform -get if available.
      if (find(tokens.begin(), tokens.end(), "-get") != tokens.end()) {
        key = tokens[1];
        filename = tokens[2];

        std::cout << key << std::endl;
        std::cout << filename << std::endl;

        client.kvget(res, key);
        std::cout << res << std::endl;
        if (res.error == 0) {
          std::ofstream outfile(filename.c_str());
          outfile << res.value << std::endl;
          outfile.close();
        }
      }

      // Find and perform -del if available.
      if (tokens[0].compare("-del") == 0) {
        key = tokens[1];

        std::cout << key << std::endl;

        client.kvdelete(res, key);
        std::cout << res << std::endl;
      }
    } 

    catch (TException& tx) {
      std::cerr << "ERROR: " << tx.what() << std::endl;
      exit(1);
    }
  }
}