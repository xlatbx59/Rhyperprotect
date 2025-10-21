//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include "basic_block.hpp"

BasicBlock new_bb(Instruction& inst)
{
	BasicBlock bb;

	bb.bb_addr = inst.get_address();
	bb.last_addr = inst.get_address();
  bb.has_fall = inst.has_fall();
  bb.has_branch = inst.has_branch();
  bb.insts.push_back(inst);
	inst.get_branch(bb.branch_addr);
	inst.get_fall(bb.fall_addr);
  bb.machine_mode = inst.get_machine_mode();
  
	return bb;
}

BasicBlock new_bb(vector<Instruction>& insts)
{
	BasicBlock bb;
	bb.insts = insts;
	bb.bb_addr = insts[0].get_address();
  bb.last_addr = insts[insts.size() - 1].get_address();
	insts[insts.size() - 1].get_branch(bb.branch_addr);
	insts[insts.size() - 1].get_fall(bb.fall_addr);
  bb.has_fall = insts[insts.size() - 1].has_fall();
  bb.has_branch = insts[insts.size() - 1].has_branch();
  bb.machine_mode = insts[0].get_machine_mode();

	return bb;
}

Instruction BasicBlock::operator[](uint32_t index) const noexcept
{
	return this->insts[index];
}
uint32_t BasicBlock::size(void) const noexcept
{
	return this->insts.size();
}
bool BasicBlock::split(uint64_t addr, BasicBlock& bb) noexcept
{
  vector<Instruction> new_insts;

  for(int i = 0; i < this->insts.size(); i++)
  {
    if(addr == this->insts[i].get_address())
    {
      for(int j = i; j < this->insts.size(); j++)
        new_insts.push_back(this->insts[j]);

      this->insts.erase(this->insts.begin() + i, this->insts.end());
      this->has_fall = true;
      this->has_branch = false;
      bb = new_bb(new_insts);
      this->fall_addr = bb.bb_addr;
      return true;
    }
  }
  return false;
}

bool BasicBlock::set_branch(uint64_t branch) noexcept
{
  static uint64_t label = 0xa550000000000000;
  ZydisDecodedOperand operand;
  operand.type = ZYDIS_OPERAND_TYPE_IMMEDIATE;

  if(this->has_branch)
    this->branch_addr = branch;

  Instruction JMP_DIRECT(jmp, label++, &operand, this->fall_addr, this->machine_mode);
  this->insts.push_back(jmp);

  return true;
}
//Not finish
bool BasicBlock::replace_inst(const uint64_t label_to_replace, const uint32_t num, const Instruction* inst) noexcept
{
  static uint64_t label = 0xdead000000000000;

  for(int i = 0; i < insts.size(); i++)
  {
    if(label == this->insts[i].label)
    {
      return true;
    }
  }
  return false;
}
