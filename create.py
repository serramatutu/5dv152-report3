for i in range(1, 18):
    num = 2**i
    with open(f"in/4/{num}", "w") as f:
        f.write(str(num) + "\n")
