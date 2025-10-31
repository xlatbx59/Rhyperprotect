//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include "basic_block.hpp"

BasicBlock new_bb(Instruction& inst)
{
	BasicBlock bb;

	bb.bb_addr = inst.get_address();
  bb.branch_type = inst.get_branch_type();
  bb.insts.push_back(inst);
  if(bb.branch_type == UncondBranch || bb.branch_type == CondBranch)
    inst.get_ref(0, bb.branch_addr);
  if(bb.branch_type == Fall || bb.branch_type == CondBranch || bb.branch_type == Call)
    inst.get_fall(bb.fall_addr);
	inst.get_fall(bb.fall_addr);
  bb.machine_mode = inst.get_machine_mode();
  
	return bb;
}

BasicBlock new_bb(vector<Instruction>& insts)
{
	BasicBlock bb;
	bb.insts = insts;
	bb.bb_addr = insts[0].get_address();
  if(bb.branch_type == UncondBranch || bb.branch_type == CondBranch)
    insts[insts.size() - 1].get_ref(0, bb.branch_addr);
  if(bb.branch_type == Fall || bb.branch_type == CondBranch || bb.branch_type == Call)
    insts[insts.size() - 1].get_fall(bb.fall_addr);	
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
      this->branch_type = Fall;
      bb = new_bb(new_insts);
      this->fall_addr = bb.bb_addr;
      return true;
    }
  }
  return false;
}
