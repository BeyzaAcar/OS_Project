#include "interrupts.h"

void printf(char* str);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256]; // The interrupt descriptor table (IDT) is a data structure used by the x86 architecture to implement an interrupt vector table. The IDT is used by the processor to determine the correct response to interrupts and exceptions.

void InterruptManager::SetInterruptDescriptorTableEntry( // Set the entry in the interrupt descriptor table
    uint8_t interruptNumber, // The interrupt number
    uint16_t codeSegmentSelectorOffset, // The code segment selector offset
    void (*handler)(), // The interrupt handler function
    uint8_t DescriptorPrivilegeLevel, // The descriptor privilege level
    uint8_t DescriptorType // The descriptor type
)
{
    const uint8_t IDT_DESC_PRESENT = 0x80; // Present
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF; // The address of the function that will handle the interrupt
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF; // The address of the function that will handle the interrupt (continued) (>> is the right shift operator which shifts the bits of the number to the right and fills the leftmost bits with the sign bit in case of a signed integer and with 0 in case of an unsigned integer.)
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset; // The code segment selector from GDT (Global Descriptor Table (GDT) is a data structure used by Intel x86-family processors starting with the 80286 in order to define the characteristics of the various memory areas used during program execution, including the base address, the size, the access privileges like executability and writability, and whether the area can be seen by user-space programs or only by the kernel. The GDT is still present in 64-bit mode, but paged memory translation is typically used instead.)
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) << 5); // Access flags
    interruptDescriptorTable[interruptNumber].reserved = 0; // Reserved by Intel 

}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) // Constructor
{
    uint16_t CodeSegment = gdt->CodeSegmentSelector(); // The code segment selector from GDT (Global Descriptor Table (GDT) is a data structure used by Intel x86-family processors starting with the 80286 in order to define the characteristics of the various memory areas used during program execution, including the base address, the size, the access privileges like executability and writability, and whether the area can be seen by user-space programs or only by the kernel. The GDT is still present in 64-bit mode, but paged memory translation is typically used instead.)
    const uint8_t IDT_INTERRUPT_GATE = 0xE; // Interrupt gate
    for(uint16_t i = 0; i < 256; i++)
    {
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE); // Set the entry in the interrupt descriptor table
    }
    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE); // Timer interrupt
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE); // Keyboard interrupt

    // Set the interrupt descriptor table
    InterruptDescriptorTablePointer idt; // The interrupt descriptor table pointer
    idt.size = 256 * sizeof(GateDescriptor) - 1; // The size of the interrupt descriptor table
    idt.base = (uint32_t)interruptDescriptorTable; // The base address of the interrupt descriptor table
    asm volatile("lidt %0" : : "m" (idt)); // Load the interrupt descriptor table
}
InterruptManager::~InterruptManager() // Destructor
{
}

void InterruptManager::Activate()
{
    asm("sti"); // Enable interrupts (sti : start interrupt)
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    printf("Interrupt: ");
    //printfHex(interruptNumber);
    printf("\n");
    return esp;
}

