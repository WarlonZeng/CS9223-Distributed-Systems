/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "kvstore_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace kvstore {

int _kErrorCodeValues[] = {
  ErrorCode::kSuccess,
  ErrorCode::kKeyNotFound,
  ErrorCode::kError
};
const char* _kErrorCodeNames[] = {
  "kSuccess",
  "kKeyNotFound",
  "kError"
};
const std::map<int, const char*> _ErrorCode_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(3, _kErrorCodeValues, _kErrorCodeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));


Result::~Result() throw() {
}


void Result::__set_value(const std::string& val) {
  this->value = val;
}

void Result::__set_error(const ErrorCode::type val) {
  this->error = val;
}

void Result::__set_errortext(const std::string& val) {
  this->errortext = val;
}

uint32_t Result::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->value);
          this->__isset.value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast0;
          xfer += iprot->readI32(ecast0);
          this->error = (ErrorCode::type)ecast0;
          this->__isset.error = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->errortext);
          this->__isset.errortext = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Result::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Result");

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("error", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32((int32_t)this->error);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("errortext", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->errortext);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Result &a, Result &b) {
  using ::std::swap;
  swap(a.value, b.value);
  swap(a.error, b.error);
  swap(a.errortext, b.errortext);
  swap(a.__isset, b.__isset);
}

Result::Result(const Result& other1) {
  value = other1.value;
  error = other1.error;
  errortext = other1.errortext;
  __isset = other1.__isset;
}
Result& Result::operator=(const Result& other2) {
  value = other2.value;
  error = other2.error;
  errortext = other2.errortext;
  __isset = other2.__isset;
  return *this;
}
void Result::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Result(";
  out << "value=" << to_string(value);
  out << ", " << "error=" << to_string(error);
  out << ", " << "errortext=" << to_string(errortext);
  out << ")";
}

} // namespace
