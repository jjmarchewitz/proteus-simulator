TARGET = Proteus_Test
FIRMWAREREPO := fehproteusfirmware

.PHONY: all deploy clean run

ifeq ($(OS),Windows_NT)	
	SHELL := CMD
endif


all:
ifeq ($(OS),Windows_NT)	
	@cd $(FIRMWAREREPO) && mingw32-make all TARGET=$(TARGET)
	@cd $(FIRMWAREREPO) && mingw32-make deploy TARGET=$(TARGET)
else
	@cd $(FIRMWAREREPO) && make all TARGET=$(TARGET)
	@cd $(FIRMWAREREPO) && make deploy TARGET=$(TARGET)
endif

deploy:
ifeq ($(OS),Windows_NT)	
	@cd $(FIRMWAREREPO) && mingw32-make deploy TARGET=$(TARGET)
else
	@cd $(FIRMWAREREPO) && make deploy TARGET=$(TARGET)
endif

clean:
ifeq ($(OS),Windows_NT)	
	@cd $(FIRMWAREREPO) && mingw32-make clean TARGET=$(TARGET)
else
	@cd $(FIRMWAREREPO) && make clean TARGET=$(TARGET)
endif

run:
ifeq ($(OS),Windows_NT)	
	@cd $(FIRMWAREREPO) && mingw32-make run TARGET=$(TARGET)
else
	@cd $(FIRMWAREREPO) && make run TARGET=$(TARGET)
endif