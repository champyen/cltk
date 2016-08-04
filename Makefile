EXEC = cltk_test
all: $(EXEC)

CFLAGS  = -I include
CFLAGS += -Wno-deprecated-declarations

LDFLAGS = -lOpenCL -lm

OBJS = \
	src/cltk.o \
	example/cltk_test.o

deps =
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -MMD -MF $@.d -c $<
deps += $(OBJS:%.o=%.o.d)

cltk_test: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) -f $(EXEC) $(OBJS) $(deps)
