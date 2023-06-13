export module abort;


extern "C" export void abort()
{
    asm volatile("cli");
    asm volatile("hlt");
}
