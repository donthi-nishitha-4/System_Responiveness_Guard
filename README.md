System Responsiveness Guard
 ~ This is a prototype submission for NETRIK NATIONAL HACK 2026 by Team Illumina
 
Preventing System Slowdowns Caused by Resource-Hungry Processes

*Problem Statement*

In multi-process Linux systems, a single CPU-intensive process can degrade system responsiveness by monopolizing CPU time. The default Linux scheduler ensures fairness but does not explicitly protect interactivity. This project implements a lightweight kernel-assisted mechanism to detect such CPU-hogging processes and dynamically reduce their scheduling priority — without killing them.

*Objective*

Monitor running processes at the kernel level

Detect CPU-intensive (hogging) processes

Dynamically reduce their priority

Restore system responsiveness

Avoid terminating any process

*System Design*

Implemented as a Linux Kernel Module

Uses a kernel timer to periodically scan processes

*Detection based on:*

CPU execution time (sum_exec_runtime)

Normal user processes only (skips kernel threads)

*Control mechanism:*

Adjusts priority using set_user_nice()

Safe and reversible

*Components*

resp_guard.c → Kernel module

Makefile → Build instructions

hog.c → CPU hog test program

* Build Instructions*
Compile kernel module:
make
Compile hog generator:
gcc hog.c -o hog

*Usage (Demo Flow)*
Start CPU hog:
./hog
Load kernel module:
sudo insmod resp_guard.ko
View kernel logs:
sudo dmesg | grep RESP_GUARD

Expected output:

RESP_GUARD: CPU hog detected! PID=1234 NAME=hog
RESP_GUARD: Priority reduced for PID=1234 new nice=10
Verify priority change:
ps -o pid,ni,comm -p 1234
Stop hog:
pkill hog
Remove module:
sudo rmmod resp_guard
 Test Program (hog.c)

Creates an infinite loop to simulate a CPU-hogging process.

*Safety Considerations*

Kernel threads are ignored

No process is killed

Priority change is reversible

Timer interval avoids kernel hogging

*Limitations*

Simple rule-based detection

Does not handle real-time processes

Demonstration-level prototype

Repeated detection possible for multiple hogs

*Hackathon Relevance*

This project demonstrates:

Kernel-level monitoring

Scheduling control

OS-level system design

Practical responsiveness protection

*Authors*

Team - Illumina : Pebbeti Navya Sri | Deepthi Subraveti|  Donthi Nishitha | Patil Shiva Deekshitha
Submission for the Event: Netrik National Hack 2026 - IIITDM Kurnool
