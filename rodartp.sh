#!/bin/bash

qntProc=$1
qntVezes=$2

if [ $# -ne 2 ]; then
	echo "Precisa utilizar 2 argumentos"
	echo "1- Numero de processos"
	echo "2- Numero de vezes pra rodar o programa"
	exit 1
fi

echo "Quantidade de processos = $qntProc"
echo -e "Quantidade de ciclos = $qntVezes\n"


for i in $(seq 1 $qntVezes)
do
	echo "Ciclo $i"
	echo -e "-----\n"
	mpirun -np $((qntProc+1)) ./tp
	echo -e "-----\n"
done
