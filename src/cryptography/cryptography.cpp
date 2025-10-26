//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include "cryptography.hpp"

using std::swap;

bool rc4(uint8_t* buffer, const uint32_t buffer_size, const uint8_t* key, const uint32_t key_size)
{
  int j = 0, i = 0;
  uint8_t sbox[256];

  if(!buffer || !key || !key_size || !buffer_size || key_size > 256)
    return false;

  for(i = 0; i < 256; i++)
    sbox[i] = i;

  for(i = 0; i < 256; i++)
  {
    j = (j + sbox[i] + key[i % key_size]) % 256;
    swap(sbox[i], sbox[j]);
  }

  i = 0;
  j = 0;
  for(int h = 0; h <buffer_size; h++)
  {
    (i += 1) %= 256;
    (j += sbox[i]) %= 256;
    swap(sbox[i], sbox[j]);
    buffer[h] ^= sbox[(sbox[i] + sbox[j]) % 256];
  }

  return true;
}
