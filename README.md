# Barnes-Hut


## Installation instructions

The SDL2 library is used for rendering the final result. On Debian
systems, this is done with
```
apt-get install libsdl2-dev
```

On MacOS, use
```
brew install sdl2
```

Compile with 
```
make
```

and run with 
```
bin/barneshut [THETA] [NUM_BODIES] [TIME_STEP]
bin/barneshut 1 5000 0.1
bin/barneshut 1 5000 0.1 --frames # Saves frame times to file
```