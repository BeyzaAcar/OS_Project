#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

    class InterruptManager 
    {
    protected:
        struct GateDescriptor
        {
            uint16_t handlerAddressLowBits; // The address of the function that will handle the interrupt
            uint16_t gdt_codeSegmentSelector; // The code segment selector from GDT (Global Descriptor Table (GDT) is a data structure used by Intel x86-family processors starting with the 80286 in order to define the characteristics of the various memory areas used during program execution, including the base address, the size, the access privileges like executability and writability, and whether the area can be seen by user-space programs or only by the kernel. The GDT is still present in 64-bit mode, but paged memory translation is typically used instead.)
            uint8_t reserved; // Reserved by Intel 
            uint8_t access; // Access flags
            uint16_t handlerAddressHighBits; // The address of the function that will handle the interrupt (continued)
        } __attribute__((packed)); // The packed attribute specifies that a variable or structure field should have the smallest possible alignment—that is, one byte for a variable, and one bit for a field, unless you specify a larger value with the aligned attribute.
    
        static GateDescriptor interruptDescriptorTable[256]; // The interrupt descriptor table (IDT) is a data structure used by the x86 architecture to implement an interrupt vector table. The IDT is used by the processor to determine the correct response to interrupts and exceptions.

        struct InterruptDescriptorTablePointer
        {
            uint16_t size; // The size of the interrupt descriptor table
            uint32_t base; // The base address of the interrupt descriptor table
        } __attribute__((packed)); // The packed attribute specifies that a variable or structure field should have the smallest possible alignment—that is, one byte for a variable, and one bit for a field, unless you specify a larger value with the aligned attribute.

        static void SetInterruptDescriptorTableEntry( // Set the entry in the interrupt descriptor table
            uint8_t interruptNumber, // The interrupt number
            uint16_t codeSegmentSelectorOffset, // The code segment selector offset
            void (*handler)(), // The interrupt handler function
            uint8_t DescriptorPrivilegeLevel, // The descriptor privilege level
            uint8_t DescriptorType // The descriptor type
        );

    public:
        InterruptManager(GlobalDescriptorTable* gdt); // Constructor
        ~InterruptManager(); // Destructor

        void Activate(); // Enable interrupts

        static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp); // esp is the stack pointer for the interrupt handler function which is called by the interrupt stub (stub means a small piece of code that is used to jump to a larger piece of code)
        
        static void IgnoreInterruptRequest(); // Ignore the interrupt request
        static void HandleInterruptRequest0x00(); // Timer interrupt
        static void HandleInterruptRequest0x01(); // Keyboard interrupt
    }; 

#endif