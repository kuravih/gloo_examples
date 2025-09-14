all: 11_cpu_test 11_gpu_test

11_cpu_test:
	make -C "./cpu_test"

11_gpu_test:
	make -C "./gpu_test"

clean:
	make clean -C "./cpu_test"
	make clean -C "./gpu_test"