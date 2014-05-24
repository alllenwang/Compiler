

play:
	flex lexical.l
	bison -d syntax.y
	gcc main.c tree/tree.c syntax.tab.c table/table.c semantics/semantics.c intercode/intercode.c intercode/createintercode.c -lfl -ly -o parser

clean:
	rm -rf parser
	rm -rf lex.yy.c
	rm -rf syntax.tab.c
	rm -rf syntax.tab.h
	rm -rf out1.ir
	rm -rf *.*~
	rm -rf */*.*~
	rm -rf *~
	
