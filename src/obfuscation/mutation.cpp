//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include "../code_analysis/basic_block.hpp"
#include <string.h>

bool mutate_ret_no_op(BasicBlock& bb)
{
  static uint64_t label = 0xbeef000000000000;
  ZydisDecodedOperand add_operand[2], jmp_mem;

  memset(add_operand, 0, sizeof(add_operand));
  memset(&jmp_mem, 0, sizeof(jmp_mem));

  add_operand[1].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
  add_operand[0].type = ZYDIS_OPERAND_TYPE_REGISTER;
  jmp_mem.type = ZYDIS_OPERAND_TYPE_MEMORY;


  if(bb.machine_mode == ZYDIS_MACHINE_MODE_LONG_64)
  {
    add_operand[1].imm.value.u = 8;
    add_operand[0].reg.value = ZYDIS_REGISTER_RSP;
    jmp_mem.mem.disp.value = -8;
    jmp_mem.mem.base = ZYDIS_REGISTER_RSP;
    jmp_mem.size = 64;
  }
  else if (bb.machine_mode == ZYDIS_MACHINE_MODE_LONG_COMPAT_32)
  {
    add_operand[1].imm.value.u = 4;
    add_operand[0].reg.value = ZYDIS_REGISTER_ESP;
    jmp_mem.mem.disp.value = -4;
    jmp_mem.mem.base = ZYDIS_REGISTER_ESP;
    jmp_mem.size = 32;
  }

  Instruction INST_NO_BRANCH(add, ZYDIS_MNEMONIC_ADD, label, add_operand, 2, bb.machine_mode, label + 1);
  label++;
  Instruction JMP_INDIRECT(jmp, label++, &jmp_mem, bb.machine_mode);

  for(int i = 0; i < bb.size(); i++)
  {
    if(ZYDIS_MNEMONIC_RET == bb[i].get_mnemonic())
    {
      if(i != 0)
        bb[i - 1].set_fall(add.label);
      bb.insts.pop_back();
      bb.insts.push_back(add);
      bb.insts.push_back(jmp);
      break;
    }
  }

  return true;
}
bool mutate_mov(BasicBlock& bb)
{
  static uint64_t label = 0xfee7000000000000;
  ZydisDecodedOperand mov_operand[2];
  memset(mov_operand, 0, sizeof(mov_operand));

  for(int i = 0; i < bb.size(); i++)
  {
    if(ZYDIS_MNEMONIC_MOV == bb[i].get_mnemonic())
    {

      bb[i].get_operand(mov_operand[0], 0);
      bb[i].get_operand(mov_operand[1], 1);

      if(bb.machine_mode == ZYDIS_MACHINE_MODE_LONG_64)
        if(mov_operand[0].size != 64 || mov_operand[1].size != 64)
          continue;
      else if(bb.machine_mode == ZYDIS_MACHINE_MODE_LONG_COMPAT_32)
        if(mov_operand[0].size != 32 || mov_operand[1].size != 32)
          continue;

      Instruction INST_NO_BRANCH(push, ZYDIS_MNEMONIC_PUSH, bb[i].label, mov_operand + 1, 1, bb.machine_mode, label++);
      Instruction INST_NO_BRANCH(pop, ZYDIS_MNEMONIC_POP, label, mov_operand, 1, bb.machine_mode, label + 1);

      bb.insts.insert(bb.insts.begin() + i + 1, push);
      bb.insts.insert(bb.insts.begin() + i + 2, pop);
      bb.insts.erase(bb.insts.begin() + i);
    }
  }

  return true;
}

bool Obfuscator::mutate_jmp(BasicBlock& bb, Blob* blob)
{
  for(int i = 0; i < bb.size(); i++)
  {
    if(bb.insts[i].get_mnemonic() == ZYDIS_MNEMONIC_JMP)
        Instruction JNZ_DIRECT(jnz, bb.insts[i].label, &operand, function.cfg[i].fall_addr, function.cfg[i].machine_mode);
        Instruction JZ_DIRECT(jz, bb.insts[i].label, &operand, function.cfg[i].fall_addr, function.cfg[i].machine_mode);
  }
  return false;
}
