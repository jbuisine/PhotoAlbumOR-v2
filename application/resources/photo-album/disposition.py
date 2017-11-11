#!/usr/bin/env python

import os, sys

def generate_head(nb, page_size, file):
    file.write("\t\"page\": {0},\n".format(nb))

    file.write("\t\"pagesize\":[")
    for i in range(1, nb+1):
        if i != nb:
            file.write("{},".format(page_size))
        else:
            file.write(str(page_size))
    file.write("],\n")

    file.write("\t\"basename\":\"page\",\n")

def generate_page(x, y, nb, file):
    x_size = 250
    y_size = 187
    x_start = 21
    y_start = 6
    margin = 12

    file.write("\t\"pages\":[\n")

    for i in range(1, nb+1):
        width = x*x_size + (x-1)*margin + x_start*2
        height = y*y_size + (y-1)*margin + y_start*2

        #Starting writing page information
        file.write("\t\t{{ \n\t\t\t\"width\":{} , \"height\":{}, \"photos\":[\n".format(width, height))

        x_axys = x_start
        y_axys = y_start

        nb_photo_per_page = y*x
        for j in range(1, y+1):
            for k in range(1, x+1):

                file.write("\t\t\t\t{{\"x\": {}, \"y\": {}, \"width\": {}, \"height\": {} }}".format(x_axys, y_axys, x_size, y_size))
                if j*k != nb_photo_per_page:
                    file.write(",\n")
                else:
                    file.write("\n")

                x_axys += x_size + margin

            x_axys = margin
            y_axys += y_size + margin

        #Ending page information
        if i != nb:
            file.write("\t\t\t]\n\t\t },\n")
        else:
            file.write("\t\t\t]\n\t\t}\n")

    file.write("\t]\n")


#===================================================================
if __name__ == '__main__':

    main_path = "./../resources/data/"


    if len(sys.argv) > 3:

        template = sys.argv[1]
        filename = sys.argv[2]
        x_elem = int(sys.argv[3])
        y_elem = int(sys.argv[4])
        nb_page = int(sys.argv[5])

        if ".json" not in filename:
            filename = filename + ".json"

        if not os.path.exists(main_path + template):
            os.makedirs(main_path + template)

        path_file = main_path + template + "/" + filename

        with open(path_file, "w+") as f:
            f.write("{\n")
            generate_head(nb_page, x_elem*y_elem, f)
            generate_page(x_elem, y_elem, nb_page, f)
            f.write("}")
    else:
        print("No params found")
        sys.exit(0)