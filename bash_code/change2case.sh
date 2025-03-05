read ANS

case $ANS in
    "Yes"|"yes"|"y"|"Y") ANS="y"
    ;;
    *) ANS="n"
    ;;
esac

echo "the value of ANS is $ANS"