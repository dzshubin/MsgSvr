// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: offline_message.proto

#ifndef PROTOBUF_offline_5fmessage_2eproto__INCLUDED
#define PROTOBUF_offline_5fmessage_2eproto__INCLUDED

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
void protobuf_AddDesc_offline_5fmessage_2eproto();
void protobuf_AssignDesc_offline_5fmessage_2eproto();
void protobuf_ShutdownFile_offline_5fmessage_2eproto();

class OfflineCach;
class OfflineMessage;

// ===================================================================

class OfflineCach : public ::google::protobuf::Message {
 public:
  OfflineCach();
  virtual ~OfflineCach();

  OfflineCach(const OfflineCach& from);

  inline OfflineCach& operator=(const OfflineCach& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const OfflineCach& default_instance();

  void Swap(OfflineCach* other);

  // implements Message ----------------------------------------------

  inline OfflineCach* New() const { return New(NULL); }

  OfflineCach* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const OfflineCach& from);
  void MergeFrom(const OfflineCach& from);
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
  void InternalSwap(OfflineCach* other);
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

  // optional int64 user_id = 1;
  void clear_user_id();
  static const int kUserIdFieldNumber = 1;
  ::google::protobuf::int64 user_id() const;
  void set_user_id(::google::protobuf::int64 value);

  // repeated .IM.OfflineMessage messages = 2;
  int messages_size() const;
  void clear_messages();
  static const int kMessagesFieldNumber = 2;
  const ::IM::OfflineMessage& messages(int index) const;
  ::IM::OfflineMessage* mutable_messages(int index);
  ::IM::OfflineMessage* add_messages();
  ::google::protobuf::RepeatedPtrField< ::IM::OfflineMessage >*
      mutable_messages();
  const ::google::protobuf::RepeatedPtrField< ::IM::OfflineMessage >&
      messages() const;

  // @@protoc_insertion_point(class_scope:IM.OfflineCach)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::int64 user_id_;
  ::google::protobuf::RepeatedPtrField< ::IM::OfflineMessage > messages_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_offline_5fmessage_2eproto();
  friend void protobuf_AssignDesc_offline_5fmessage_2eproto();
  friend void protobuf_ShutdownFile_offline_5fmessage_2eproto();

  void InitAsDefaultInstance();
  static OfflineCach* default_instance_;
};
// -------------------------------------------------------------------

class OfflineMessage : public ::google::protobuf::Message {
 public:
  OfflineMessage();
  virtual ~OfflineMessage();

  OfflineMessage(const OfflineMessage& from);

  inline OfflineMessage& operator=(const OfflineMessage& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const OfflineMessage& default_instance();

  void Swap(OfflineMessage* other);

  // implements Message ----------------------------------------------

  inline OfflineMessage* New() const { return New(NULL); }

  OfflineMessage* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const OfflineMessage& from);
  void MergeFrom(const OfflineMessage& from);
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
  void InternalSwap(OfflineMessage* other);
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

  // optional int64 send_id = 1;
  void clear_send_id();
  static const int kSendIdFieldNumber = 1;
  ::google::protobuf::int64 send_id() const;
  void set_send_id(::google::protobuf::int64 value);

  // optional string content = 2;
  void clear_content();
  static const int kContentFieldNumber = 2;
  const ::std::string& content() const;
  void set_content(const ::std::string& value);
  void set_content(const char* value);
  void set_content(const char* value, size_t size);
  ::std::string* mutable_content();
  ::std::string* release_content();
  void set_allocated_content(::std::string* content);

  // optional string send_time = 3;
  void clear_send_time();
  static const int kSendTimeFieldNumber = 3;
  const ::std::string& send_time() const;
  void set_send_time(const ::std::string& value);
  void set_send_time(const char* value);
  void set_send_time(const char* value, size_t size);
  ::std::string* mutable_send_time();
  ::std::string* release_send_time();
  void set_allocated_send_time(::std::string* send_time);

  // @@protoc_insertion_point(class_scope:IM.OfflineMessage)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::int64 send_id_;
  ::google::protobuf::internal::ArenaStringPtr content_;
  ::google::protobuf::internal::ArenaStringPtr send_time_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_offline_5fmessage_2eproto();
  friend void protobuf_AssignDesc_offline_5fmessage_2eproto();
  friend void protobuf_ShutdownFile_offline_5fmessage_2eproto();

