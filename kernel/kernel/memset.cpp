import <cstddef>;

extern "C"
void *
memset (unsigned char *dest, int val, size_t len)
{
  if constexpr(true)
  {
    void* original_dest = dest;
    asm volatile(
        "rep stosb\n"
        : "=D"(original_dest), "=c"(len)
        : "0"(original_dest), "1"(len), "a"(val)
        : "memory");
    return dest;

  }
  else
  {
    unsigned char *ptr = dest;
    while (len-- > 0)
      *ptr++ = val;
    return dest;
  }
}
