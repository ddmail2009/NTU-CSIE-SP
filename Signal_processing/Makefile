COMPILER = g++
FLAGS = -Wall -O3

FILE = Score_Board Desk Player
RM = rm -rf
all : $(FILE)

% : %.cpp
	$(COMPILER) $(FLAGS) $< -o $@

clean :
	rm -rf $(FILE)

cleanall :
	$(RM) $(FILE)
	$(RM) Result.txt
