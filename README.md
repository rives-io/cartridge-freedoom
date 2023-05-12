# Cartesi DOOM Example

This is an example of running DOOM inside the Cartesi in real-time.

You can see a video of this in action in [this tweet](https://twitter.com/edubart/status/1621512794339446786)

## How to run in Ubuntu

You need the following development packages to compile:

```
apt-get install build-essential libx11-dev libxi-dev libxcursor-dev libgl1-mesa-dev git wget
```

You also need the Cartesi Machine v0.13.0, you can download it prebuilt for Ubuntu with the following commands:

```
wget https://github.com/cartesi/machine-emulator/releases/download/v0.13.0/machine-emulator-Linux-v0.13.0.tar.gz
mkdir -p /opt/cartesi
tar -C /opt/cartesi -xf machine-emulator-Linux-v0.13.0.tar.gz
```

After the above steps are completed, you should be able to clone, compile and run the demo in Ubuntu using the following commands:

```
git clone https://github.com/edubart/cartesi-doom-example.git
cd cartesi-doom-example
make download-rootfs
make test
```

The DOOM game should pop-up in a few seconds for you to play.

*This was tested in Ubuntu 22.04 LTS*

## How to run in any Linux

First make sure you have the following requirements:
 - Your host system is using Linux, this is the only supported platform at this moment.
 - You have [machine-emulator v0.13.0](https://github.com/cartesi/machine-emulator/releases/tag/v0.13.0) installed, and its library should be available in `/opt/cartesi/lib/libcartesi.so`. Make sure you are using this exact version, there are hard coded constants to work with exactly this version of the emulator.
 - You have the `make`, `gcc`, git` and `wget` installed.

Once you have all the above requirements, you should be able to clone, compile and run the demo using the following commands:

```
git clone https://github.com/edubart/cartesi-doom-example.git
cd cartesi-doom-example
make download-rootfs
make test
```

The DOOM game should pop-up in a few seconds for you to play.

## Game Controls

- Ctrl              - Fire
- Space             - Use / Open
- Left/Right        - Turn
- Up/Down           - Walk
- Alt + Left/Right  - Walk laterally
- Tab               - Game Map
- Shift             - Run
- Escape            - Menu
- Enter             - Menu select

## How all this works

It runs DOOM normally, video and audio are written to a virtual memory range that is known to be fixed
in a physical memory range visible from outside. At every frame, the machine performs a yield automatic to expose
audio and video to the host and gather keyboard inputs from the host.

In the host the following libraries are used:
 - [Sokol](https://github.com/floooh/sokol) and [Sokol GP](https://github.com/edubart/sokol_gp) - for window, inputs and graphics
 - [Miniaudio](https://github.com/mackron/miniaudio) - for audio
 - [Cartesi Machine](https://github.com/cartesi/machine-emulator) - For running a RISC-V virtual machine with DOOM

## How to build rootfs

In case you want to patch sources inside `fbdoom`, you will need to rebuild it and its rootfs.
Check if you have the `riscv64-cartesi-linux-gnu-gcc` toolchain available in your path first,
and docker installed with RISC-V QEMU support.
Then run `make rootfs.ext2`.

## Editing source code

First make sure you have Nelua installed, after editing the sources code,
to regenerate the C code from Nelua sources type `make gen-c`,
then you can use `make test` to test it.

## Structure

- `build`: Temporary folder used for building.
- `fbdoom`: [fbDOOM sources](https://github.com/maximevince/fbDOOM) patched to use RIV for Audio/Video/Input.
- `fbdoom-machine`: Source code of the Cartesi Machine with Audio/Video/Input support using RIV.
- `images`: folder with pre compiled images
    - `fbdoom_rootfs.ext2`: the virtual machine root filesystem
    - `linux-hugetlbfs-5.15.63-ctsi-1.bin`: the virtual machine kernel, patched with HUGETLBFS support
    - `rom-v0.15.0.bin`: the virtual machine rom
- `rivlib`: minimal library to communicate audio/video/input between the host and virtual machine
- `scripts`: miscellaneous scripts
- `third-party`: third party binaries used when building `fbdoom_rootfs.ext2`
- `Dockerfile`: Docker script for building `fbdoom_rootfs.ext2`

## FAQ

### What is the RIV library in the sources?
It's a pseudo device used to communicate audio/video/input directly between the host machine and the virtual RISC-V machine.
Its name has no special meaning, I may reuse this name in a future project.

### Why does the code use the Nelua programming language?
Because I can, I am author of the [Nelua programming language](https://github.com/edubart/nelua-lang).
But you don't need to install Nelua to compile, because Nelua compiles to C,
and the generated C files are already provided in this source tree,
you just need a GCC compiler.
You shouldn't have difficulty understanding its code, the sources are readable.

### Why does this use a custom Linux kernel?
Because the code depends on [HUGETLBFS](https://www.kernel.org/doc/Documentation/vm/hugetlbpage.txt) support.
Huge pages are used to guarantee a contiguous fixed physical memory address that can be write/read from outside
the machine, it also bypasses the kernel's page cache.
It's used like a pseudo device, to map physical memory memory to user space virtual memory,
it's a dirty trick to exchange bytes between the guest/host without overhead.

### Why does the code use the Cartesi Machine as a library?
To inject keyboard input and spit-out video and audio output to the host machine.
The low level Cartesi Machine C API is used to accomplish this.

### Could this be used on-chain?
Yes, the RIV library could be extended to work accepting on-chain inputs.
This same project could be used to record a log of keyboard inputs in the user host machine,
then this same project could be used to replay keyboard inputs sent on-chain,
and run DOOM without a display or audio, just to capture frames or validate gameplays.

### What platforms do this work?
Currently this can only be played in host systems using Linux.
This could be run on Windows or even WebAssembly in the future
when the Cartesi Machine gets support for those platforms.

### Is this finished?
No, there is no guarantee in the quality of the code, this was made as a quick dirty hack.
I am opening the source of this without polishing or without a real on-chain example,
to inspire people and due to requests.
This code is also a proof that the cartesi machine can indeed run DOOM in real-time.

### Other question?
Ping me in Cartesi Discord server or create an issue.

## License
?? - use at your own risk

## Author

Eduardo Bart - edub4rt@gmail.com
