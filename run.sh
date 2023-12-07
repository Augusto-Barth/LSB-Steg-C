rm -r ./out/*

mkdir -p out/big
mkdir -p out/small

gcc lsbStegDiffuse.c -o lsbStegDiffuse -lpng -O2

for img in {1..7}
do
    for i in {1..8}
    do
        ./lsbStegDiffuse in/"imag_"$img".png" out/small/"out_"$img"_"$i"b.png" message.txt $i
        ./lsbStegDiffuse in/"imag_"$img".png" out/big/"out_"$img"_"$i"b.png" message_g.txt $i
    done
done