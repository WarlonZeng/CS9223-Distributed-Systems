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
#include <time.h>

#include <stdio.h>       // perror, snprintf
#include <stdlib.h>      // exit
#include <unistd.h>      // close, write
#include <string.h>      // strlen
#include <strings.h>     // bzero
#include <time.h>        // time, ctime
#include <sys/socket.h>  // socket, AF_INET, SOCK_STREAM, bind, listen, accept
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <netinet/in.h>  // servaddr, INADDR_ANY, htons

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

// using namespace boost::program_options;
// namespace po = boost::program_options;

// using namespace std;
using namespace kvstore;



int sequence_num = 0;

std::string get = "-set";
std::string set = "-get";
std::string key_1 = "kurt";
std::string key_2 = "sterling";
std::string value_1 = "google man";
std::string value_2 = "old man";


std::mutex mtx;
std::condition_variable cv;
bool ready = false;
int num_threads_b = 0;


struct event {
  int seq_num;
  Result response;
};


void go() {
  std::unique_lock<std::mutex> lck(mtx);
  ready = true;
  cv.notify_all();
}


void requests(std::string server, int port) {

  int rand_num;
  Result res;

  std::vector<std::string> random_requests;
  std::vector<std::string> random_keys;
  std::vector<std::string> random_values;

  std::thread::id this_thread_id = std::this_thread::get_id();


  // =============================
  // SET UP TRANSACTIONS
  // =============================



  for (int i = 0; i < 50; i++) {
    
    rand_num = rand() % 2;
    if (rand_num == 0)
      random_requests.push_back(set);
    else
      random_requests.push_back(get);

    rand_num = rand() % 2;
    if (rand_num == 0)
      random_keys.push_back(key_1);
    else
      random_keys.push_back(key_2);

    rand_num = rand() % 2;
    if (rand_num == 0)
      random_values.push_back(value_1);
    else
      random_values.push_back(value_2);

  }


  // =============================
  // CONNECT TO SERVER
  // =============================


  boost::shared_ptr<TTransport> socket(new TSocket(server, port));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  KVStoreClient client(protocol);

  try { transport->open(); }
  catch (TException& tx) {
    std::cerr << "ERROR: " << tx.what() << std::endl;
    exit(2);
  }


  std::unique_lock<std::mutex> lck(mtx);
  while (!ready) 
    cv.wait(lck);


  // =============================
  // PERFORM TRANSACTIONS
  // =============================


  std::vector<event> log;

  for (int j = 0; j < 50; j++) {
    if (random_requests[j] == set) {
      event data;

      data.seq_num = sequence_num++;
      client.kvset(res, random_keys[j], random_values[j]);
      data.response = res;
      //std::cout << "kvset(" + random_keys[j] + ", " + random_values[j] + ")" << ": " << res << std::endl;
      //std::cout << type_id(res) << std::endl;

      log.push_back(data);

    }
    else {
      event data;

      data.seq_num = sequence_num++;
      client.kvget(res, random_keys[j]);
      data.response = res;
      //std::cout << "------kvget(" + random_keys[j] + ")" << ": " << res << std::endl;
      //std::cout << type_id(res) << std::endl;

      log.push_back(data);

    }
  }

}


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


  // =============================
  // SET UP THREADS
  // =============================

  srand(time(NULL));
  // std::vector<std::thread> threads;


  std::thread threads[5];
  for (int i = 0 ; i < 5; i++)
    threads[i] = std::thread(requests, server, port);

  sleep(2);
  go();

  for (auto& t : threads) 
    t.join();

  std::cout << sequence_num << std::endl;


}