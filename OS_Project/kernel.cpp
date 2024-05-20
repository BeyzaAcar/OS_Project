#include "types.h"
#include "gdt.h"
#include "interrupts.h"

void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000; // The memory location of the VGA text buffer
    static uint8_t x = 0, y = 0; // The current cursor position
    for (int i = 0; str[i] != '\0'; ++i) // Loop through the string
    {
        switch (str[i]) // Switch on the current character
        {
            case '\n': // Newline
                x = 0; // Move to the start of the next line
                y++; // Move to the next line
                break;
            default: // Any other character
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i]; // Write the character to the buffer
                x++; // Move to the next column
                break;
        }
        if (x >= 80) // If we're at the end of the line
        {
            x = 0; // Move to the start of the next line
            y++; // Move to the next line
        }
        if (y >= 25) // If we're at the end of the screen
        {
            for (y = 0; y < 25; y++) // Loop through the lines
                for (x = 0; x < 80; x++) // Loop through the columns
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' '; // Clear the character
            x = 0; // Move to the start of the screen
            y = 0; // Move to the start of the screen
        }
    }
}

typedef void (*constructor)(); // Define a constructor type
extern "C" constructor start_ctors; // Declare the start of the constructors
extern "C" constructor end_ctors; // Declare the end of the constructors
extern "C" void callConstructors() // Call the constructors
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++) // Loop through the constructors
        (*i)(); // Call the constructor
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) // The kernel's entry point
{
    printf("Hello, Human!\n");
    printf("Hello, Human!");

    GlobalDescriptorTable gdt; // Create a new global descriptor table
    InterruptManager interrupts(&gdt); // Create a new interrupt manager



    interrupts.Activate(); // Enable interrupts

    while(1);
}