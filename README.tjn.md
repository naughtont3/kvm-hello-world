README TJN
----------

Shift to using this test driver for simple driver to test the RDTSC hooking.
The changes will **only** work for the LONG MODE guest64 case as we only
care about dealing with rdtsc for a modern 64-bit system.

In this branch we eliminated the build for short/real modes setup things
to just build/test the case we care about.

The `guest.c` was modifed to call rdtsc ASM and then print the resulting
value.

The `kvm-hello-world.c` was modified to handle the `KVM_EXIT_RDTSC` exit
reason in the main event loop.  It simply continues for now.  It should
instead set the `kvm_run` value and then continue.



NOTE
----
 - Currently the RDTSC is not getting fully set back up to calling
   processes, which is what we are setting this code up to test.
   So for now it will just show a 0 for the value of TSC after a read.
