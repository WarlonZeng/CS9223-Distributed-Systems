# Distributed Systems Key-Value Store
Term project for CS9223 Distributed Systems, taught by Kurt Rosenfeld.

## OS Version
Ubuntu 16.0.4 LTS

## Program Language
C++

## Specifical Configurations
Unix ">" does not work. Simply ignore when performing the GET request.

## Libraries/Compilation Configurations
Thrift 0.9.3
GCC std=C++11
Boost library


---


## Compile Instructions
Where the MakeFile is, open terminal and run
```git
make
```


---


## Run Instructions
Server and client commands:

### Run server:
* By default, host = localhost and port = 9090
```git
./kvserver
```


---


### Run client commands
Parameter definitions: 

server(string host, int post): 
connects to host and port specification. Defaults localhost, 9090. 
```git
./kvclient -server host:port
```

set(string key, string value): 
creates "key" and maps "value" to key. Key-Value map is stored in server memory.
```git
./kvclient -server host:port -set 'my_key' 'my_value_file'
```

get(string key, string filename): 
gets value for key, if key exists in server memory. Outputs to file specified by flename containing the value for associated key.
```git
./kvclient -server host:port -get 'my_key' my_key_containg_value
```

del(string key): 
deletes key from server memory.
```git
./kvclient -server host:port -del 'my_key'
```


---


## Sample client commands
Simple test case: 
```git
./kvclient -server localhost:9090 -set 'hi' 'bye'

./kvclient -server localhost:9090 -get 'hi' 'hi'

./kvclient -server host:port -del 'hi'
```

---


## If running doesn't work (after compliation)
May need to run this for every new terminal (one for server, one for client): 
```git
export LD_LIBRARY_PATH=/usr/local/lib/:${LD_LIBRARY_PATH}
```
