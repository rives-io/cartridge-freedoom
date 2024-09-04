# validate if we are using the correct emulator version, otherwise things will not work
if ! zcat /proc/config.gz | grep -q CONFIG_SQUASHFS; then
  echo "Linux kernel is not compatible, maybe you should upgrade the cartesi machine emualtor?"
  exit 1
fi

# validate if Linux boot was properly done, otherwise things will not work
if [ ! -f /sys/fs/cgroup/cgroup.subtree_control ]; then
  echo "Linux kernel has no cgroups active, maybe you forgot to run /etc/cartesi-init.d/riv-init on boot?"
  exit 1
fi

# validate if we are root, otherwise things will not work
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root"
  exit 1
fi

# path to the file containing the cartridge
export RIV_CARTRIDGE=/dev/pmem1

# path to the file containing the gameplay log (the input)
export RIV_REPLAYLOG=/dev/pmem2

# path to the file that we will save the score json (the output)
export RIV_OUTCARD=/run/outcard

# uncommenting this will make DOOM now draw any pixel,
# this will make computation much lightweight and faster,
# but you won't able to capture screenshots of the game framebuffer
#export RIV_NO_YIELD=y

# compute the gameplay
riv-run

# copy the score to the output drive
cat /run/outcard > /dev/pmem3
