if test "$#" -ne 1; then
        mpiexec -n $1 $2
    else
        mpiexec -n 4 $1
fi
