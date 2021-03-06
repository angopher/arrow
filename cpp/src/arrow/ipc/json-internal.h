// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef ARROW_IPC_JSON_INTERNAL_H
#define ARROW_IPC_JSON_INTERNAL_H

#include <memory>
#include <sstream>
#include <string>

#include "arrow/json/rapidjson-defs.h"
#include "rapidjson/document.h"      // IWYU pragma: export
#include "rapidjson/encodings.h"     // IWYU pragma: export
#include "rapidjson/error/en.h"      // IWYU pragma: export
#include "rapidjson/stringbuffer.h"  // IWYU pragma: export
#include "rapidjson/writer.h"        // IWYU pragma: export

#include "arrow/status.h"    // IWYU pragma: export
#include "arrow/type_fwd.h"  // IWYU pragma: keep
#include "arrow/util/visibility.h"

namespace rj = arrow::rapidjson;
using RjWriter = rj::Writer<rj::StringBuffer>;
using RjArray = rj::Value::ConstArray;
using RjObject = rj::Value::ConstObject;

#define RETURN_NOT_FOUND(TOK, NAME, PARENT)              \
  if (NAME == (PARENT).MemberEnd()) {                    \
    return Status::Invalid("field ", TOK, " not found"); \
  }

#define RETURN_NOT_STRING(TOK, NAME, PARENT)                          \
  RETURN_NOT_FOUND(TOK, NAME, PARENT);                                \
  if (!NAME->value.IsString()) {                                      \
    return Status::Invalid("field was not a string line ", __LINE__); \
  }

#define RETURN_NOT_BOOL(TOK, NAME, PARENT)                             \
  RETURN_NOT_FOUND(TOK, NAME, PARENT);                                 \
  if (!NAME->value.IsBool()) {                                         \
    return Status::Invalid("field was not a boolean line ", __LINE__); \
  }

#define RETURN_NOT_INT(TOK, NAME, PARENT)                           \
  RETURN_NOT_FOUND(TOK, NAME, PARENT);                              \
  if (!NAME->value.IsInt()) {                                       \
    return Status::Invalid("field was not an int line ", __LINE__); \
  }

#define RETURN_NOT_ARRAY(TOK, NAME, PARENT)                           \
  RETURN_NOT_FOUND(TOK, NAME, PARENT);                                \
  if (!NAME->value.IsArray()) {                                       \
    return Status::Invalid("field was not an array line ", __LINE__); \
  }

#define RETURN_NOT_OBJECT(TOK, NAME, PARENT)                           \
  RETURN_NOT_FOUND(TOK, NAME, PARENT);                                 \
  if (!NAME->value.IsObject()) {                                       \
    return Status::Invalid("field was not an object line ", __LINE__); \
  }

namespace arrow {
namespace ipc {

class DictionaryMemo;

namespace internal {
namespace json {

/// \brief Append integration test Schema format to rapidjson writer
ARROW_EXPORT
Status WriteSchema(const Schema& schema, DictionaryMemo* dict_memo, RjWriter* writer);

ARROW_EXPORT
Status WriteDictionary(int64_t id, const std::shared_ptr<Array>& dictionary,
                       RjWriter* writer);

ARROW_EXPORT
Status WriteRecordBatch(const RecordBatch& batch, RjWriter* writer);

ARROW_EXPORT
Status WriteArray(const std::string& name, const Array& array, RjWriter* writer);

ARROW_EXPORT
Status ReadSchema(const rj::Value& json_obj, MemoryPool* pool,
                  DictionaryMemo* dictionary_memo, std::shared_ptr<Schema>* schema);

ARROW_EXPORT
Status ReadRecordBatch(const rj::Value& json_obj, const std::shared_ptr<Schema>& schema,
                       DictionaryMemo* dict_memo, MemoryPool* pool,
                       std::shared_ptr<RecordBatch>* batch);

ARROW_EXPORT
Status ReadArray(MemoryPool* pool, const rj::Value& json_obj,
                 const std::shared_ptr<Field>& type, DictionaryMemo* dict_memo,
                 std::shared_ptr<Array>* array);

ARROW_EXPORT
Status ReadArray(MemoryPool* pool, const rj::Value& json_obj, const Schema& schema,
                 DictionaryMemo* dict_memo, std::shared_ptr<Array>* array);

}  // namespace json
}  // namespace internal
}  // namespace ipc
}  // namespace arrow

#endif  // ARROW_IPC_JSON_INTERNAL_H
