dir=/usr/bin


if test -d $dir; then
    echo "it is a directory"
elif test -h $dir; then
    echo "it is a symbol link"
else
    echo "i dont know"
fi
