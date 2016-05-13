---
layout: about
---

As you might have read from the description, it's an operating system. It is right from the boot sector code to the kernel, that means it practically has no dependencies (at least till now). It has a few drivers (clock, VGA, and keyboard) and very soon I am planning to add a memory manager.

# Why?
Well, I have written about the story of why this ever came into existence and why I decided to publish it (considering that every Computer Science student has to develop one) on [my blog](http://blog.asutoshpalai.in/2016/05/my-journey-in-land-of-operating-systems.html). As a short version, I would I like to say that I was just kinda attracted towards it and too a bit further than required by my course. Then I thought that someone else might want to relive the journey so I want to document my resources in developing this.

# A bit of road-map
I don't plan on it much, but I just love to keep hacking on this when I have time and energy. My present plans for this is to first give it a user space. It still a bit far away considering I don't have a memory manager and process manager, but I didn't have high hopes for it right from the start. 

# Features it currently has
1. A boot loader (it's a two staged one)
2. Kernel is in C and is in Protected Mode (obviously)
3. The VGA driver has some basic functionalities like the terminal scroll and stuff.
4. A bit organized folder structure and Makefile to make life easier
5. GDT and ISR (+ IRQ) are setup up.
6. A pretty basic but decent keyboard driver.
7. A dummy shell so that we can give at least some command after bootup.

# Disclaimer
I am not an OS expert, I am still exploring such low-level stuff myself. I developed this as an intersecting engagement and plan to continue on it. I just follow available guides, learn stuff and then try to code it. 

# Contribute or Contact
I would love to see contributions on it and they are very welcome.
If you want to reach me, drop me a mail at [asupalai@gmail.com](mailto:asupalai@gmail.com). I would be happy to hear from you.