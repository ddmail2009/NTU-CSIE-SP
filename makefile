Compiler = gcc
FILES = query return borrow
RM = rm -rf

all : $(FILES)

% : %.cpp
	$(Compiler) $< -o $@

clean :
	$(RM) $(FILES)
	$(RM) book.log