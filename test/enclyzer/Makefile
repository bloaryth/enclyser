#################### ADDITIONAL SETTINGS ####################

.PHONY: libenclyzer kenclyzer disable-prefetcher

ADDITIONAL_INCLUDE_PATHS := -I../..
ADDITIONAL_COMPILE_FLAGS := -DNAMESPACE_SGX_SHARED -DNAMESPACE_SGX_YES -DNAMESPACE_SGX_NO
ADDITIONAL_LINK_FLAGS := -L../../enclyzer/libenclyzer -lenclyzer_urts -lenclyzer_trts
ADDITIONAL_TARGETS := libenclyzer kenclyzer disable-prefetcher

libenclyzer:
	@make -C ../../enclyzer/libenclyzer --quiet

kenclyzer:
	@sudo make -C ../../kenclyzer clean all load --quiet

disable-prefetcher:
	@sudo modprobe msr
	@sudo wrmsr -a 0x1a4 0xf

#################### COMMANDS TO USE ####################

.PHONY: all clean

#################### TEST COMPILATION ####################

TEST_C_FILES := test-enclyzer-libenclyzer-attack.c test-enclyzer-libenclyzer-memory.c \
	test-enclyzer-libenclyzer-lfb.c test-enclyzer-libenclyzer-flush_reload.c \
	test-sgx-analyser-app.c
TEST_C_OBJECTS := $(patsubst %.c, %.o, $(TEST_C_FILES))

TEST_INCLUDE_PATHS := $(ADDITIONAL_INCLUDE_PATHS)
TEST_COMPILE_FLAGS := -O2 -Wall -Wno-unknown-pragmas $(ADDITIONAL_COMPILE_FLAGS) $(TEST_INCLUDE_PATHS)
TEST_LINK_FLAGS := -lcriterion $(ADDITIONAL_LINK_FLAGS)

TEST_EXECUTABLE := test

all: $(ADDITIONAL_TARGETS) $(TEST_EXECUTABLE)

%.o : %.c
	@$(CC) $(TEST_COMPILE_FLAGS) -c $< -o $@

$(TEST_EXECUTABLE): $(TEST_C_OBJECTS)
	@$(CC) $^ -o $(TEST_EXECUTABLE) $(TEST_LINK_FLAGS)

run:
	@sudo $(CURDIR)/$(TEST_EXECUTABLE) --verbose -j1

clean:
	@rm -rf *.o $(TEST_EXECUTABLE)