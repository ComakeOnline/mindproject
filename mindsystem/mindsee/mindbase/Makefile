all:
	arm-buildroot-linux-uclibcgnueabihf-g++ baseDemo.cpp -o baseDemo -L./ -lstream -I./ -std=c++11

strip:
	arm-buildroot-linux-uclibcgnueabihf-strip -S -g -d baseDemo

copy:all strip
	cp baseDemo ~/ForBoard
