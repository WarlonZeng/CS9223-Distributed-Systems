# Distributed Systems Key-Value Store
Term project for CS9223 Distributed Systems, taught by Kurt Rosenfeld.


## OS Version
Ubuntu 16.0.4 LTS


## Program Language
C++


## Specifical Configurations
None


## Libraries/Compilation Configurations
Thrift 0.9.3 located in /var/lib/thrift-0.9.3
GCC std=C++11
Boost library
-pthread


---


## Compile Instructions
Where the MakeFile is, open terminal and run.
```git
make
```
Apache Thrift KVStore is compiled with Thrift, however, 
```git
thrift --gen cpp kvstore
```


----


# Distributed Systems Part 2
Tested for consistency. 

## Run server:
By default, host = localhost and port = 9090.
```git.
./kvserver
```

## Run consistency test:
Runs about 2500 requests across 5 threads.
```git
./kvserver
./consistency_test -server localhost:9090
```

## Additional features:
kvclient_1 and kvclient_2 are user while-true looped for testing Apache Thrift's concurrency.
```git
./kvserver
./kvclient_1 -server localhost:9090
-set 'hi' 'bye'
-get 'hi' 'hi'
````


---


# Distributed Systems Part 1 

## Run server
```git
./kvserver
```

## Client commands via command line
set(string key, string value): 
```git
./kvclient -server host:port -set 'hi' 'bye'
./kvclient -server host:port -get 'hi' 'hi'
./kvclient -server host:port -del 'my_key'
```


----


## If running doesn't work (after compliation)
May need to run this for every new terminal (one for server, one for client): 
```git
export LD_LIBRARY_PATH=/usr/local/lib/:${LD_LIBRARY_PATH}
```
