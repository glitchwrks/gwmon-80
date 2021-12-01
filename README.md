GWMON-80
========

GWMON-80 is intended to be a simple ROM-type system monitor for systems utilizing processors that are binary-compatible with the 8080, including (but not limited to) 8085 and Z80 systems. It is written in a modular format so that it can be extended for use with specific system hardware with ease. It is being developed and released under the GNU GPLv3 as open source software (see LICENSE and/or GPL-3.0 in project root for more information).

Contributing
------------

Contributions can be made to any part of this code; however, we're especially encouraging people to contribute their I/O modules and system customizations. A wide variety of I/O modules make this monitor useful to more people without having to write their own modules.

### I/O Modules

I/O Modules should assemble under our fork of the [A85 assembler](https://github.com/glitchwrks/a85). Intel mnemonics are thus a requirement. If an I/O module is particular to a very restricted set of hardware (i.e. a system that cannot have more than one type/revision of processor), I/O modules may include opcodes from instruction sets that extend that of the 8080. Keeping to the 8080 Instruction Set Architecture is preferred.

### Core Monitor

The core monitor code should assemble under our fork of the [A85 assembler](https://github.com/glitchwrks/a85). Intel mnemonics are a requirement. The core monitor code *MUST* be 8080 compatible; therefore, no opcodes from instruction sets that extend that of the 8080 may be used. Contributions using non-8080 opcodes will be rejected. If you wish to optimize the core monitor code for your specific architecture, please fork the project.

Installation
------------

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

The SM command processor automatically inserts the spaces after each element. So, to dump memory from 0x0000 to 0x000F you'd type

    d0000000f

...and you'd get

    >d 0000 000F
    0000 : xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx
    
    >

...where the xx fields are the hex representation of the bytes at those addresses.

No returns or spaces are typed in the commands. This is very similar to the NorthStar ROM monitor, most likely because it's about the simplest way to implement. Input is auto-downcased, so you can type entries in either (or even mixed) case.

The Intel HEX loader expects 16-bit addresses. It behaves as an Intel loader should, allowing empty blocks in the middle to be skipped. It will accept either UNIX-style LF endings or DOS/Windows CR/LF endings. The loader will accept an `EOF` (End of File, `0x01`) record or a data record (`0x00`) with zero length as the terminating condition.

After invoking the loader, paste your Intel HEX file into the terminal or do an ASCII upload (depending on your terminal program).

Writing I/O Modules
-------------------

I/O modules need to implement a few named subroutines:

* `IOSET` prepares the console device for use
* `CINNE` inputs a char from the console, don't echo
* `COUT` outputs a char to the console

IOSET should initialize console device, if the devices are not already initialized. Any system-specific setup should be implemented in the customization file, not the I/O module.

`CINNE` and `COUT` are character I/O routines for your console device (`CIN`, input with echo, is implemented elsewhere). They should not modify any registers other than the A register, so push everything else to the stack and pop it off after your routine. Both of these subroutines should terminate in a RET instruction. It's usually good practice to have CIN call CINNE.

Tools
-----

The `tools/` directory contains utilties for working with GWMON-80:

* `gwmon2bin.rb` is a Ruby script for converting GWMON-80 `D` command dumps to binary files
* `gwmon2bin_sample.txt` is a sample `D` command dump for use with `gwmon2bin.rb`
