FROM --platform=linux/riscv64 riscv64/ubuntu:22.04
RUN apt-get update
RUN apt-get install -y --no-install-recommends busybox-static
RUN rm -rf /var/lib/apt/lists/* && rm -rf /var/log/*
ADD third-party/machine-emulator-tools-v0.10.0.tar.gz /

# Install Riv library
COPY build/libriv.so /usr/lib/libriv.so

# Install Doom
COPY --chmod=755 build/fbdoom /root/fbdoom
COPY third-party/doom1.wad /root/doom1.wad
