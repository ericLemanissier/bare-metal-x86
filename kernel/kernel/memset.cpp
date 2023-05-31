import <cstddef>;

extern "C"
void *
memset (unsigned char *dest, int val, size_t len)
{
  unsigned char *ptr = dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}
