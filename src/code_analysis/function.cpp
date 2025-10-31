//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include "function.hpp"

Function::Function(const std::vector<BasicBlock>& cfg)
{
  Blocks block();
  bool flag = false;
  ZydisDecodedOperand operand;
  this->label_var_allocator = nullptr;

  block.type = BlockType::basic_block;
  for(BasicBlock bb: cfg)
  {
    block.bb = bb;
    this->cfg.push_back(block);

    for(Instruction inst : bb.insts)
    {
      if(inst.get_mnemonic() == ZYDIS_MNEMONIC_PUSH)
      {
        if(operand.type != ZYDIS_OPERAND_TYPE_REGISTER)
          continue;
        this->callee_saved.push_back(operand.reg.value);
      }
      else if(inst.get_mnemonic() == ZYDIS_MNEMONIC_SUB)
      {
        inst.get_operand(operand, 0);
        if(operand.type != ZYDIS_OPERAND_TYPE_REGISTER ||
        inst.get_machine_mode() == ZYDIS_MACHINE_MODE_LONG_COMPAT_32 && operand.reg.value != ZYDIS_REGISTER_ESP
        || inst.get_machine_mode() == ZYDIS_MACHINE_MODE_LONG_64 && operand.reg.value != ZYDIS_REGISTER_RSP)
          continue;
        
        inst.get_operand(operand, 1);
        if(operand.type != ZYDIS_OPERAND_TYPE_IMMEDIATE)
          continue;

        this->stack_size = operand.imm.value.u;
        this->label_var_allocator = &inst;
        flag = true;
        break;
      }
      if(flag)
        break;
    }
  }
}

Function::Function(BasicBlock bb)
{
  ZydisDecodedOperand operand;
  Blocks block();
  this->cfg.push_back(bb);
  this->label_var_allocator = nullptr;
  Block block = {BlockType::basic_block, bb};

  for(Instruction inst : bb.insts)
  {
    if(inst.get_mnemonic() == ZYDIS_MNEMONIC_PUSH)
    {
      if(operand.type != ZYDIS_OPERAND_TYPE_REGISTER)
        continue;
      this->callee_saved.push_back(operand.reg.value);
    }
    if(inst.get_mnemonic() == ZYDIS_MNEMONIC_SUB)
    {
      inst.get_operand(operand, 0);
      if(operand.type != ZYDIS_OPERAND_TYPE_REGISTER ||
      inst.get_machine_mode() == ZYDIS_MACHINE_MODE_LONG_COMPAT_32 && operand.reg.value != ZYDIS_REGISTER_ESP
      || inst.get_machine_mode() == ZYDIS_MACHINE_MODE_LONG_64 && operand.reg.value != ZYDIS_REGISTER_RSP)
        continue;
      
      inst.get_operand(operand, 1);
      if(operand.type != ZYDIS_OPERAND_TYPE_IMMEDIATE)
        continue;

      this->stack_size = operand.imm.value.u;
      this->label_var_allocator = &inst;
      break;
    }
  }
}
Function::~Function()
{
  if(label_var_allocator)
    delete [] label_var_allocator;
}
