import <cstddef>;

extern "C"
void* memcpy(void* dest_ptr, void const* src_ptr, size_t n);

extern "C" void *
memmove (char *dest, const char *src, size_t len)
{
  char *d = dest;
  const char *s = src;
  if (d < s || d >= s + len)
    return memcpy(dest, src, len);

  const char *lasts = s + (len-1);
  char *lastd = d + (len-1);
  while (len--)
    *lastd-- = *lasts--;
  return dest;
}
