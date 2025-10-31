//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include "basic_block.hpp"
#include <string.h>

enum BlockType
{
  basic_block, DataBlock
};
struct Blob
{
  uint64_t label;
  uint64_t size;
  uint8_t* data;
};
struct Blocks
{
  Blocks() {}
  BlockType type;
  union 
  {
    Blob blob;
    BasicBlock bb;
  };
};

class Function
{
private:
  uint64_t stack_size;
  Instruction* label_var_allocator;
  vector<ZydisRegister> callee_saved;
  vector<Blocks> blocks;
  //std::vector<uint64_t> called_func;

public:
  std::vector<Blocks> cfg;
  Function(BasicBlock bb);
  Function(const std::vector<BasicBlock>& bbs);
  Function(const BasicBlock* bbs, const uint64_t size);
  void new_var(const uint8_t var_size);    //var_size in bits
  ~Function();
};
