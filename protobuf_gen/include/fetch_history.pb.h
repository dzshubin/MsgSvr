// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: fetch_history.proto

#ifndef PROTOBUF_fetch_5fhistory_2eproto__INCLUDED
#define PROTOBUF_fetch_5fhistory_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace IM {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_fetch_5fhistory_2eproto();
void protobuf_AssignDesc_fetch_5fhistory_2eproto();
void protobuf_ShutdownFile_fetch_5fhistory_2eproto();

class FetchHistoryReq;

// ===================================================================

class FetchHistoryReq : public ::google::protobuf::Message {
 public:
  FetchHistoryReq();
  virtual ~FetchHistoryReq();

  FetchHistoryReq(const FetchHistoryReq& from);

  inline FetchHistoryReq& operator=(const FetchHistoryReq& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const FetchHistoryReq& default_instance();

  void Swap(FetchHistoryReq* other);

  // implements Message ----------------------------------------------

  inline FetchHistoryReq* New() const { return New(NULL); }

  FetchHistoryReq* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FetchHistoryReq& from);
  void MergeFrom(const FetchHistoryReq& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(FetchHistoryReq* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int64 req_id = 1;
  void clear_req_id();
  static const int kReqIdFieldNumber = 1;
  ::google::protobuf::int64 req_id() const;
  void set_req_id(::google::protobuf::int64 value);

  // optional int64 target_id = 2;
  void clear_target_id();
  static const int kTargetIdFieldNumber = 2;
  ::google::protobuf::int64 target_id() const;
  void set_target_id(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:IM.FetchHistoryReq)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::int64 req_id_;
  ::google::protobuf::int64 target_id_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_fetch_5fhistory_2eproto();
  friend void protobuf_AssignDesc_fetch_5fhistory_2eproto();
  friend void protobuf_ShutdownFile_fetch_5fhistory_2eproto();

  void InitAsDefaultInstance();
  static FetchHistoryReq* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// FetchHistoryReq

// optional int64 req_id = 1;
inline void FetchHistoryReq::clear_req_id() {
  req_id_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 FetchHistoryReq::req_id() const {
  // @@protoc_insertion_point(field_get:IM.FetchHistoryReq.req_id)
  return req_id_;
}
inline void FetchHistoryReq::set_req_id(::google::protobuf::int64 value) {
  
  req_id_ = value;
  // @@protoc_insertion_point(field_set:IM.FetchHistoryReq.req_id)
}

// optional int64 target_id = 2;
inline void FetchHistoryReq::clear_target_id() {
  target_id_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 FetchHistoryReq::target_id() const {
  // @@protoc_insertion_point(field_get:IM.FetchHistoryReq.target_id)
  return target_id_;
}
inline void FetchHistoryReq::set_target_id(::google::protobuf::int64 value) {
  
  target_id_ = value;
  // @@protoc_insertion_point(field_set:IM.FetchHistoryReq.target_id)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace IM

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_fetch_5fhistory_2eproto__INCLUDED
