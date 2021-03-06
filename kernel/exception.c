#ifndef EXCEPTION_C
#define EXCEPTION_C

#include "terminal.c"

typedef struct interrupt_frame_struct {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} interrupt_frame;

#define EXCEPTION_HANDLER_NO_ERR(NAME, STR) \
__attribute__((interrupt)) void NAME (interrupt_frame* frame) { \
    outb(0x20, 0x20); \
    bsod(frame, STR, -1); \
}

#define EXCEPTION_HANDLER_ERR(NAME, STR) \
__attribute__((interrupt)) void NAME (interrupt_frame* frame, uint32_t err_code) { \
    outb(0x20, 0x20); \
    bsod(frame, STR, err_code); \
}

void bsod(interrupt_frame* frame, char* err_msg, int32_t err_code) {

    terminal_initialize();
    terminal_writestring("An exception occured: ");
    terminal_writestring(err_msg);
    if(err_code != -1) {
        terminal_writestring(" (error code = 0x");
        terminal_writeint(err_code, 16);
        terminal_writestring(")");
    }
    terminal_writestring("\nHere's what we know:\n");
    terminal_writestring("eip = 0x");
    terminal_writeint(frame->eip, 16);
    terminal_writestring("\ncs = 0x");
    terminal_writeint(frame->cs, 16);
    terminal_writestring("\neflags = 0b");
    terminal_writeint(frame->eflags, 2);
    terminal_writestring("\n(");

    terminal_writestring((0x0001 & frame->eflags) != 0 ? "NC,": "CY,");
    terminal_writestring((0x0004 & frame->eflags) != 0 ? "PE,": "PO,");
    terminal_writestring((0x0010 & frame->eflags) != 0 ? "AC,": "NA,");
    terminal_writestring((0x0040 & frame->eflags) != 0 ? "ZR,": "NZ,");
    terminal_writestring((0x0080 & frame->eflags) != 0 ? "NG,": "PL,");
    terminal_writestring("TF:");
    terminal_writeint((0x0100 & frame->eflags) >> 8, 2);
    terminal_putchar(',');
    terminal_writestring((0x0200 & frame->eflags) != 0 ? "EI,": "DI,");
    terminal_writestring((0x0400 & frame->eflags) != 0 ? "DN,": "UP,");
    terminal_writestring((0x0800 & frame->eflags) != 0 ? "OV,": "NV,");
    terminal_writestring("IOPL:");
    terminal_writeint((0x3000 & frame->eflags) >> 12, 10);
    terminal_putchar(',');
    terminal_writestring("NT:");
    terminal_writeint((0x4000 & frame->eflags) >> 14, 2);
    terminal_putchar(',');
    terminal_writestring("RF:");
    terminal_writeint((0x0001000 & frame->eflags) >> 16, 2);
    terminal_putchar(',');
    terminal_writestring("VM:");
    terminal_writeint((0x0002000 & frame->eflags) >> 17, 2);
    terminal_putchar(',');
    terminal_writestring("AC:");
    terminal_writeint((0x0004000 & frame->eflags) >> 18, 2);
    terminal_putchar(',');
    terminal_writestring("VIF:");
    terminal_writeint((0x0008000 & frame->eflags) >> 19, 2);
    terminal_putchar(',');
    terminal_writestring("VIP:");
    terminal_writeint((0x0010000 & frame->eflags) >> 20, 2);
    terminal_putchar(',');
    terminal_writestring("ID:");
    terminal_writeint((0x0020000 & frame->eflags) >> 21, 2);
    terminal_putchar(')');

    for(;;) {
        asm("hlt");
    }
}

EXCEPTION_HANDLER_NO_ERR(divide_by_zero_handler, "Divide by zero")
EXCEPTION_HANDLER_NO_ERR(debug_handler, "debug")
EXCEPTION_HANDLER_NO_ERR(non_maskable_interrupt_handler, "Non maskable interrupt")
EXCEPTION_HANDLER_NO_ERR(breakpoint_handler, "Breakpoint")
EXCEPTION_HANDLER_NO_ERR(overflow_handler, "Overflow")
EXCEPTION_HANDLER_NO_ERR(bound_range_exceeded_handler, "Bound range exceeded")
EXCEPTION_HANDLER_NO_ERR(invalid_opcode_handler, "Invalid opcode")
EXCEPTION_HANDLER_NO_ERR(device_not_available_handler, "Device not available")
EXCEPTION_HANDLER_ERR(double_fault_handler, "Double fault")
EXCEPTION_HANDLER_NO_ERR(coprocessor_seg_overrun_handler, "Coprocessor seg overrun")
EXCEPTION_HANDLER_ERR(invalid_tss_handler, "Invalid tss")
EXCEPTION_HANDLER_ERR(segment_not_present_handler, "Segment not present")
EXCEPTION_HANDLER_ERR(stack_segment_fault_handler, "Stack segment fault")
EXCEPTION_HANDLER_ERR(general_protection_fault_handler, "General protection fault")
EXCEPTION_HANDLER_ERR(page_fault_handler, "Page fault")
EXCEPTION_HANDLER_NO_ERR(reserved_handler, "Reserved")
EXCEPTION_HANDLER_NO_ERR(x87_fp_exception_handler, "x87 floating point exception")
EXCEPTION_HANDLER_NO_ERR(alignment_check_handler, "Alignment check");
EXCEPTION_HANDLER_NO_ERR(machine_check_handler, "Machine check");
EXCEPTION_HANDLER_NO_ERR(simd_fp_exception_handler, "SIMD floating point exception");
EXCEPTION_HANDLER_NO_ERR(virtualization_exception_handler, "Virtualization exception");
EXCEPTION_HANDLER_NO_ERR(security_exception_handler, "Security exception");

#endif //EXCEPTION_C