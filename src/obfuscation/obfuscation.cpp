//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include <stdint.h>
#include "obfuscation.hpp"
#include "../utils.hpp"
#include "../code_analysis/function.hpp"

using std::swap;

//void Obfuscator::shuffle_bbs(Function& function, Blob* blob)
//{
//  static uint64_t label = 0xb00b000000000000;
//  uint32_t bb_num = function.cfg.size();
//  ZydisDecodedOperand operand;
//  operand.type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
//
//  for(int j = 0; j < 3; j++)
//  {
//    for(int i = function.cfg.size() - 1; i > 0; i--)
//    {
//      uint8_t rand_temp;
//      srand(time(NULL));
//      rand_temp = rand() % i;
//
//      swap(function.cfg[rand_temp], function.cfg[i]);
//    }
//  }
//
//  //Since the basic blocks have been scattered, the instructions 
//  //with no branches that fall to a new basic block have to jump now
//  for(int i = 0; i < bb_num; i++)
//  {
//    if( function.cfg[i].branch_type == Fall || function.cfg[i].branch_type == CondBranch || function.cfg[i].branch_type == Call &&
//        (i != function.cfg.size() - 1 && function.cfg[i].fall_addr != function.cfg[i + 1].bb_addr || i == function.cfg.size() - 1))
//    {
//      //Inserting jmps
//      if(!function.cfg[i].branch_type == UncondBranch || !function.cfg[i].branch_type == CondBranch)
//        function.cfg[i].set_branch(function.cfg[i].fall_addr);
//      else
//      {
//        Instruction JMP_DIRECT(jmp, label, &operand, function.cfg[i].fall_addr, function.cfg[i].machine_mode);
//        function.cfg[i].fall_addr = label++;
//        function.cfg.insert(function.cfg.begin() + i + 1, new_bb(jmp));
//        bb_num++;
//      }
//    }
//  }
//}
