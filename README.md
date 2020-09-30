# Amiga - basic screen setup

In this repo, two directories to showcase two different ways to open a basic screen on Amiga.

**In the hardware directory**, there is a very old (assembly) source I wrote when I was learning Amiga hardware, that was my first working setup / system restore code, and copperlist setup.

In 2020 I added the C version. It is not *too* fat : executable takes 1868 bytes (368 bytes for the assembly version).

Tested on 68000/ECS (UAE and REAL machine) and 68040/AGA (UAE).

**In the graphicslib directory**, there is a C source originally published in a French Amiga magazine, which I did not modify a lot, only a few things so it can be cross-compiled with my [vbcc setup](https://github.com/nicolasbauw/Amiga-cc).

To build both C versions, just type **make** (make sure you have installed the cross-compiler and set environment variables).