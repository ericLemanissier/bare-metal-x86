namespace std{
    void terminate()
    {
        asm volatile("cli");
        asm volatile("hlt");
    }
}
