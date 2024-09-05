# Python DApp RIV Template

This is a template for RIV Cartesi DApps. It uses python3 to execute verify a DOOM gameplay and display
its result as a notice.

This project was created by doing minimal modifications on top of `cartesi create --template=python dapp`
using @cartesi/cli 0.16.1.

## Build

```
# install cartesi cli first
npm i -g @cartesi/cli

# this is only necessary in cartesi rollups v1.x because we need custom kernel
docker build --tag riv/cartesi-sdk:0.9.0 --target cartesi-riv-sdk .

# build
cartesi build

# run
cartesi run
```

## Test

Send a gameplay log advance with:

```
cartesi send generic --input 0x0101060840010000000000000a00000075667268736920672d8f416e1b411003a7c8e6f4ec8e76155b7002c4c83ff2ff97e5a090e72aa0c64058ae046cb06d00ca784e8ab6936e773bb1eda4ed6e80c48682392b317e4bf2ffc6867627c68969a7d35d041209220514db3273920448bddd7652982e777777a73b49179693c49a669a984ef59b21152999ccf7904cb9d39db60da6a2584ee4c8b1e4d04d642357bb0b5974b92945911245d43bae18e3f65f34c7aad43ec974098ab93e2fe4e7f1e7d5b50b3d87c731f1a5a756d4f97109d7f42c5f5b757595615689c829dc5825ad3d9887b4a3b1bb25ad024a35179fd263bcbec67365478c7a7d7c9f3296736e44dde5f1fbfb08d2e95bb1bd835d07cfb997f5d4d0f128eee3661c59759e8746e71ad3e2d6ce477def1f65f18a9c31ebc8a99fcb33378fba3d1e637cc107705cd178bcb6596aa6aaa4bb60ad4375f318bfb874dd034671fb1f
```

The input above is the hexdump of `gameplay-example.rivtape`.
After sending this you  will be able to see the game JSON score output as a notice in the explorer URL.

### Test other gameplays

You can record and download other gameplay logs by playing DOOM at https://emulator.rives.io/ .
You could also download different cartridges there and change the game of this example.
