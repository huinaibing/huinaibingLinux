x=0
while [ $x -lt 10 ]
do 
    y=0
    while [ $y -le $x ]
    do
        echo "$y \c"
        y=$(($y+1))
    done
    echo
    x=$(($x+1))
done