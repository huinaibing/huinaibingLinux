changeDir()
{
    if [ $# -ne 1 ]; then
        echo "error number of value"
        return 1
    fi

    if [ -e $1 ]; then
        echo "file/dir exist"
        return 2
    fi

    mkdir -p $1

    cd $1
    pwd
}

changeDir $1