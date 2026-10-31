import math


def main():
    print("{", end="")
    for i in range(0, 256):
        print(
            math.floor(
                math.sqrt(math.sin(
                    i / 255 * math.pi
                )) * 255
            ),
        end=",")
    print("}")

if __name__ == "__main__":
    main()

