import <cstddef>;

extern "C" void *
memmove (char *dest, const char *src, size_t len)
{
  char *d = dest;
  const char *s = src;
  if (d < s)
    while (len--)
      *d++ = *s++;
  else
    {
      const char *lasts = s + (len-1);
      char *lastd = d + (len-1);
      while (len--)
        *lastd-- = *lasts--;
    }
  return dest;
}
