from subprocess import Popen, PIPE

FRAME_COUNT=350

def video():
    for frame in range(FRAME_COUNT):
        zoom = (frame + 1) * 0.08

        lines = [
            1920,
            1080,
            f"-0.749 0.1006 {zoom}",
            250,
            f"out/video/{frame:03d}.png"
        ]
        input_data= "\n".join(str(line) for line in lines)

        p = Popen(["./bin/5/rowtask.out"], stdout=PIPE, stdin=PIPE, stderr=PIPE)
        stdout_data = p.communicate(input=input_data.encode("utf-8"))[0]
        print(f"Finished frame {frame}.")


if __name__ == "__main__":
    video()