i=1
for f in *.png; do
    mv -- "$f" "imag_"$i".png"
    i=$((i+1))
done