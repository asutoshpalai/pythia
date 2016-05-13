---
layout: post
title: The boot sector
---

# First post
This is the first of the posts describing the references I used and decisions I made while developing [Pythia OS](https://github.com/asutoshpalai/pythia). In these posts I shall try to restrict myself to the sources and my decisions and not dwell on the technical details, there are enough sites out there to give that (some being, [OSDev](http://wiki.osdev.org/) and [OSDever.net](http://www.osdever.net/)).

In the first post, I shall be talking about the boot sector code.

Most hobbyist OS use some external boot sector (or Boot Loader) such as GRUB and they are right in doing so. It is a barren environment and heavily dependent on the system. I had no choice in that matter since I was assigned to write one and because I wrote this, my old interest in OS development was renewed. 

# Boot Sector
The boot sector technically refers to the first 512 bytes of any disk that ends with the two magic bytes 0x55 0xaa. The computer on startup searches for any of the disks that has this property and when it finds one, it loads that 512 bytes onto memory (at address 0x7c00) and starts executing from the first byte. This part of the OS was what was assigned as my project.

# Gold Mine
So, when I started to search materials to write one, I found a good number of them. But most of them were unfit for my purpose (which was just to host a protected mode kernel in C). I had one night to write it and didn't have much time to play with things around. So, I kept searching instead of writing using what was available. Then I stuck the [gold mine to write a boot sector](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf). This book had exactly what I
wanted. Yes, you might question that I had such a short time and I sat down to read this log PDF. Well, as much as I wanted to complete my task, I wanted to learn all the concepts behind what I write and be able to explain each nitty-gritty of my code. So this was a gold mine for me.

This PDF, although represents an incomplete work by the author, it damn well explains what it does. It explains all the options we have, the environment we are in and why each thing is done in a certain way.
While reading it, I had sufficient time to puts my hands around and bit and poke a bit here and there. I wrote most of the routines myself after reading it and some I just copied right from the book (the code was pretty basic and once you read them, writing them for yourself was almost a rewrite of it).

# Version Control
As this stage, I also started version controlling my code. This gave me pretty much room to play around and be sure that I can always go back. I used git for VCS. I am a strong advocate of version controlling your projects, however small. This gives you freedom with a guarantee. You can break the code to find some optimal solution and if you fail you know where to go. This really helps in learning.

# Makefile
Yup, for those, who like me, are new to large C/C++ projects, don't shy away from writing your Makefile. That's a gem out there. It takes away almost all the commands in your debugging and developing cycle. By that, it saves a lot of time and frustrations, trust me. Writing the Makefile is thousand times worth the effort. Once written, it will be always there as your faithful server and it's easy to extend.

# Conclusion
So that's is how I wrote my boot sector. [The PDF](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf), also linked above, has all that you need to write one with proper explanations. I also came along [another source](http://wiki.osdev.org/Category:Babystep), a bit less explanatory but none the less good. But I found it much later when I was finding resources for the kernel.

Hopes this guides you in writing your own boot sector.
