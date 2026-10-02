# CHIP-8 Emulator

A CHIP-8 emulator written in C with SDL2.

![IBM logo running in the emulator](screenshot.png)

## Build

```
gcc main.c -o main.exe -lmingw32 -lSDL2main -lSDL2
```

## Run

```
./main.exe            # pick a ROM from a menu
./main.exe game.ch8   # or run one directly
```

## Controls

```
1 2 3 4
Q W E R
A S D F
Z X C V
```

Passes the [Timendus CHIP-8 test suite](https://github.com/Timendus/chip8-test-suite).
