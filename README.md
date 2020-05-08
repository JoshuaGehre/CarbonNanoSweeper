# CarbonNanoSweeper

CarbonNanoSweeper is a game similar to Minesweeper but it's played on a carbon nanotube.
It is part of the Minesveeper series on [heptaveegesimal.com](https://heptaveegesimal.com/).

## Controls

**Left Click**
Dig a tile

**Right Click**
Place a flag

**W, A, S, D**
Rotate or move the carbon nanotube

**Q, E**
Look to the side

**1, 2**
Change color scheeme

**3, 4, 5, 6**
Change textures

**Enter**
Restart

**Esc**
Quit

## Compiling

Under Ubuntu based Distros the following packages are required for compiling CarbonNanoSweeper:

sudo apt-get install -y g++ freeglut3 freeglut3-dev libxmu-dev libxmu-headers libxi-dev libxrandr-dev libxcursor-dev libxinerama-dev libglfw3-dev

Run the make file within the CarbonNanoSweeper subdirectory to compile this.
You will then be able to run CarbonNanoSweeper as long as it is within the same directory as the resources directory.

To install CarbonNanoSweeper you can run the Install.sh script.

If you instead want CarbonNanoSweeper as a Visual Studio Project, switch to the branch *vs-project*.
However, that branch is not up to date and has sometimes shown issues with compiling.

## Libraries

This project additionaly uses the following libraries:
- [Glm](https://glm.g-truc.net)
- [Stb](http://nothings.org/)
