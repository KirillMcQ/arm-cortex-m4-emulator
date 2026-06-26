SRCS = main.c memory.c instructions.c cpu.c
SRCS_W_PATH = $(addprefix ./src/, $(SRCS))

build: $(SRCS_W_PATH)
	gcc -I ./include/ $(SRCS_W_PATH) -o main

clean:
	rm main