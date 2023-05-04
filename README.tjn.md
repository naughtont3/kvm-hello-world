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

This assumes you are using the `kvm-handle-rdtsc` branch in the DVAST
kernel repo (or my fork).

Setup
-----
 - Build KVM from the `kvm-handle-rdtsc` branch

      ```
        git clone -b kvm-handle-rdtsc <DVAST_KERNEL_REPO_URL_HERE>
        cd ubuntu-focal/
         # enable DVAST feature during 'make menuconfig'
         # make menuconfig
        make modules
      ```

 - Then reload kvm...

      ```
        sudo rmmod kvm_intel
        sudo rmmod kvm
        sleep 1
        sudo insmod arch/x86/kvm/kvm.ko
        sudo insmod arch/x86/kvm/kvm-intel.ko
      ```

Usage
-----
 - After build/load of KVM
 - Run driver/guest using `make`
   (this builds and automatically runs `./kvm-hello-world -l`)

    ```
        make
    ```

NOTE
----
 - We are setting the `fake_tsc_val` in the DRIVER (kvm-hello-world.c)
   and each 'rdtsc' issued from the VM (guest.c) just gets +1 this start
   value.

