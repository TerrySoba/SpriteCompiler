coptions = -ml -oneatx -xs -xr -zp4 -0 -ad

all: dostest.exe

%.o : %.cpp
	@mkdir -p $(dir $@)
	wpp $(coptions) -fo=$@ $<

-include *.d

FORCE:

dostest.exe: main.o
	echo "NAME   $@" > game.lnk
	echo "SYSTEM DOS" >> game.lnk
	echo "OPTION STACK=20000" >> game.lnk
	echo "FILE {main.o}" >> game.lnk
	wlink  @game.lnk

clean:
	find . -name "*.d" -type f -delete
	find . -name "*.o" -type f -delete
	find . -name "*.lnk" -type f -delete
	find . -name "*.exe" -type f -delete
	find . -name "*.err" -type f -delete