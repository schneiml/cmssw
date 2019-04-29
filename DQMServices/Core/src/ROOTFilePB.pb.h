// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: DQMServices/Core/src/ROOTFilePB.proto

#ifndef PROTOBUF_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto__INCLUDED
#define PROTOBUF_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
void InitDefaultsROOTFilePB_HistoImpl();
void InitDefaultsROOTFilePB_Histo();
void InitDefaultsROOTFilePBImpl();
void InitDefaultsROOTFilePB();
inline void InitDefaults() {
  InitDefaultsROOTFilePB_Histo();
  InitDefaultsROOTFilePB();
}
}  // namespace protobuf_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto
namespace dqmstorepb {
class ROOTFilePB;
class ROOTFilePBDefaultTypeInternal;
extern ROOTFilePBDefaultTypeInternal _ROOTFilePB_default_instance_;
class ROOTFilePB_Histo;
class ROOTFilePB_HistoDefaultTypeInternal;
extern ROOTFilePB_HistoDefaultTypeInternal _ROOTFilePB_Histo_default_instance_;
}  // namespace dqmstorepb
namespace dqmstorepb {

// ===================================================================

class ROOTFilePB_Histo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:dqmstorepb.ROOTFilePB.Histo) */ {
 public:
  ROOTFilePB_Histo();
  ~ROOTFilePB_Histo() override;

