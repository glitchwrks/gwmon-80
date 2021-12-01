GWMON-80
========

GWMON-80 is intended to be a simple ROM-type system monitor for systems utilizing processors that are binary-compatible with the 8080, including (but not limited to) 8085 and Z80 systems. It is written in a modular format so that it can be extended for use with specific system hardware with ease. It is being developed and released under the GNU GPLv3 as open source software (see LICENSE and/or GPL-3.0 in project root for more information).

Components
----------

GWMON-80 is composed of the following components:

    * Vectors
    * Core monitor, either SM (Small Monitor) or XM (eXtended Monitor)
    * Command set(s) and handlers
    * I/O modules
    * Customizations

### Vectors

Vectors provide consistent entry points to GWMON-80. Starting with GWMON-80 0.9, vectors are provided at the beginning of the monitor in the form of a jump table. Standard vectors are:

    * CSTART at ORG + 0: cold start routine
    * WSTART at ORG + 3: warm start routine
    * COUT at ORG + 6: output a character to console
    * CIN at ORG + 9: wait for and input a character from console

Older versions of GWMON-80 provide only `CSTART` and `WSTART`.

### Core Monitor

There are two options available for the core monitor: SM (Small Monitor) or XM (eXtended Monitor). Currently, only SM is recommended as XM is still under development. SM has the advantage of being simpler and much smaller: most customizations using SM are 512 bytes or less.

### Command Sets and Handlers

Command sets provide data structures that define commands. The SM and XM standard command sets (`scmdstd.inc` and `xcmdstd.inc`, respectively) also provide the default command handlers for those core monitors. Command sets are chainable such that additional commands can be added into a given customization. Command sets are terminated by a "NULL command," which is monitor-specific and should be included after all other command sets.

Handlers provide actual implementation of commands. The core command handlers are defined with their command sets, but additional handlers must be defined separately from their command sets to allow command set chaining.

### I/O Modules

I/O modules contain the routines necessary to initialize the console I/O device, receive characters from it, and transmit characters to it. They are generic implementations for a given device; for example, `6850acia.inc` provides the routines for talking to any system using an I/O mapped Motorola 6850 ACIA.

### Customizations

Customizations tie together vectors, a core monitor, I/O module, and one or more command sets into a machine-specific implementation of GWMON-80. See `smmits1.asm` for an example of a basic customization of the SM for the MITS 88-2SIO with the standard SM command set.

Building GWMON-80
-----------------

If a customization for your system already exists, GWMON-80 can be built using the included `Makefile`:

    make smmits1

...or by assembling the source directly from the command line. The resulting Intel HEX file can be LOADed as a CP/M program, burned into ROM, etc.

SM Command Syntax
-----------------

The Small Monitor (SM) command syntax is as follows:

    D XXXX YYYY     Dump memory from XXXX to YYYY
    E XXXX          Edit memory starting at XXXX (CTRL+C to end)
    G XXXX          GO starting at address XXXX
    I XX            Input from I/O port XX and display as hex
    O XX YY         Output to I/O port XX byte YY
    L               Load an Intel HEX file into memory

`CTRL+C` may be pressed at any text entry point to cancel the current operation. Hexadecimal inputs are validated, entering a non-hex character will cancel the current operation with an `ERORR` message.

The SM command processor automatically inserts the spaces after each element. So, to dump memory from 0x0000 to 0x000F you'd type

    d0000000f

...and you'd get

    >d 0000 000F
    0000 : xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx
    
    >

...where the xx fields are the hex representation of the bytes at those addresses.

No returns or spaces are typed in the commands. This is very similar to the NorthStar ROM monitor, most likely because it's about the simplest way to implement. Input is auto-downcased, so you can type entries in either (or even mixed) case.

### Edit Memory

The `E` command prompts for a starting address, displays the contents of the memory location, and allows input of a replacement value. Pressing `RETURN` will leave the current value unchanged and move to the next memory address. Pressinc `CTRL+C` at any time terminates the `E` command.

### GO Command

`G` prompts for an address and transfers control to that address. The GWMON-80 warm start address is placed on the stack prior to control transfer, so routines may return to GWMON-80 with a `RET` as long as the stack has been preserved.

### Input and Output

The `I` and `O` commands allow reading and writing I/O ports. Since the Intel 8080 only includes `IN` and `OUT` instructions with immediate addressing, this is accomplished through dynamic routines located just above the stack.

### Intel HEX Loader

The Intel HEX loader expects 16-bit addresses. It behaves as an Intel loader should, allowing empty blocks in the middle to be skipped. It will accept either UNIX-style LF endings or DOS/Windows CR/LF endings. The loader will accept an `EOF` (End of File, `0x01`) record or a data record (`0x00`) with zero length as the terminating condition.

After invoking the loader, paste your Intel HEX file into the terminal or do an ASCII upload (depending on your terminal program).

Writing I/O Modules
-------------------

I/O modules need to implement three named subroutines:

* `IOSET` prepares the console device for use
* `CINNE` inputs a char from the console, don't echo
* `COUT` outputs a char to the console

IOSET should initialize console device, if the devices are not already initialized. Any system-specific setup should be implemented in the customization file, not the I/O module.

`CINNE` and `COUT` are character I/O routines for your console device (`CIN`, input with echo, is implemented elsewhere). They should not modify any registers other than the A register, so push everything else to the stack and pop it off after your routine. Both of these subroutines should terminate in a RET instruction. It's usually good practice to have CIN call CINNE.

Writing Customizations
----------------------

In addition to bringing together the components of GWMON-80, a customization should:

    * Set the required I/O module parameters
    * Set the stack starting address
    * Set the ORG for GWMON-80

See `smmits1.asm` for a simple example of how to piece together a customization for a simple system.

Tools
-----

The `tools/` directory contains utilties for working with GWMON-80:

* `gwmon2bin.rb` is a Ruby script for converting GWMON-80 `D` command dumps to binary files
* `gwmon2bin_sample.txt` is a sample `D` command dump for use with `gwmon2bin.rb`

Contributing
------------

Contributions can be made to any part of this code; however, we're especially encouraging people to contribute their I/O modules and system customizations. A wide variety of I/O modules make this monitor useful to more people without having to write their own modules.

Any contributed code should assemble with our fork of the [A85 assembler](https://github.com/glitchwrks/a85). Intel mnemonics are a requirement. If you wish to optimize the core monitor for another architecture such as Z80, please fork the project.
