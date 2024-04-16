# FREEDOOM Cartridge

This is the source code for DOOM ported to RIV using FREEDOOM assets.
It's the very same cartridge used on [RIVES](https://rives.io).

The DOOM game is run with RIVEMU,
an special emulator used by RIVES with graphics, audio and keyboard support,
that embeds the Cartesi Machine emulator to be strongly deterministic.

## Compiling

To build, first make sure you have the RIVEMU and RIV SDK before installed,
check its documentation on how to do this.

Having the SDK set, to build is very simple, just clone the repository and run make:

```sh
git clone https://github.com/rives-io/cartridge-freedoom.git
cd cartridge-freedoom
make
```

## Playing

First you need to download `freedoom.sqfs`, you can either download from
[this link](https://github.com/rives-io/cartridge-freedoom/releases/latest/download/freedoom.sqfs)
or build yourself following the instructions above.

To play, just run with `rivemu`:

```sh
rivemu freedoom.sqfs
```

These are the controls to play:
- Ctrl - Fire
- Space - Use / Open
- Left/Right - Turn
- Up/Down - Walk
- Alt + Left/Right - Walk laterally
- Tab - Game Map
- Shift - Run

## Recording and replaying a tape

You can use tapes to store gameplay sessions to be shared or watched again.

Record a tape of your gameplay with `-record` option in RIVEMU:

```sh
rivemu -record=gameplay.rivtape freedoom.sqfs
```

And then you can replay it with `-replay` option:

```sh
rivemu -replay=gameplay.rivtape freedoom.sqfs
```

## Inspecting a tape score

You can inspect a tape score by replaying with `-save-outcard` option:

```sh
rivemu -replay=gameplay.rivtape -save-outcard=score.json -speed=2 freedoom.sqfs
cat score.json
```

## History

Running DOOM inside the Cartesi machine in real time was originally accomplished in
[this tweet](https://twitter.com/edubart/status/1621512794339446786).

Since then the project evolved into a fantasy console called RIV,
and its distribution platform called RIVES,
and now FREEDOOM is just a game of this ecosystem.

## Compute machine

A cartesi machine for computing scores of DOOM game sessions
is provided in the [/compute-machine](/compute-machine) directory
as an example on how to embed this in a cartesi machine.

