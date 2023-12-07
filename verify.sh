mkdir -p out/small/temp
mkdir -p out/big/temp

rm out/small.txt
rm out/big.txt

count=$(ls in/ | wc -l)

for ((img = 1; img < count; img = img+1))
do
    for i in {1..8}
    do
        ./lsbStegDiffuse out/small/"out_"$img"_"$i"b.png" out/small/temp/"text_out_"$img"_"$i"b.txt" $i
        cmp -n 50166 message.txt out/small/temp/"text_out_"$img"_"$i"b.txt" >> out/small.txt


        ./lsbStegDiffuse out/big/"out_"$img"_"$i"b.png" out/big/temp/"text_out_"$img"_"$i"b.txt" $i
        cmp -n 340871 message_g.txt out/big/temp/"text_out_"$img"_"$i"b.txt" >> out/big.txt

    done
done
