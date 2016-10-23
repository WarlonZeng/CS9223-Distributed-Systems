#include "KVStore.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <boost/program_options.hpp>

#include <typeinfo>

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <utility>
#include <functional>
#include <sstream>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using boost::shared_ptr;
using namespace boost::program_options;
namespace po = boost::program_options;

using namespace std;
using namespace kvstore;

int main(int argc, char **argv) {
  // Finding arguments.
  vector <string> args( argv + 1, argv + argc );
  cout << argv[ 0 ] << " has " << args.size() << " arguments.\n";

  for (size_t n = 0; n < args.size(); n++)
    cout << n << ": \"" << args[ n ] << "\"\n";

  // This argument is fixed. 
  size_t n = find( args.begin(), args.end(), "-server" ) - args.begin();
  
  string server = args[n + 1].substr(0, args[n + 1].find(":")); // initialize host
  string str_port = args[n + 1].substr(args[n + 1].find(":") + 1);

  stringstream convert(str_port);
  int port = 0; // initialize port
  convert >> port;

  // Confirming configuration.
  cout << server << endl;
  cout << port << endl;

  string key;
  string value;

  boost::shared_ptr<TTransport> socket(new TSocket(server, port));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  KVStoreClient client(protocol);

  try {
    transport->open();
    Result res;
    
    // Find and perform -set if available.
    size_t j = find( args.begin(), args.end(), "-set" ) - args.begin();
    if (find(args.begin(), args.end(), "-set") != args.end()) {
      key = args[j + 1].substr(0);
      value = args[j + 2].substr(0);

      client.kvset(res, key, value);
      cout << key << endl;
      cout << value << endl;
      cout << res << endl;
    }

    // Find and perform -get if available.
    // Do not mind the coded out block below.. 
    size_t l = find( args.begin(), args.end(), "-get" ) - args.begin();
    if (find(args.begin(), args.end(), "-get") != args.end()) {
      key = args[l + 1].substr(0);
      string filename = args[l + 2].substr(0);

      client.kvget(res, key);
      //ofstream outfile(args[l + 2].substr(0));
      //ofstream outfile;
      //outfile.open(args[l + 2].substr(0).c_str());
      //ofstream outfile(filename.c_str());
      cout << res << endl;
      if (res.error == 0) {
        //ofstream outfile("my_value_file");
        ofstream outfile(filename.c_str());
        outfile << res.value << endl;
        outfile.close();
      }
    }

    // Find and perform -del if available.
    size_t k = find( args.begin(), args.end(), "-del" ) - args.begin();
    if (find(args.begin(), args.end(), "-del") != args.end()) {
      key = args[k + 1].substr(0);
      cout << key << endl;

      client.kvdelete(res, key);
      cout << res << endl;
    }

    transport->close();
  } 
  catch (TException& tx) {
    cout << "ERROR: " << tx.what() << endl;
  }
  return 0;
}