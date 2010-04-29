#!/bin/bash
if test -d ../BuildScripts
then
    cd ../BuildScripts
fi

if test -d BuildScripts
then
    cd BuildScripts
fi

if test -f ../benthicQT.dmg
then 
    rm -f ../benthicQT.dmg
fi
if test -d tmpbuild
then
    rm -rf tmpbuild
fi
mkdir -p tmpbuild
cp -r ../build/benthicQT.app tmpbuild
ls
VERSIONER_PERL_PREFER_32_BIT=yes ./dsstore-xml.pl --file benthicQT.xml tmpbuild/
mv tmpbuild/.DS_Store benthicQT.dsstore
./make-diskimage ../benthicQT.dmg tmpbuild/ benthicQT -null- benthicQT.dsstore  benthicQT.background.png
rm -rf tmpbuild

