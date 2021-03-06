// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: operate_req_base.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "operate_req_base.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace IM {

namespace {

const ::google::protobuf::Descriptor* OperateReqBase_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  OperateReqBase_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_operate_5freq_5fbase_2eproto() {
  protobuf_AddDesc_operate_5freq_5fbase_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "operate_req_base.proto");
  GOOGLE_CHECK(file != NULL);
  OperateReqBase_descriptor_ = file->message_type(0);
  static const int OperateReqBase_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OperateReqBase, user_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OperateReqBase, channel_id_),
  };
  OperateReqBase_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      OperateReqBase_descriptor_,
      OperateReqBase::default_instance_,
      OperateReqBase_offsets_,
      -1,
      -1,
      -1,
      sizeof(OperateReqBase),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OperateReqBase, _internal_metadata_),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OperateReqBase, _is_default_instance_));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_operate_5freq_5fbase_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      OperateReqBase_descriptor_, &OperateReqBase::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_operate_5freq_5fbase_2eproto() {
  delete OperateReqBase::default_instance_;
  delete OperateReqBase_reflection_;
}

void protobuf_AddDesc_operate_5freq_5fbase_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\026operate_req_base.proto\022\002IM\"5\n\016OperateR"
    "eqBase\022\017\n\007user_id\030\001 \001(\003\022\022\n\nchannel_id\030\002 "
    "\001(\005b\006proto3", 91);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "operate_req_base.proto", &protobuf_RegisterTypes);
  OperateReqBase::default_instance_ = new OperateReqBase();
  OperateReqBase::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_operate_5freq_5fbase_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_operate_5freq_5fbase_2eproto {
  StaticDescriptorInitializer_operate_5freq_5fbase_2eproto() {
    protobuf_AddDesc_operate_5freq_5fbase_2eproto();
  }
} static_descriptor_initializer_operate_5freq_5fbase_2eproto_;

namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD;
static void MergeFromFail(int line) {
  GOOGLE_CHECK(false) << __FILE__ << ":" << line;
}

}  // namespace


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int OperateReqBase::kUserIdFieldNumber;
const int OperateReqBase::kChannelIdFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

OperateReqBase::OperateReqBase()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:IM.OperateReqBase)
}

void OperateReqBase::InitAsDefaultInstance() {
  _is_default_instance_ = true;
}

OperateReqBase::OperateReqBase(const OperateReqBase& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:IM.OperateReqBase)
}

void OperateReqBase::SharedCtor() {
    _is_default_instance_ = false;
  _cached_size_ = 0;
  user_id_ = GOOGLE_LONGLONG(0);
  channel_id_ = 0;
}

OperateReqBase::~OperateReqBase() {
  // @@protoc_insertion_point(destructor:IM.OperateReqBase)
  SharedDtor();
}

void OperateReqBase::SharedDtor() {
  if (this != default_instance_) {
  }
}

void OperateReqBase::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* OperateReqBase::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return OperateReqBase_descriptor_;
}

const OperateReqBase& OperateReqBase::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_operate_5freq_5fbase_2eproto();
  return *default_instance_;
}

OperateReqBase* OperateReqBase::default_instance_ = NULL;

OperateReqBase* OperateReqBase::New(::google::protobuf::Arena* arena) const {
  OperateReqBase* n = new OperateReqBase;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void OperateReqBase::Clear() {
#define ZR_HELPER_(f) reinterpret_cast<char*>(\
  &reinterpret_cast<OperateReqBase*>(16)->f)

#define ZR_(first, last) do {\
  ::memset(&first, 0,\
           ZR_HELPER_(last) - ZR_HELPER_(first) + sizeof(last));\
} while (0)

  ZR_(user_id_, channel_id_);

#undef ZR_HELPER_
#undef ZR_

}

bool OperateReqBase::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:IM.OperateReqBase)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int64 user_id = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &user_id_)));

        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_channel_id;
        break;
      }

      // optional int32 channel_id = 2;
      case 2: {
        if (tag == 16) {
         parse_channel_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &channel_id_)));

        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:IM.OperateReqBase)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:IM.OperateReqBase)
  return false;
#undef DO_
}

void OperateReqBase::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:IM.OperateReqBase)
  // optional int64 user_id = 1;
  if (this->user_id() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->user_id(), output);
  }

  // optional int32 channel_id = 2;
  if (this->channel_id() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->channel_id(), output);
  }

  // @@protoc_insertion_point(serialize_end:IM.OperateReqBase)
}

::google::protobuf::uint8* OperateReqBase::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:IM.OperateReqBase)
  // optional int64 user_id = 1;
  if (this->user_id() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->user_id(), target);
  }

  // optional int32 channel_id = 2;
  if (this->channel_id() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->channel_id(), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:IM.OperateReqBase)
  return target;
}

int OperateReqBase::ByteSize() const {
  int total_size = 0;

  // optional int64 user_id = 1;
  if (this->user_id() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int64Size(
        this->user_id());
  }

  // optional int32 channel_id = 2;
  if (this->channel_id() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->channel_id());
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void OperateReqBase::MergeFrom(const ::google::protobuf::Message& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const OperateReqBase* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const OperateReqBase>(
          &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void OperateReqBase::MergeFrom(const OperateReqBase& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  if (from.user_id() != 0) {
    set_user_id(from.user_id());
  }
  if (from.channel_id() != 0) {
    set_channel_id(from.channel_id());
  }
}

void OperateReqBase::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void OperateReqBase::CopyFrom(const OperateReqBase& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool OperateReqBase::IsInitialized() const {

  return true;
}

void OperateReqBase::Swap(OperateReqBase* other) {
  if (other == this) return;
  InternalSwap(other);
}
void OperateReqBase::InternalSwap(OperateReqBase* other) {
  std::swap(user_id_, other->user_id_);
  std::swap(channel_id_, other->channel_id_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata OperateReqBase::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = OperateReqBase_descriptor_;
  metadata.reflection = OperateReqBase_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// OperateReqBase

// optional int64 user_id = 1;
void OperateReqBase::clear_user_id() {
  user_id_ = GOOGLE_LONGLONG(0);
}
 ::google::protobuf::int64 OperateReqBase::user_id() const {
  // @@protoc_insertion_point(field_get:IM.OperateReqBase.user_id)
  return user_id_;
}
 void OperateReqBase::set_user_id(::google::protobuf::int64 value) {
  
  user_id_ = value;
  // @@protoc_insertion_point(field_set:IM.OperateReqBase.user_id)
}

// optional int32 channel_id = 2;
void OperateReqBase::clear_channel_id() {
  channel_id_ = 0;
}
 ::google::protobuf::int32 OperateReqBase::channel_id() const {
  // @@protoc_insertion_point(field_get:IM.OperateReqBase.channel_id)
  return channel_id_;
}
 void OperateReqBase::set_channel_id(::google::protobuf::int32 value) {
  
  channel_id_ = value;
  // @@protoc_insertion_point(field_set:IM.OperateReqBase.channel_id)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace IM

// @@protoc_insertion_point(global_scope)
