# FREEDOOM Compute Machine

This folder will generate a special Cartesi Machine image
that allows you to compute a DOOM score for a given *tape*.

The machine works as follows, it has 4 flash drives:

1. The *rootfs* flash drive - Contains RIV OS, a bare minimal linux based on Alpine Linux able to run the game.
2. The *cartridge* flash drive - Contains a compressed DOOM cartridge, compiled to RISC-V using RIV libraries for graphics/audio/input.
3. The *tape* flash drive - Receives a log of keystrokes pressed while playing the game.
4. The *outcard* flash drive - Outputs the outcome of the given DOOM tape, in case of DOOM is a score in JSON format.

## Compiling the machine

First make sure you have latest `cartesi-machine`, `make` and `wget` installed in your system, then:

```sh
make
```

This will compile a snapshot of the machine in the `image` folder.

## Generating scores

First record a *tape* with `rivemu`,
if you don't know how to this read please read the main README of this repository.

In case you don't want to record one yourself,
the tape `gameplay-example.rivtape` is provided
in this directory as an example.

Now to compute the score for a given tape,
execute the following make command:

```sh
make compute TAPE=gameplay-example.rivtape
```

This will load the machine snapshot while replacing
the *tape* flash drive with a file as an input,
and *outcard* flash drives to output to a file.
A JSON with the score will be printed at the very end,
and also saved to `gameplay-example.rivtape.json`.

The JSON score should be similar to:

```
==== OUTCARD ====
JSON{
    "level_stats": [{
        "name": "E1M1",
        "pctkills": 34.48,
        "numkills": 10,
        "pctitems": 14.29,
        "numitems": 7,
        "pctsecrets": 0.00,
        "numsecrets": 0,
        "frames": 1354
    }],
    "score": 326,
    "levels": 1,
    "kills": 15,
    "items": 7,
    "secrets": 0,
    "frames": 1919,
    "died": true
}
```

## Limitations

The *tape* is limited to 64KB, big enough for hours of DOOM gameplay,
a game session should never reach this value.
The *outcard* is also limited to 64KB, big enough to save any JSON score.
The computation of a session is limited around 96 billion RISC-V instructions,
enough to verify about 30 minutes of DOOM gameplay,
while the validation should take less than 1/4 of the gameplay time in modern machines.