  ROOTFilePB_Histo(const ROOTFilePB_Histo& from);

/* almost unused */   inline ROOTFilePB_Histo& operator=(const ROOTFilePB_Histo& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ROOTFilePB_Histo(ROOTFilePB_Histo&& from) noexcept
    : ROOTFilePB_Histo() {
    *this = ::std::move(from);
  }

  inline ROOTFilePB_Histo& operator=(ROOTFilePB_Histo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
/* almost unused */   inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
/* almost unused */   inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

/* almost unused */   static const ::google::protobuf::Descriptor* descriptor();
/* almost unused */   static const ROOTFilePB_Histo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ROOTFilePB_Histo* internal_default_instance() {
    return reinterpret_cast<const ROOTFilePB_Histo*>(
               &_ROOTFilePB_Histo_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(ROOTFilePB_Histo* other);
  friend void swap(ROOTFilePB_Histo& a, ROOTFilePB_Histo& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

/* almost unused */   inline ROOTFilePB_Histo* New() const PROTOBUF_FINAL { return New(nullptr); }

  ROOTFilePB_Histo* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
/* almost unused */   void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const ROOTFilePB_Histo& from);
  void MergeFrom(const ROOTFilePB_Histo& from);
  void Clear() PROTOBUF_FINAL;
/* almost unused */   bool IsInitialized() const PROTOBUF_FINAL;

/* almost unused */   size_t ByteSizeLong() const PROTOBUF_FINAL;
/* almost unused */   bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
/* almost unused */   void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
/* almost unused */   ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
/* almost unused */   int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
/* almost unused */   void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(ROOTFilePB_Histo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
/* almost unused */   inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

/* almost unused */   ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string full_pathname = 1;
  bool has_full_pathname() const;
/* almost unused */   void clear_full_pathname();
  static const int kFullPathnameFieldNumber = 1;
  const ::std::string& full_pathname() const;
  void set_full_pathname(const ::std::string& value);
  #if LANG_CXX11
/* almost unused */   void set_full_pathname(::std::string&& value);
  #endif
/* almost unused */   void set_full_pathname(const char* value);
/* almost unused */   void set_full_pathname(const char* value, size_t size);
  ::std::string* mutable_full_pathname();
/* almost unused */   ::std::string* release_full_pathname();
/* almost unused */   void set_allocated_full_pathname(::std::string* full_pathname);

  // required bytes streamed_histo = 3;
  bool has_streamed_histo() const;
/* almost unused */   void clear_streamed_histo();
  static const int kStreamedHistoFieldNumber = 3;
  const ::std::string& streamed_histo() const;
/* almost unused */   void set_streamed_histo(const ::std::string& value);
  #if LANG_CXX11
/* almost unused */   void set_streamed_histo(::std::string&& value);
  #endif
/* almost unused */   void set_streamed_histo(const char* value);
  void set_streamed_histo(const void* value, size_t size);
  ::std::string* mutable_streamed_histo();
/* almost unused */   ::std::string* release_streamed_histo();
/* almost unused */   void set_allocated_streamed_histo(::std::string* streamed_histo);

  // required uint32 size = 2;
  bool has_size() const;
/* almost unused */   void clear_size();
  static const int kSizeFieldNumber = 2;
  ::google::protobuf::uint32 size() const;
  void set_size(::google::protobuf::uint32 value);

  // required uint32 flags = 4;
  bool has_flags() const;
/* almost unused */   void clear_flags();
  static const int kFlagsFieldNumber = 4;
  ::google::protobuf::uint32 flags() const;
  void set_flags(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:dqmstorepb.ROOTFilePB.Histo)
 private:
  void set_has_full_pathname();
  void clear_has_full_pathname();
  void set_has_size();
  void clear_has_size();
  void set_has_streamed_histo();
  void clear_has_streamed_histo();
  void set_has_flags();
  void clear_has_flags();

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr full_pathname_;
  ::google::protobuf::internal::ArenaStringPtr streamed_histo_;
  ::google::protobuf::uint32 size_;
  ::google::protobuf::uint32 flags_;
  friend struct ::protobuf_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto::TableStruct;
  friend void ::protobuf_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto::InitDefaultsROOTFilePB_HistoImpl();
};
// -------------------------------------------------------------------

class ROOTFilePB : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:dqmstorepb.ROOTFilePB) */ {
 public:
  ROOTFilePB();
  ~ROOTFilePB() override;

  ROOTFilePB(const ROOTFilePB& from);

/* almost unused */   inline ROOTFilePB& operator=(const ROOTFilePB& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ROOTFilePB(ROOTFilePB&& from) noexcept
    : ROOTFilePB() {
    *this = ::std::move(from);
  }

  inline ROOTFilePB& operator=(ROOTFilePB&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
/* almost unused */   inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
/* almost unused */   inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

/* almost unused */   static const ::google::protobuf::Descriptor* descriptor();
/* almost unused */   static const ROOTFilePB& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ROOTFilePB* internal_default_instance() {
    return reinterpret_cast<const ROOTFilePB*>(
               &_ROOTFilePB_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(ROOTFilePB* other);
  friend void swap(ROOTFilePB& a, ROOTFilePB& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

/* almost unused */   inline ROOTFilePB* New() const PROTOBUF_FINAL { return New(nullptr); }

  ROOTFilePB* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
/* almost unused */   void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const ROOTFilePB& from);
  void MergeFrom(const ROOTFilePB& from);
  void Clear() PROTOBUF_FINAL;
/* almost unused */   bool IsInitialized() const PROTOBUF_FINAL;

/* almost unused */   size_t ByteSizeLong() const PROTOBUF_FINAL;
/* almost unused */   bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
/* almost unused */   void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
/* almost unused */   ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
/* almost unused */   int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
/* almost unused */   void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(ROOTFilePB* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
/* almost unused */   inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

/* almost unused */   ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  typedef ROOTFilePB_Histo Histo;

  // accessors -------------------------------------------------------

  // repeated .dqmstorepb.ROOTFilePB.Histo histo = 1;
  int histo_size() const;
/* almost unused */   void clear_histo();
  static const int kHistoFieldNumber = 1;
  const ::dqmstorepb::ROOTFilePB_Histo& histo(int index) const;
/* almost unused */   ::dqmstorepb::ROOTFilePB_Histo* mutable_histo(int index);
  ::dqmstorepb::ROOTFilePB_Histo* add_histo();
  ::google::protobuf::RepeatedPtrField< ::dqmstorepb::ROOTFilePB_Histo >*
/* almost unused */       mutable_histo();
  const ::google::protobuf::RepeatedPtrField< ::dqmstorepb::ROOTFilePB_Histo >&
      histo() const;

  // @@protoc_insertion_point(class_scope:dqmstorepb.ROOTFilePB)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::dqmstorepb::ROOTFilePB_Histo > histo_;
  friend struct ::protobuf_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto::TableStruct;
  friend void ::protobuf_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto::InitDefaultsROOTFilePBImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ROOTFilePB_Histo

// required string full_pathname = 1;
inline bool ROOTFilePB_Histo::has_full_pathname() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ROOTFilePB_Histo::set_has_full_pathname() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ROOTFilePB_Histo::clear_has_full_pathname() {
  _has_bits_[0] &= ~0x00000001u;
}
/* almost unused */ inline void ROOTFilePB_Histo::clear_full_pathname() {
  full_pathname_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_full_pathname();
}
inline const ::std::string& ROOTFilePB_Histo::full_pathname() const {
  // @@protoc_insertion_point(field_get:dqmstorepb.ROOTFilePB.Histo.full_pathname)
  return full_pathname_.GetNoArena();
}
inline void ROOTFilePB_Histo::set_full_pathname(const ::std::string& value) {
  set_has_full_pathname();
  full_pathname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:dqmstorepb.ROOTFilePB.Histo.full_pathname)
}
#if LANG_CXX11
/* almost unused */ inline void ROOTFilePB_Histo::set_full_pathname(::std::string&& value) {
  set_has_full_pathname();
  full_pathname_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:dqmstorepb.ROOTFilePB.Histo.full_pathname)
}
#endif
/* almost unused */ inline void ROOTFilePB_Histo::set_full_pathname(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  set_has_full_pathname();
  full_pathname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:dqmstorepb.ROOTFilePB.Histo.full_pathname)
}
/* almost unused */ inline void ROOTFilePB_Histo::set_full_pathname(const char* value, size_t size) {
  set_has_full_pathname();
  full_pathname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:dqmstorepb.ROOTFilePB.Histo.full_pathname)
}
inline ::std::string* ROOTFilePB_Histo::mutable_full_pathname() {
  set_has_full_pathname();
  // @@protoc_insertion_point(field_mutable:dqmstorepb.ROOTFilePB.Histo.full_pathname)
  return full_pathname_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
/* almost unused */ inline ::std::string* ROOTFilePB_Histo::release_full_pathname() {
  // @@protoc_insertion_point(field_release:dqmstorepb.ROOTFilePB.Histo.full_pathname)
  clear_has_full_pathname();
  return full_pathname_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
/* almost unused */ inline void ROOTFilePB_Histo::set_allocated_full_pathname(::std::string* full_pathname) {
  if (full_pathname != nullptr) {
    set_has_full_pathname();
  } else {
    clear_has_full_pathname();
  }
  full_pathname_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), full_pathname);
  // @@protoc_insertion_point(field_set_allocated:dqmstorepb.ROOTFilePB.Histo.full_pathname)
}

// required uint32 size = 2;
inline bool ROOTFilePB_Histo::has_size() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ROOTFilePB_Histo::set_has_size() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ROOTFilePB_Histo::clear_has_size() {
  _has_bits_[0] &= ~0x00000004u;
}
/* almost unused */ inline void ROOTFilePB_Histo::clear_size() {
  size_ = 0u;
  clear_has_size();
}
inline ::google::protobuf::uint32 ROOTFilePB_Histo::size() const {
  // @@protoc_insertion_point(field_get:dqmstorepb.ROOTFilePB.Histo.size)
  return size_;
}
inline void ROOTFilePB_Histo::set_size(::google::protobuf::uint32 value) {
  set_has_size();
  size_ = value;
  // @@protoc_insertion_point(field_set:dqmstorepb.ROOTFilePB.Histo.size)
}

// required bytes streamed_histo = 3;
inline bool ROOTFilePB_Histo::has_streamed_histo() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ROOTFilePB_Histo::set_has_streamed_histo() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ROOTFilePB_Histo::clear_has_streamed_histo() {
  _has_bits_[0] &= ~0x00000002u;
}
/* almost unused */ inline void ROOTFilePB_Histo::clear_streamed_histo() {
  streamed_histo_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_streamed_histo();
}
inline const ::std::string& ROOTFilePB_Histo::streamed_histo() const {
  // @@protoc_insertion_point(field_get:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
  return streamed_histo_.GetNoArena();
}
/* almost unused */ inline void ROOTFilePB_Histo::set_streamed_histo(const ::std::string& value) {
  set_has_streamed_histo();
  streamed_histo_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
}
#if LANG_CXX11
/* almost unused */ inline void ROOTFilePB_Histo::set_streamed_histo(::std::string&& value) {
  set_has_streamed_histo();
  streamed_histo_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
}
#endif
/* almost unused */ inline void ROOTFilePB_Histo::set_streamed_histo(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  set_has_streamed_histo();
  streamed_histo_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
}
inline void ROOTFilePB_Histo::set_streamed_histo(const void* value, size_t size) {
  set_has_streamed_histo();
  streamed_histo_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
}
inline ::std::string* ROOTFilePB_Histo::mutable_streamed_histo() {
  set_has_streamed_histo();
  // @@protoc_insertion_point(field_mutable:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
  return streamed_histo_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
/* almost unused */ inline ::std::string* ROOTFilePB_Histo::release_streamed_histo() {
  // @@protoc_insertion_point(field_release:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
  clear_has_streamed_histo();
  return streamed_histo_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
/* almost unused */ inline void ROOTFilePB_Histo::set_allocated_streamed_histo(::std::string* streamed_histo) {
  if (streamed_histo != nullptr) {
    set_has_streamed_histo();
  } else {
    clear_has_streamed_histo();
  }
  streamed_histo_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), streamed_histo);
  // @@protoc_insertion_point(field_set_allocated:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
}

// required uint32 flags = 4;
inline bool ROOTFilePB_Histo::has_flags() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ROOTFilePB_Histo::set_has_flags() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ROOTFilePB_Histo::clear_has_flags() {
  _has_bits_[0] &= ~0x00000008u;
}
/* almost unused */ inline void ROOTFilePB_Histo::clear_flags() {
  flags_ = 0u;
  clear_has_flags();
}
inline ::google::protobuf::uint32 ROOTFilePB_Histo::flags() const {
  // @@protoc_insertion_point(field_get:dqmstorepb.ROOTFilePB.Histo.flags)
  return flags_;
}
inline void ROOTFilePB_Histo::set_flags(::google::protobuf::uint32 value) {
  set_has_flags();
  flags_ = value;
  // @@protoc_insertion_point(field_set:dqmstorepb.ROOTFilePB.Histo.flags)
}

// -------------------------------------------------------------------

// ROOTFilePB

// repeated .dqmstorepb.ROOTFilePB.Histo histo = 1;
inline int ROOTFilePB::histo_size() const {
  return histo_.size();
}
/* almost unused */ inline void ROOTFilePB::clear_histo() {
  histo_.Clear();
}
inline const ::dqmstorepb::ROOTFilePB_Histo& ROOTFilePB::histo(int index) const {
  // @@protoc_insertion_point(field_get:dqmstorepb.ROOTFilePB.histo)
  return histo_.Get(index);
}
/* almost unused */ inline ::dqmstorepb::ROOTFilePB_Histo* ROOTFilePB::mutable_histo(int index) {
  // @@protoc_insertion_point(field_mutable:dqmstorepb.ROOTFilePB.histo)
  return histo_.Mutable(index);
}
inline ::dqmstorepb::ROOTFilePB_Histo* ROOTFilePB::add_histo() {
  // @@protoc_insertion_point(field_add:dqmstorepb.ROOTFilePB.histo)
  return histo_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::dqmstorepb::ROOTFilePB_Histo >*
/* almost unused */ ROOTFilePB::mutable_histo() {
  // @@protoc_insertion_point(field_mutable_list:dqmstorepb.ROOTFilePB.histo)
  return &histo_;
}
inline const ::google::protobuf::RepeatedPtrField< ::dqmstorepb::ROOTFilePB_Histo >&
ROOTFilePB::histo() const {
  // @@protoc_insertion_point(field_list:dqmstorepb.ROOTFilePB.histo)
  return histo_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace dqmstorepb

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto__INCLUDED
