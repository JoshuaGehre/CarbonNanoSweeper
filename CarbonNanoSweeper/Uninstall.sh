#!/bin/bash

set -e -u

pushd /usr/share > /dev/null
	if [ -d carbon-nano-sweeper ]
	then
		sudo rm -r carbon-nano-sweeper
	else
		echo "CarbonNanoSweeper files not installed!"
	fi
popd > /dev/null

pushd /usr/share/applications > /dev/null
	if [ -f carbon-nano-sweeper.desktop ]
	then
		sudo rm carbon-nano-sweeper.desktop
	else
		echo "CarbonNanoSweeper .desktop file not set!"
	fi
popd > /dev/null

pushd /usr/bin > /dev/null
	if [ -L carbon-nano-sweeper ]
	then
		sudo rm carbon-nano-sweeper
	else
		echo "CarbonNanoSweeper symbolic link not set!"
	fi
popd > /dev/null

echo "CarbonNanoSweeper uninstalled!"
