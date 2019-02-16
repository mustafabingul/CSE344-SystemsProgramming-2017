gcc -c server.c && gcc -o exe server.o -lpthread -lm
gcc -c client.c && gcc -o exe2 client.o -lpthread -lm

echo "start Server"
gnome-terminal -x ./exe 6060 dataFinal.dat log
sleep 2
echo "start client"
gnome-terminal -x ./exe2 ersin C 22 127.0.0.1 6060
gnome-terminal -x ./exe2 ersin C 22 127.0.0.1 6060
