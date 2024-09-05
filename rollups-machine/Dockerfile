# syntax=docker.io/docker/dockerfile:1

# this custom kernel is only necessary in rollups v1.x
ARG CARTESI_SDK_VERSION=0.9.0
FROM cartesi/sdk:${CARTESI_SDK_VERSION} AS cartesi-riv-sdk
ARG KERNEL_VERSION=0.19.1-riv1
RUN curl -s -L https://github.com/rives-io/kernel/releases/download/v${KERNEL_VERSION}/linux-6.5.9-ctsi-1-v${KERNEL_VERSION}.bin \
    -o /usr/share/cartesi-machine/images/linux.bin

FROM --platform=linux/riscv64 cartesi/python:3.10-slim-jammy

ARG MACHINE_EMULATOR_TOOLS_VERSION=0.14.1
ADD https://github.com/cartesi/machine-emulator-tools/releases/download/v${MACHINE_EMULATOR_TOOLS_VERSION}/machine-emulator-tools-v${MACHINE_EMULATOR_TOOLS_VERSION}.deb /
RUN dpkg -i /machine-emulator-tools-v${MACHINE_EMULATOR_TOOLS_VERSION}.deb \
  && rm /machine-emulator-tools-v${MACHINE_EMULATOR_TOOLS_VERSION}.deb

LABEL io.cartesi.rollups.sdk_version=0.9.0
LABEL io.cartesi.rollups.sdk_name=riv/cartesi-sdk
LABEL io.cartesi.rollups.ram_size=128Mi

ARG DEBIAN_FRONTEND=noninteractive
RUN <<EOF
set -e
apt-get update
apt-get install -y --no-install-recommends \
  busybox-static=1:1.30.1-7ubuntu3
rm -rf /var/lib/apt/lists/* /var/log/* /var/cache/*
EOF

# this creates dapp user actually with root privilege (need to call riv-run)
RUN useradd --create-home --non-unique --uid 0 --gid 0 dapp

ENV PATH="/opt/cartesi/bin:${PATH}"

WORKDIR /opt/cartesi/dapp
COPY ./requirements.txt .

RUN <<EOF
set -e
pip install -r requirements.txt --no-cache
find /usr/local/lib -type d -name __pycache__ -exec rm -r {} +
EOF

# install DOOM
ADD --chmod=644 https://github.com/rives-io/cartridge-freedoom/releases/download/v0.4.0/freedoom.sqfs /cartridges/freedoom.sqfs

# Install RIV OS
ARG RIV_VERSION=0.3-rc16
ADD --chmod=644 https://github.com/rives-io/riv/releases/download/v${RIV_VERSION}/rivos.ext2 /rivos.ext2
ADD --chmod=644 https://raw.githubusercontent.com/rives-io/riv/v${RIV_VERSION}/rivos/skel/etc/sysctl.conf /etc/sysctl.conf
ADD --chmod=755 https://raw.githubusercontent.com/rives-io/riv/v${RIV_VERSION}/rivos/skel/usr/sbin/cartesi-init /usr/sbin/cartesi-init
ADD --chmod=755 https://raw.githubusercontent.com/rives-io/riv/v${RIV_VERSION}/rivos/skel/etc/cartesi-init.d/riv-init /etc/cartesi-init.d/riv-init
RUN <<EOF
set -e
mkdir -p /rivos
echo "mount -o ro,noatime,nosuid -t ext2 /rivos.ext2 /rivos" >> /etc/cartesi-init.d/0-riv-mount
echo "mount --bind /cartridges /rivos/cartridges" >> /etc/cartesi-init.d/0-riv-mount
echo "mount --bind /tapes /rivos/tapes" >> /etc/cartesi-init.d/0-riv-mount
chmod 755 /etc/cartesi-init.d/0-riv-mount
EOF

COPY ./dapp.py .

ENV ROLLUP_HTTP_SERVER_URL="http://127.0.0.1:5004"

ENTRYPOINT ["rollup-init"]
CMD ["python3", "dapp.py"]
