make hc
make hcd

rm out/source.zip
zip -r out/source.zip Makefile doit.sh hc.cpp hcd.cpp

mkdir -p out

./hc data/a.in > out/a.out
./hc data/b.in > out/b.out
./hc data/c.in > out/c.out
./hc data/d.in > out/d-ignore.out
./hcd data/d.in > out/d.out
./hc data/e.in > out/e.out
