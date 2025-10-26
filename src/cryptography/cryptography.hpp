//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include <iostream>
#include <stdint.h>

bool rc4( uint8_t *plain, const uint32_t plain_size,
          const uint8_t *key, const uint32_t key_size);
