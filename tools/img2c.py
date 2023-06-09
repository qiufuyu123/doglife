import sys
from PIL import Image


def get_arr_img(fn, id):
    image = Image.open(fn)
    img_arr = ""
    img_arr = str("\n"
                  "#define {id}_width  {w}\n"
                  "#define {id}_height {h}\n"
                  "\n"
                  "const uint8_t PROGMEM {id}_data[] = {{\n"
                  .format(id=id, w=image.width, h=image.height))

    for y in range(0, image.height):
        for x in range(0, (image.width + 7)//8 * 8):
            if x == 0:
                img_arr += str("  ")
            if x % 8 == 0:
                img_arr += str("0b")

            bit = '0'
            if x < image.width and image.getpixel((x, y))[0]>128:
                bit = '1'
            img_arr += str(bit)

            if x % 8 == 7:
                img_arr += str(",")
        img_arr += str("\n")
    img_arr += str("};")
    # 将img_arr存为文件
    with open(f"img_arr.h".format(id), "w") as f:
        f.write(img_arr)


if __name__ == '__main__':

    file_name = sys.argv[1]
    arr_name = sys.argv[2]
    get_arr_img(file_name, arr_name)
    print("Done!")