  void InitAsDefaultInstance();
  static OfflineMessage* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// OfflineCach

// optional int64 user_id = 1;
inline void OfflineCach::clear_user_id() {
  user_id_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 OfflineCach::user_id() const {
  // @@protoc_insertion_point(field_get:IM.OfflineCach.user_id)
  return user_id_;
}
inline void OfflineCach::set_user_id(::google::protobuf::int64 value) {
  
  user_id_ = value;
  // @@protoc_insertion_point(field_set:IM.OfflineCach.user_id)
}

// repeated .IM.OfflineMessage messages = 2;
inline int OfflineCach::messages_size() const {
  return messages_.size();
}
inline void OfflineCach::clear_messages() {
  messages_.Clear();
}
inline const ::IM::OfflineMessage& OfflineCach::messages(int index) const {
  // @@protoc_insertion_point(field_get:IM.OfflineCach.messages)
  return messages_.Get(index);
}
inline ::IM::OfflineMessage* OfflineCach::mutable_messages(int index) {
  // @@protoc_insertion_point(field_mutable:IM.OfflineCach.messages)
  return messages_.Mutable(index);
}
inline ::IM::OfflineMessage* OfflineCach::add_messages() {
  // @@protoc_insertion_point(field_add:IM.OfflineCach.messages)
  return messages_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::IM::OfflineMessage >*
OfflineCach::mutable_messages() {
  // @@protoc_insertion_point(field_mutable_list:IM.OfflineCach.messages)
  return &messages_;
}
inline const ::google::protobuf::RepeatedPtrField< ::IM::OfflineMessage >&
OfflineCach::messages() const {
  // @@protoc_insertion_point(field_list:IM.OfflineCach.messages)
  return messages_;
}

// -------------------------------------------------------------------

// OfflineMessage

// optional int64 send_id = 1;
inline void OfflineMessage::clear_send_id() {
  send_id_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 OfflineMessage::send_id() const {
  // @@protoc_insertion_point(field_get:IM.OfflineMessage.send_id)
  return send_id_;
}
inline void OfflineMessage::set_send_id(::google::protobuf::int64 value) {
  
  send_id_ = value;
  // @@protoc_insertion_point(field_set:IM.OfflineMessage.send_id)
}

// optional string content = 2;
inline void OfflineMessage::clear_content() {
  content_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& OfflineMessage::content() const {
  // @@protoc_insertion_point(field_get:IM.OfflineMessage.content)
  return content_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void OfflineMessage::set_content(const ::std::string& value) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:IM.OfflineMessage.content)
}
inline void OfflineMessage::set_content(const char* value) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:IM.OfflineMessage.content)
}
inline void OfflineMessage::set_content(const char* value, size_t size) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:IM.OfflineMessage.content)
}
inline ::std::string* OfflineMessage::mutable_content() {
  
  // @@protoc_insertion_point(field_mutable:IM.OfflineMessage.content)
  return content_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* OfflineMessage::release_content() {
  
  return content_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void OfflineMessage::set_allocated_content(::std::string* content) {
  if (content != NULL) {
    
  } else {
    
  }
  content_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), content);
  // @@protoc_insertion_point(field_set_allocated:IM.OfflineMessage.content)
}

// optional string send_time = 3;
inline void OfflineMessage::clear_send_time() {
  send_time_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& OfflineMessage::send_time() const {
  // @@protoc_insertion_point(field_get:IM.OfflineMessage.send_time)
  return send_time_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void OfflineMessage::set_send_time(const ::std::string& value) {
  
  send_time_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:IM.OfflineMessage.send_time)
}
inline void OfflineMessage::set_send_time(const char* value) {
  
  send_time_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:IM.OfflineMessage.send_time)
}
inline void OfflineMessage::set_send_time(const char* value, size_t size) {
  
  send_time_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:IM.OfflineMessage.send_time)
}
inline ::std::string* OfflineMessage::mutable_send_time() {
  
  // @@protoc_insertion_point(field_mutable:IM.OfflineMessage.send_time)
  return send_time_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* OfflineMessage::release_send_time() {
  
  return send_time_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void OfflineMessage::set_allocated_send_time(::std::string* send_time) {
  if (send_time != NULL) {
    
  } else {
    
  }
  send_time_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), send_time);
  // @@protoc_insertion_point(field_set_allocated:IM.OfflineMessage.send_time)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace IM

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_offline_5fmessage_2eproto__INCLUDED
