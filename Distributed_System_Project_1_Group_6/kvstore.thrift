/* 
	Distributed Systems Part 1
 */

namespace cpp kvstore
namespace java kvstore

enum ErrorCode {
    kSuccess = 0,
    kKeyNotFound = 1,
    kError = 2,
}

struct Result {
  // If an RPC returns a value, it is here.
  1: string value,

  2: ErrorCode error,

  // If an RPC returns kError, errortext may contain human-readable text.
  3: string errortext,
}

service KVStore {
   // If a key-value pair already exists, overwrite its value.
   // If a key-value pair does not already exist, create it.
   Result kvset(1:string key, 2:string value),

   // If a key-value pair exists, return its value and kSuccess.
   // If a key-value pair does not exist, return error kKeyNotFound.
   Result kvget(1:string key),


   // If a key-value pair exists, delete it and return kSuccess.
   // If a key-value pair does not exist, return kKeyNotFound.
   Result kvdelete(1:string key),
}
