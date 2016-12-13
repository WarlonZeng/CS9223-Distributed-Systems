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

#include <cstdlib>
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

int sequence_no = 0;
int num_of_threads = 5;
int num_of_runs = 500;

std::string set = "[-set]";
std::string get = "[-get]";
std::string key_1 = "[abc]";
std::string key_2 = "[xzy]";
std::string value_1 = "[123]";
std::string value_2 = "[789]";


struct events {
  int thread_id;
  int sequence_number;
  std::string operation;
  std::vector<std::string> parameters;
  Result response;
};


std::vector<std::vector<events>> bigTable(num_of_threads * 2);


void requests(std::string server, int port, int id) {

  int rand_num;
  Result res;

  std::vector<std::string> random_requests;
  std::vector<std::string> random_keys;
  std::vector<std::string> random_values;

  // std::thread::id this_thread_id = std::this_thread::get_id();


  // =============================
  // SET UP TRANSACTIONS
  // =============================



  for (int i = 0; i < num_of_runs; i++) {
    
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


  // =============================
  // PERFORM TRANSACTIONS
  // =============================

  std::vector<events> log;

  for (int j = 0; j < num_of_runs; j++) {
    if (random_requests[j] == set) {
      events data;

      data.thread_id = id;
      data.operation = random_requests[j];
      data.parameters.push_back(random_keys[j]);
      data.parameters.push_back(random_values[j]);

      data.sequence_number = sequence_no++;
      client.kvset(res, random_keys[j], random_values[j]);
      data.response = res;

      log.push_back(data);

    }
    else {
      events data;

      data.thread_id = id;
      data.operation = random_requests[j];
      data.parameters.push_back(random_keys[j]);

      data.sequence_number = sequence_no++;
      client.kvget(res, random_keys[j]);
      data.response = res;

      log.push_back(data);

    }
    //std::cout << sequence_no << "           " << id << std::endl;
  }
  // return log;
  bigTable[id] = log;
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

  std::thread threads[num_of_threads];
  for (int i = 0 ; i < num_of_threads; i++)
    threads[i] = std::thread(requests, server, port, i);


  for (auto& t : threads) // immediate join for simultaneous concurrent threads
    t.join();


  // for (int i = 0; i < bigTable.size(); i++)
  //   for (int j = 0; j < bigTable[i].size(); j++)
  //     if (bigTable[i][j].operation == set)
  //       std::cout << bigTable[i][j].sequence_number << " - " << bigTable[i][j].thread_id << " - " << bigTable[i][j].operation << " - " << bigTable[i][j].parameters[0] << " - " << bigTable[i][j].parameters[1] << " - " << bigTable[i][j].response << std::endl;
  //     else
  //       std::cout << bigTable[i][j].sequence_number << " - " << bigTable[i][j].thread_id << " - " << bigTable[i][j].operation << " - " << bigTable[i][j].parameters[0] << " - " << bigTable[i][j].response << std::endl;


  // =============================
  // SET UP BIGTABLE
  // =============================


  std::vector<events> sortedBigTable(num_of_threads * num_of_runs * 2);

  std::cout << bigTable.size() << std::endl;

  // while (index < (num_of_threads * num_of_runs)) 

  for (size_t i = 0; i < bigTable.size(); i++)
    for (size_t j = 0; j < bigTable[i].size(); j++) {
      sortedBigTable[bigTable[i][j].sequence_number] = bigTable[i][j];
    }

  //std::cout << sortedBigTable.size() << std::endl;

    int i = 0;
    while (i < (num_of_runs * num_of_threads - 15)) { // YOU MAY OCCUR SEGMENTATION FAULT; do not process unknown memory.
      if (sortedBigTable[i].operation == set)
        std::cout << sortedBigTable[i].sequence_number << " - " << sortedBigTable[i].thread_id << " - " << sortedBigTable[i].operation << " - " << sortedBigTable[i].parameters[0] << " - " << sortedBigTable[i].parameters[1] << " - " << sortedBigTable[i].response << std::endl;
      else
        std::cout << sortedBigTable[i].sequence_number << " - " << sortedBigTable[i].thread_id << " - " << sortedBigTable[i].operation << " - " << sortedBigTable[i].parameters[0] << " - " << sortedBigTable[i].response << std::endl;
      i++;
    }


  // =============================
  // TIME TO DO ACTUAL TESTS
  // =============================

  
  // std::string set = "[-set]";
  // std::string get = "[-get]";
  // std::string key_1 = "[abc]";
  // std::string key_2 = "[xzy]";
  // std::string value_1 = "[123]";
  // std::string value_2 = "[789]";


  int last_index;
  int bug_index;
  std::string last_operation;
  std::string last_updated_value;
  std::string last_parameter;


  for (size_t i = 0; i < sortedBigTable.size(); i++) {
  	// this is not a real consistency error, it is simply that the first few random requests were fetching a value that doesn't exist yet.
  	if ( (sortedBigTable[i].operation == get) && (sortedBigTable[i].response.error == ErrorCode::kKeyNotFound) ) {
  		last_index = sortedBigTable[i].sequence_number;
  		bug_index = sortedBigTable[i].sequence_number;
  		exit(2);
  	}
  }

  // check to see if clients can read updated values 
  for (size_t i = 0; i < sortedBigTable.size(); i++) {
    // solid set, xyz
    if ( (sortedBigTable[i].operation == set) && (sortedBigTable[i].parameters[0] == key_2) ) {  
      last_index = sortedBigTable[i].sequence_number;
      last_updated_value = sortedBigTable[i].parameters[1]; // 123 or 789
    }
    // someone fetched xyz and didnt get what i set
    if ( (sortedBigTable[i].operation == get) && (sortedBigTable[i].parameters[0] == key_2) && (sortedBigTable[i].response.value != last_updated_value) ) {
      bug_index = sortedBigTable[i].sequence_number;
      std::cout << last_index << " - " << bug_index << std::endl;
      exit(1);
    }
  }

  
  // check to see if clients are consistently reading updated values
  last_updated_value = value_1;
  for (size_t i = 0; i < sortedBigTable.size(); i++) {
    // if get, xyz, 123
    if ( (sortedBigTable[i].operation == get) && (sortedBigTable[i].parameters[0] == key_2) && (sortedBigTable[i].response.value == value_1) ) { 
      last_index = sortedBigTable[i].sequence_number;
    }
    // pin point xyz 123 -> 789. set triggers difference in gets.
    if ( (sortedBigTable[i].operation == set) && (sortedBigTable[i].parameters[0] == key_2) && (sortedBigTable[i].parameters[1] == value_2) ) {
      last_index = sortedBigTable[i].sequence_number;
      last_operation = set;
    }
    if ( (sortedBigTable[i].operation == get) && (sortedBigTable[i].parameters[0] == key_2) && (sortedBigTable[i].response.value == value_2) ) { 
      if (last_operation != set) {
        bug_index = sortedBigTable[i].sequence_number;
        std::cout << last_index << " - " << bug_index << std::endl;
        exit(1);
      }
    }
  }


  for (size_t i = 0; i < sortedBigTable.size(); i++) {
    if ( (sortedBigTable[i].operation == set) && (sortedBigTable[i].parameters[1] != sortedBigTable[i].response.value) ) {
      last_index = sortedBigTable[i].sequence_number;
      bug_index = sortedBigTable[i].sequence_number;
      std::cout << last_index << " - " << bug_index << std::endl;
      exit(1);
    }
  }

exit(0);
}