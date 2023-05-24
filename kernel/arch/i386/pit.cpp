#include "kernel/pit.h"

#include "kernel/ll.h"

#define PIT_CHANNEL0_DATA_PORT 0x40
#define PIT_COMMAND_PORT 0x43
#define PIT_FREQUENCY 1193182    // PIT input frequency in Hz

// Function to initialize the PIT
void init_pit(void)
{
    // Set the PIT to generate periodic interrupts
    uint16_t divisor = PIT_FREQUENCY / 1000;    // Convert to milliseconds (adjust as needed)


    // Send the command byte to the PIT command port
    outb(PIT_COMMAND_PORT, 0x34);

    // Send the divisor to the PIT channel 0 data port
    outb(PIT_CHANNEL0_DATA_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL0_DATA_PORT, (divisor >> 8) & 0xFF);
}
