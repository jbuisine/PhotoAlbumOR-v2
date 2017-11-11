#!/usr/bin/env python
# -*- coding=utf-8 -*-

import json
import time
import os, sys

#----------------------------------------------------
class Album:
    '''
    @param photoFilename the file name with the description of the photos
    @param albumFileName the file name with the description of the album
    '''
    def __init__(self, albumFileName, photoFileName):
        # read the file with the photos
        data = None
        with open(photoFileName) as data_file:
            data = json.load(data_file)
        self.photos = [ None ] * len(data)
        for p in data:
            self.photos[ p["index"] ] = p
            # read the file with the album description
        with open(albumFileName) as data_file:
            self.album = json.load(data_file)

            #print "%d %s" % (self.data[1]["id"], self.data[1]["name"])

    '''
    Create one page of the album
    @param numPage the number fo the page
    @param photoList the ordered list of photo in the page
    '''
    def create_page(self, html_dir, photos_dir, numPage, photoList):
        f = open(os.path.join(html_dir, "%s_%d.html" % (self.album["basename"], numPage)), "w")
        self.create_body_page(f, photos_dir, numPage, photoList)
        f.close()

    '''
    Create the bottom of the page 
    @param numPage the number fo the page
    '''
    def create_body_page(self, f, photos_dir, numPage, photoList):
        f.write("<div id=\"album\">\n")
        f.write("<h3>Page %d</h3>\n" % (numPage + 1))

        self.create_navigation(f, numPage)

        f.write("<div id=\"pictures\" style=\"width:%dpx; height:%dpx\">\n" % (self.album["pages"][numPage]["width"], self.album["pages"][numPage]["height"]))
        pos = 0
        for i in photoList:
            photo = self.album["pages"][numPage]["photos"][pos]
            f.write("<img src=\"/<%%= templateName %%>/%s/%s\" width=\"%d\" height=\"%d\">\n" % (photos_dir, self.photos[i]["name"], photo["width"], photo["height"]))
            pos += 1

        f.write("</div>\n") # pictures
        self.create_navigation(f, numPage)

        f.write("</div>\n") # album

    '''
    Create the navigation link in the album
    @param numPage the number of the page
    '''
    def create_navigation(self, f, numPage):
        f.write("<div id=\"navigation\">\n")
        f.write("<nav aria-label=\"Page navigation\">\n")
        f.write("<ul class=\"pagination\">\n")

        if numPage > 0:
            f.write("<li><a href=\"/template/<%%= templateName %%>/%d\" aria-label=\"Previous\"><span aria-hidden=\"true\">&laquo;</span></a></li>\n" % (numPage -1))

        for i in range(0, self.album["page"]):
            if(i == numPage):
                f.write("<li class=\"active\">")
            else:
                f.write("<li>")
            f.write("<a href=\"/template/<%%= templateName %%>/%d\">%d</a></li>\n" % (i, i+1))

        if numPage < self.album["page"] - 1:
            f.write("<li><a href=\"/template/<%%= templateName %%>/%d\" aria-label=\"Next\"><span aria-hidden=\"true\">&raquo;</span></a></li>\n" % (numPage + 1))

        f.write("</ul>\n")
        f.write("</nav>\n")
        f.write("</div>")

    ''' 
    Create all the pages of the album 
    @param html_dir
    @param photos_dir
    @param solution_name the order of the photo in the album
    '''
    def create_album(self, html_dir, photos_dir, solution_name, line_file):
        # read the solution order
        with open(solution_name) as f:
            lines = f.readlines()
            photosOrder = [ int(x) for x in lines[int(line_file)].split(',')[0].split() ]

        n = 0
        for i in range(self.album["page"]):
            self.create_page(html_dir, photos_dir, i, photosOrder[n:(n+self.album["pagesize"][i])])
            n = n + self.album["pagesize"][i]

#===================================================================
if __name__ == '__main__':
    album_name     = "../resources/data/"                       # file name of the album information
    html_dir       = "../www/views/templates/"                  # path to html source files
    photos_dir     = "img"                                      # path to images from the html directory
    solution_name  = "../resources/data/chronologic-order.sol"  # (default) file name of the solution which gives the assignement of the photos

    if len(sys.argv) > 1:
        if not os.path.exists(sys.argv[1]):
            print ("File not found: " + sys.argv[1])
            sys.exit()
        else:
            solution_name = sys.argv[1]

        if not os.path.exists(album_name + sys.argv[3] + "/" + sys.argv[2]):
            print ("Album type not found: " + sys.argv[2])
            sys.exit()
        else:
            album_name += sys.argv[3] + "/" +sys.argv[2]
            print (album_name)

        if not os.path.exists(html_dir + sys.argv[3]):
            print ("Template folder not found: " + sys.argv[3])
            sys.exit()
        else:
            photos_name    = html_dir + sys.argv[3] + "/info-photo.json"        # file name of the photo information
            album = Album(album_name, photos_name)
            album.create_album(html_dir + sys.argv[3], photos_dir, solution_name, sys.argv[4])

            #Write info values
            with open(solution_name) as f:
                lines = f.readlines()

                head = lines[0].split(',')
                values = lines[int(sys.argv[4])].split(',')

                info = " ["

                for (i, item) in enumerate(head):
                    info +=  item + " : " + format(float(values[i+1]), '.2f')

                    if i+1 < len(head):
                        info += ", "

                info += "] "

                infoFile = open(html_dir + sys.argv[3] + "/info.txt", "w")
                infoFile.write(solution_name + info)
                infoFile.close()




