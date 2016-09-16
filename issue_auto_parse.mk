objs+=${OUT}/issue_auto_parse.o 
CFLAGS+=

all: mk_out ${OUT}/issue_auto_parse
mk_out:
	mkdir -p out

${OUT}/issue_auto_parse.o: apps/app_tools/app_prj_cmd/issue_auto_parse.c
	${CC} ${CFLAGS} -c -o $@ $<

${OUT}/issue_auto_parse: ${objs}
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -rf ${OUT}