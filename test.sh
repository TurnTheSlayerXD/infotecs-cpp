#!/bin/bash


cmake -S ./progone -B ./progone/build && cmake --build ./progone/build 
cmake -S ./progtwo -B ./progtwo/build && cmake --build ./progtwo/build


./progone/build/progone | ./progtwo/build/progtwo > echo