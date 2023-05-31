import <cstddef>;


extern "C" void * memcpy (char *dest, const char *src, size_t len)
{
  char *d = dest;
  const char *s = src;
  while (len--)
    *d++ = *s++;
  return dest;
}
