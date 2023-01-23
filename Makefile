
# create bin dir if not exists
bin:
	mkdir -p -- bin

4_FLAGS = -pedantic-errors -Wall -Wextra -fopenmp -O1
4_FILES = 4.c

# compile 4
4: 4_serial 4_parallel1 4_parallel2
	echo "Done."

4_bin: bin
	mkdir -p -- bin/4

4_serial: 4_bin
	clang ${4_FLAGS} -o bin/4/serial.out ${4_FILES}

4_parallel1: 4_bin
	clang -D PARALLELIZE_1 ${4_FLAGS} -o bin/4/parallel1.out ${4_FILES}

4_parallel2: 4_bin
	clang -D PARALLELIZE_2 ${4_FLAGS} -o bin/4/parallel2.out ${4_FILES}


# compile 5
5_FLAGS = -fopenmp -Wall -Wextra -pedantic -O3
5_FILES = 5.cpp lodepng.cpp

5: 5_serial 5_for 5_pixel_task 5_row_task
	echo "Done."

5_bin: bin
	mkdir -p -- bin/5

5_serial: 5_bin
	g++ ${5_FLAGS} -o bin/5/serial.out ${5_FILES}

5_for: 5_bin
	g++ -D P_FOR ${5_FLAGS} -o bin/5/for.out ${5_FILES}

5_pixel_task: 5_bin
	g++ -D P_PIXEL_TASK ${5_FLAGS} -o bin/5/pixeltask.out ${5_FILES}

5_row_task: 5_bin
	g++ -D P_ROW_TASK ${5_FLAGS} -o bin/5/rowtask.out ${5_FILES}