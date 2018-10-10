for i in {1..500}
do
	./tsc -u "User#$i" < in.txt > out.txt &
	echo "Started user $i"
done
#./tsc -u "test" &
