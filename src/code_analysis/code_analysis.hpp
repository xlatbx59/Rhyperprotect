//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include "function.hpp"


bool disassemble( ZydisDecoder& decoder,
                      vector<BasicBlock>& cfg,
                      vector<uint64_t>& disassembled,
                      const uint8_t* start,
                      const uint64_t size,
                      uint64_t pc,
                      bool bits = true,
                      ZyanU64 runtime_address = 0x00400000);
