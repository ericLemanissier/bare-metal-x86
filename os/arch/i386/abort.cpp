extern "C" void abort()
{
    asm volatile("cli");
    asm volatile("hlt");
}
