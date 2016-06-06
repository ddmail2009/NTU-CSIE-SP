return value:
	0: PROCESS FINISHED SUCCESSFULLY
	1: INPUT FORMAT IS WRONG
	-1: ROOT PATH DOESN'T EXIST


Makefile:
    make: create the excutable file
    make clean: forcely remove the excutable file
    make $(target): define $(target) in Makefile. This command will excute "make"
                    and make $(target) as the target of the file then remove it.
