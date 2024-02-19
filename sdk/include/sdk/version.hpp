// Copyright 2024 TeiaCare
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

namespace tc::sdk::info
{
extern const char* const name;
extern const char* const version;

extern const char* const project_description;
extern const char* const project_url;

extern const char* const build_type;
extern const char* const compiler_name;
extern const char* const compiler_version;

extern const char* const cxx_flags;
extern const char* const cxx_flags_debug;
extern const char* const cxx_flags_release;
extern const char* const cxx_standard;

extern const char* const os_name;
extern const char* const os_version;
extern const char* const os_processor;
}
