//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include "basic_block.hpp"
#include <string.h>

class Function
{
private:
  uint64_t stack_size;
  std::vector<BasicBlock> bbs;
  Instruction* label_var_allocator;
  vector<ZydisRegister> callee_saved;
  //std::vector<uint64_t> called_func;

public:
  Function(const BasicBlock bb);
  Function(const std::vector<BasicBlock>& bbs);
  Function(const BasicBlock* bbs, const uint64_t size);
  void new_var(const uint8_t var_size);    //var_size in bits
};
