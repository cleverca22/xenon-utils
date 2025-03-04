nand-info: nand-info.cpp
	$(CXX) -o $@ $< -std=c++23

install: nand-info
	mkdir -pv $(out)/bin
	cp -vr nand-info $(out)/bin/
