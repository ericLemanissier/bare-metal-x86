export module abort;


extern "C" export void abort()
{
    asm volatile("cli");
    while(true)
        asm volatile("hlt");
}
