rm -r ./out/small

mkdir -p out/small

gcc lsbStegDiffuse.c -o lsbStegDiffuse -lpng -O2

count=$(ls in/ | wc -l)

for ((img = 1; img < count; img = img+1))
do
    for i in {1..8}
    do
        ./lsbStegDiffuse in/"imag_"$img".png" out/small/"out_"$img"_"$i"b.png" message.txt $i $1
    done
done