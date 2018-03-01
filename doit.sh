make

zip -r out/source.zip Makefile doit.sh hc.cpp

mkdir -p out

# ./hc 0 data/a.in > out/a.out
# ./hc 1 data/b.in > out/b.out
# ./hc 2 data/c.in > out/c.out
# ./hc 3 data/d.in > out/d.out
# ./hc 4 data/e.in > out/e.out

./hc 0 data/a.in
./hc 0 data/b.in
./hc 0 data/c.in
./hc 1 data/d.in
./hc 0 data/e.in
