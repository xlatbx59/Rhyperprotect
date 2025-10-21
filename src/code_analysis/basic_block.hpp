//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include <iostream>
#include <vector>
#include "instruction.hpp"

using std::vector;

class BasicBlock
{
public:
	uint64_t bb_addr;
	uint64_t last_addr;
	uint64_t fall_addr;
	uint64_t branch_addr;
	vector<Instruction> insts;
	bool has_fall;
	bool has_branch;
  ZydisMachineMode machine_mode;

public:
	Instruction operator[](uint32_t index) const noexcept;
	uint32_t size(void) const noexcept;

  bool split(uint64_t index, BasicBlock& bb) noexcept;
  bool set_branch(uint64_t branch) noexcept;
  bool replace_inst(const uint64_t label, const uint32_t num, const Instruction* inst) noexcept;
};

BasicBlock new_bb(Instruction& inst);
BasicBlock new_bb(vector<Instruction>& insts);
