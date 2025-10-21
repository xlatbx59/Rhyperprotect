//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include "../code_analysis/basic_block.hpp"

void shuffle_bbs(vector<BasicBlock>& cfg);
bool mutate_ret_no_op(BasicBlock& bb);
bool mutate_mov(BasicBlock& bb);
