FIRMWAREFOLDER := Firmware
SIMULATORFOLDER := Simulator

.PHONY: proteus simulator clean

# Change shell to CMD if on Windows
ifeq ($(OS),Windows_NT)	
	SHELL := CMD
endif


proteus:
ifeq ($(OS),Windows_NT)	
	@cd $(FIRMWAREFOLDER) && mingw32-make all
else
	@cd $(FIRMWAREFOLDER) && make all
endif

simulator:
ifeq ($(OS),Windows_NT)	
	@cd $(SIMULATORFOLDER) && mingw32-make all
else
	@cd $(SIMULATORFOLDER) && make all	
endif

clean:
ifeq ($(OS),Windows_NT)	
	@cd $(FIRMWAREFOLDER) && mingw32-make clean
	@cd $(SIMULATORFOLDER) && mingw32-make clean
else
	@cd $(FIRMWAREFOLDER) && make clean
	@cd $(SIMULATORFOLDER) && make clean
endif