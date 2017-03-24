FLAGS=-g -Wall -lm -O3
lib_merge_sort= lib/heap.o lib/merge_sort_dev.o

all: merge_sort_fork merge_sort_vfork

$(lib_merge_sort): lib/heap.h lib/heap.c lib/merge.c lib/merge_sort.h lib/merge_sort_dev.c lib/sort.c lib/Makefile
    make -C lib

merge_sort_fork: merge_sort_fork.c $(lib_merge_sort)
    gcc -std=c99 $(FLAGS) -o $@ $< lib/heap.o lib/merge_sort_dev.o 
merge_sort_vfork: merge_sort_vfork.c $(lib_merge_sort)
    gcc -std=c99 $(FLAGS) -o $@ $< lib/heap.o lib/merge_sort_dev.o 
clean:
    rm merge_sort_fork merge_sort_vfork
