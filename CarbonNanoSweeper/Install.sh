#!/bin/bash

set -e -u

make all

CNSDIR=/usr/share/carbon-nano-sweeper

sudo mkdir -p $CNSDIR

cat > CarbonNanoSweeper.sh <<end-of-file
#!/bin/bash

set -e -u

pushd $CNSDIR

./CarbonNanoSweeper
end-of-file

cat > carbon-nano-sweeper.desktop <<end-of-file
[Desktop Entry]
Encoding=UTF-8
Name=CarbonNanoSweeper
Comment=Find mines on a Carbonnanotube
Exec=$CNSDIR/CarbonNanoSweeper.sh
Icon=$CNSDIR/CNS256.png
Terminal=false
Type=Application
Categories=Game;Application;
StartupNotify=false
end-of-file

sudo cp -R resources/ $CNSDIR
sudo cp CNS256.png $CNSDIR
sudo cp CarbonNanoSweeper $CNSDIR
sudo cp CarbonNanoSweeper.sh $CNSDIR
sudo chmod +x $CNSDIR/CarbonNanoSweeper.sh
sudo cp carbon-nano-sweeper.desktop /usr/share/applications

rm CarbonNanoSweeper.sh carbon-nano-sweeper.desktop
