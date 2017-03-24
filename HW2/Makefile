Compiler = g++
FILE = hw2
RM = rm -rf
Target = SP_HW2_0 SP_HW2_1 SP_HW2_2 ME TEST ./ qqq

all : $(FILE)

$(FILE) : $(FILE).cpp
	$(Compiler) $< -o $@

$(Target) : $(FILE)
	./$(FILE) $@
	$(RM) $(FILE)

clean :
	$(RM) $(FILE)
