SHELL		= /bin/bash
TARGET		= test_exe

CXX			= clang++
CXXFLAGS	= -Wall -Wextra -Werror -O3 -x c++ -std=c++20
LDFLAGS		= 
RM			= rm -f

ifdef SANITIZE
	CXXFLAGS += -g
	LDFLAGS += -fsanitize=address
endif

${TARGET}:
	@${CXX} ${CXXFLAGS} ${INCLUDES} -o ${TARGET} ${SOURCES} ${LDFLAGS}

all: ${TARGET}

__bstree:
	@${MAKE} SOURCES="main.cpp __bstree/__tree" INCLUDES="-I__bstree" ${TARGET}

__avltree:
	@${MAKE} SOURCES="main.cpp __avltree/__tree" INCLUDES="-I__avltree" ${TARGET}

__btree:
	@${MAKE} SOURCES="main.cpp __btree/__tree" INCLUDES="-I__btree" ${TARGET}

clean:
	@${RM} *.o

fclean: clean
	@${RM} ${TARGET}

.PHONY: __bstree __avltree __btree clean fclean

.NOTPARALLEL: __bstree __avltree __btree clean fclean