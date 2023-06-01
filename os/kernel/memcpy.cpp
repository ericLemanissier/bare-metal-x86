import <cstddef>;


extern "C" void * memcpy (char *dest, const char *src, size_t len)
{
  if constexpr(true)
  {
    void* original_dest = dest;
    asm volatile(
        "rep movsb"
        : "+D"(dest), "+S"(src), "+c"(len)::"memory");
    return original_dest;

  }
  else
  {
    char *d = dest;
    const char *s = src;
    while (len--)
      *d++ = *s++;
    return dest;
  }
}
