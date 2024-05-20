#ifndef __GDT_H
#define __GDT_H

#include "types.h"

class GlobalDescriptorTable // The Global Descriptor Table (GDT) is a data structure used by Intel x86-family processors starting with the 80286 in order to define the characteristics of the various memory areas used during program execution, including the base address, the size, the access privileges like executability and writability, and whether the area can be seen by user-space programs or only by the kernel. The GDT is still present in 64-bit mode, but paged memory translation is typically used instead.
{
    
    public:
        class SegmentDescriptor // The segment descriptor
        {
            private:
                uint16_t limit_lo; // The lower 16 bits of the limit
                uint16_t base_lo; // The lower 16 bits of the base
                uint8_t base_hi; // The higher 8 bits of the base
                uint8_t type; // The type of segment
                uint8_t flags_limit_hi; // The flags and the higher 4 bits of the limit
                uint8_t base_vhi; // The higher 8 bits of the base
            public:
                SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type); // Constructor
                uint32_t Base(); // Get the base address
                uint32_t Limit(); // Get the limit
        } __attribute__((packed)); // The packed attribute specifies that a variable or structure field should have the smallest possible alignment—that is, one byte for a variable, and one bit for a field, unless you specify a larger value with the aligned attribute.

    private:
        SegmentDescriptor nullSegmentSelector; // The null segment selector
        SegmentDescriptor unusedSegmentSelector; // The unused segment selector
        SegmentDescriptor codeSegmentSelector; // The code segment selector
        SegmentDescriptor dataSegmentSelector; // The data segment selector

    public:
        GlobalDescriptorTable(); // Constructor
        ~GlobalDescriptorTable(); // Destructor

        uint16_t CodeSegmentSelector(); // Get the code segment selector
        uint16_t DataSegmentSelector(); // Get the data segment selector
};

#endif