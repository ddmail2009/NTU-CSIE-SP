FLAGS=-Wall -O3
all: sort merge
heap.o: heap.c
	gcc $(FLAGS) -c heap.c
merge_sort_dev.o: merge_sort_dev.c heap.o
	gcc $(FLAGS) -c merge_sort_dev.c
sort: sort.c merge_sort_dev.o heap.o
	gcc $(FLAGS) sort.c merge_sort_dev.o heap.o -o sort
merge: merge.c merge_sort_dev.o heap.o
	gcc $(FLAGS) merge.c merge_sort_dev.o heap.o -o merge
clean:
	rm *.o sort merge
