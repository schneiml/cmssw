#ifndef SIPIXEL_PHASE1_ENUM_H
#define SIPIXEL_PHASE1_ENUM_H
// -*- C++ -*-
//
// Package:    SiPixelPhase1Common
// Macro:      SIPIXEL_PHASE1_ENUM
//
// This provides a simple, macro based C++ 'named' enum, that can convert to 
// string and back. It creates a very normal C++ enum, not scoped, but named 
// if SIPIXEL_PHASE1_NAMED_ENUM is used.
// The unnamed version can only appear once per namespace.
//
// Original Author:  Marcel Schneider
//

#include <iostream>
#include <string>

#include "FWCore/Utilities/interface/Exception.h"

// The magic here is that the enum names are passed thru __VA_ARGS__, and we 
// also get a ", " separated string of the names as #__VA_ARGS__.
// We use this to declare two functions _tostring and _fromstring, that contain
// the constant string and parse it at runtime.
// This could be more efficient (use constexpr to declare a table?) but using 
// strings in critical code is bad anyways, so this is correct, short, and fine
// for now. 
// The conversion function throw (hopefully) helpful exceptions on bad input.
#define SIPIXEL_PHASE1_ENUM_IMPL(name, type, ...) enum name { __VA_ARGS__ }; \
 std::string name##_tostring(type i) { \
   auto max = sizeof((type[]){__VA_ARGS__}) / sizeof(type); \
   if (i >= max) throw cms::Exception("SiPixelPhase1EnumValueInvalid") << "Value '" << i << "' out of range for enum " ##name ", legal values {" #__VA_ARGS__ "}"; \
   const char* str = #__VA_ARGS__; \
   while (i > 0) {if (*str == ',') i--; str++;} \
   while (*str == ' ') str++; \
   const char* end = str; \
   while (*end != '\0' && *end != ',') end++; \
   return std::string(str, end); } \
 type name##_fromstring(std::string val) { \
   auto max = sizeof((type[]){__VA_ARGS__}) / sizeof(type); \
   for (unsigned int i = 0; i < max; i++) \
       if (name##_tostring(i) == val) return (type)i; \
   throw cms::Exception("SiPixelPhase1EnumValueInvalid") << "String name '" << val << "' not in enum " #name ", legal values {" #__VA_ARGS__ "}"; }; \
 unsigned int name##_enum_max() { return sizeof((type[]){__VA_ARGS__}) / sizeof(type); }; 
   
// Declares an anoynymous enum and uses unsigned int as a type for the enum
// Use carefully, since the conversion functions are not properly scoped.
#define SIPIXEL_PHASE1_ENUM(...) SIPIXEL_PHASE1_ENUM_IMPL(, unsigned int, __VA_ARGS__)

// Declares a named enum. The conversion functions are prefixed with the name
// and use the proper enum type. However, the syntax might be confusing.
#define SIPIXEL_PHASE1_NAMED_ENUM(name, ...) SIPIXEL_PHASE1_ENUM_IMPL(name, name, __VA_ARGS__)

#endif
