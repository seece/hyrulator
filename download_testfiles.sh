#!/bin/bash
echo trying to download test files rom dir...
if [ ! -d "rom" ]; then
mkdir rom
fi
wget -O rom/zelda.zip http://www.castledragmire.com/hynes/reference/resource/zelda.zip && \
unzip rom/zelda.zip -d rom

