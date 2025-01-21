all : modpack-nix

modpack-nix : main.cpp
	g++ -std=c++20 $^ -o $@