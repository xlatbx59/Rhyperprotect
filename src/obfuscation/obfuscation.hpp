//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include "../code_analysis/assembler.hpp"
#include "../code_analysis/function.hpp"

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

public:
  Obfuscator (const ZydisMachineMode machine_mode);
  void shuffle_bbs(Function& function, Blob* blob);
  bool mutate_jmp(BasicBlock& bb, Blob* blob);
  void encryption(Function& function, Blob* blob);
  void obfuscate();
};
