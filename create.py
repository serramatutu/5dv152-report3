import os


def create_4():
    os.makedirs("in/4", exist_ok=True)

    for i in range(1, 18):
        num = 2**i
        with open(f"in/4/{num}", "w") as f:
            f.write(str(num) + "\n")

def create_5():
    os.makedirs("in/5", exist_ok=True)

    dimension_facs = [1/8, 1/4, 1/2, 1, 2, 4, 8]
    for dim_fac in dimension_facs:
        dim_x = int(1920 * dim_fac)
        dim_y = int(1080 * dim_fac)
        iterations = 1000
        mandelbrot_coords = "-0.5 0.0 1.0"

        file_name = f"{dim_x}x{dim_y}-{iterations}iter"

        with open(f"in/5/{file_name}", "w") as f:
            lines = [
                dim_x,
                dim_y,
                mandelbrot_coords,
                iterations,
                f"out/{file_name}.png"
            ]
            f.writelines(str(line) + "\n" for line in lines)


def create_5_jobscripts():
    os.makedirs("jobscripts", exist_ok=True)

    with open("submit-job-template.sh", "r") as f:
        template = f.read()
    
    for impl_fullname in os.listdir("bin/5"):
        impl = impl_fullname.strip(".out")
        for infile in os.listdir("in/5/"):
            with open(f"jobscripts/{infile}_{impl}.sh", "w") as f:
                f.write(
                    template.format(
                        impl=impl,
                        infile=infile
                    )
                )



if __name__ == "__main__":
    # create_4()
    # create_5()
    create_5_jobscripts()
