CC=clang
EXEC = libcltk.so cltk_test cltk_image_test gemm
all: $(EXEC)

CFLAGS  = -I include -I stb
CFLAGS += -Wno-deprecated-declarations -fPIC

LIB_LDFLAGS = -L /usr/lib/x86_64-linux-gnu/ -lOpenCL
LIB_OBJS = \
	src/cltk.o

LDFLAGS = -L. -lm -lcltk -lOpenCL
OBJS = \
	example/cltk_test.o

IMG_OBJS = \
	example_image/cltk_image_test.o

GEMM_OBJS = \
	gemm_cl/gemm.o

deps =
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -MMD -MF $@.d -c $<
deps += $(OBJS:%.o=%.o.d)
deps += $(IMG_OBJS:%.o=%.o.d)
deps += $(GEMM_OBJS:%.o=%.o.d)
deps += $(LIB_OBJS:%.o=%.o.d)

cltk_test: $(OBJS) libcltk.so
	$(CC) -o $@ $^ $(LDFLAGS)

cltk_image_test: $(IMG_OBJS) libcltk.so
	$(CC) -o $@ $^ $(LDFLAGS)

gemm: $(GEMM_OBJS) libcltk.so
	$(CC) -o $@ $^ $(LDFLAGS)

libcltk.so: $(LIB_OBJS)
	$(CC) -shared -o $@ $^ $(LIB_LDFLAGS)

clean:
	$(RM) -f $(EXEC) $(LIB_OBJS) $(OBJS) $(IMG_OBJS) $(GEMM_OBJS) $(deps)
