//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include "../code_analysis/assembler.hpp"
#include "../code_analysis/function.hpp"

void shuffle_bbs(vector<BasicBlock>& cfg);
bool mutate_ret_no_op(BasicBlock& bb);
bool mutate_mov(BasicBlock& bb);

struct PassChanges
{
  uint64_t label;
  uint64_t how_many_new;
  Instruction* new_insts;
};

class Obfuscator
{
private:
  vector<Function> functions;
  ZydisMachineMode machine_mode;
  Assembler x86asm;
//  elf_format elf_file;

public:
  Obfuscator (const ZydisMachineMode machine_mode);
  void obfuscate();
};
