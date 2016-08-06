EXEC = libcltk.so cltk_test
all: $(EXEC)

CFLAGS  = -I include
CFLAGS += -Wno-deprecated-declarations -fPIC

LIB_LDFLAGS = -lOpenCL
LIB_OBJS = \
	src/cltk.o

LDFLAGS = -L. -lm -lcltk -lOpenCL
OBJS = \
	example/cltk_test.o

deps =
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -MMD -MF $@.d -c $<
deps += $(OBJS:%.o=%.o.d)
deps += $(LIB_OBJS:%.o=%.o.d)

cltk_test: $(OBJS) libcltk.so
	$(CC) -o $@ $^ $(LDFLAGS)

libcltk.so: $(LIB_OBJS)
	$(CC) -shared -o $@ $^ $(LIB_LDFLAGS)

clean:
	$(RM) -f $(EXEC) $(LIB_OBJS) $(OBJS) $(deps)
