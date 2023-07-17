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

For full controls list check the `READ THIS!` menu while in-game.

## Dumping game statistics

You can play and dump game statistics to a file using the following command:

`./build/fbdoom-machine -statdump /dev/mtdblock1 -skill <difficulty> -warp <episode> <level>`

Episode can only be 1.

Skill is a number from 0-5:

0. No monsters
1. "I'm Too Young To Die"
2. "Hey, Not Too Rough"
3. "Hurt Me Plenty"
4. "Ultra-Violence"
5. "Nightmare!"

Level is a number from 1-9:

1. E1M1: Hangar
2. E1M2: Nuclear Plant
3. E1M3: Toxin Refinery
4. E1M4: Command Control
5. E1M5: Phobos Lab
6. E1M6: Central Processing
7. E1M7: Computer Station
8. E1M8: Phobos Anomaly
9. E1M9: Military Base


For example:
```
./build/fbdoom-machine -statdump /dev/mtdblock1 -skill 3 -warp 1 2
```

This will start a new game with difficulty 3 at level 2, when the user quits the game
statistics for each level will be saved to machine `/dev/mtdblock1`, and the contents
will be saved to a file in the host named `machine_output.txt`.
The statistics has the following format:

```
===========================================
E1M1
===========================================

Time: 0:21 (par: 0:30)

Player 1 (Green):
    Kills: 0 / 4 (0%)
    Items: 3 / 37 (8%)
    Secrets: 0 / 3 (0%)

===========================================
E1M2
===========================================

Time: 1:53 (par: 1:15)

Player 1 (Green):
    Kills: 10 / 20 (50%)
    Items: 5 / 42 (11%)
    Secrets: 0 / 6 (0%)


```

This file can have multiple entries for the same level,
and can capture up to 32 entries.

## Showing FPS

You can use pass the option `-debug-fps` when running, this will print frame rate statistics every
1 second, for example:

```
./build/fbdoom-machine -debug-fps
...
frame=16 fps=16.65 guest_cpu_clock=97.42MHz host_cpu_usage=61.13%
frame=51 fps=35.00 guest_cpu_clock=138.61MHz host_cpu_usage=8.69%
frame=86 fps=35.00 guest_cpu_clock=138.99MHz host_cpu_usage=8.67%
frame=122 fps=35.02 guest_cpu_clock=140.19MHz host_cpu_usage=8.60%
frame=158 fps=35.00 guest_cpu_clock=139.31MHz host_cpu_usage=8.65%
frame=192 fps=33.32 guest_cpu_clock=136.58MHz host_cpu_usage=61.96%
frame=227 fps=34.76 guest_cpu_clock=134.67MHz host_cpu_usage=64.52%
frame=263 fps=35.03 guest_cpu_clock=139.34MHz host_cpu_usage=26.92%
frame=298 fps=34.96 guest_cpu_clock=138.37MHz host_cpu_usage=24.15%
```

Where:
* `frame`: is the current frame number since the application started, this can also be understood as "frame" time elapsed in the game.
* `fps`: is the current frame rate per second, DOOM is limited to 35 FPS by design, so you should expect values to be around that.
* `guest_cpu_clock`: is the RISC-V guest machine CPU clock, this value is the amount of million instructions per second the machine is capable of outputting, it varies because it depends very much on the instructions being executed.
* `host_cpu_usage`: it's how much of the host CPU core was used to process everything, when this value is near 100 you should expect FPS to drop, a value of 50% means that the host CPU would be capable of doubling the FPS if it was unlimited.

## Auto quit option

In case you want the DOOM game to automatically exit when a level is finished or a player dies,
you can use the `-autoquit` option.

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

Check if you have the following requirements:
- `riscv64-linux-gnu-gcc` toolchain available in your path
- [https://github.com/cartesi/genext2fs](genext2fs) installed and available in your path, must be this custom version
- Docker installed with RISC-V QEMU support.

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
