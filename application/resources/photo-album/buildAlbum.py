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
    def create_page(self, html_dir, numPage, photoList):
        f = open(os.path.join(html_dir, "%s_%d.html" % (self.album["basename"], numPage)), "w")
        self.create_body_page(f, numPage, photoList)
        f.close()

    '''
    Create the bottom of the page 
    @param numPage the number fo the page
    '''
    def create_body_page(self, f, numPage, photoList):
        f.write("<div id=\"album\">\n")
        f.write("<h3>Page %d</h3>\n" % (numPage + 1))

        self.create_navigation(f, numPage)

        f.write("<div id=\"pictures\" style=\"width:%dpx; height:%dpx\">\n" % (self.album["pages"][numPage]["width"], self.album["pages"][numPage]["height"]))
        pos = 0
        for i in photoList:
            photo = self.album["pages"][numPage]["photos"][pos]
            f.write("<img src=\"../img/%s\" width=\"%d\" height=\"%d\">\n" % (self.photos[i]["name"], photo["width"], photo["height"]))
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
            f.write("<li><a href=\"page_%d.html\" aria-label=\"Previous\"><span aria-hidden=\"true\">&laquo;</span></a></li>\n" % (numPage -1))

        for i in range(0, self.album["page"]):
            if(i == numPage):
                f.write("<li class=\"active\">")
            else:
                f.write("<li>")
            f.write("<a href=\"page_%d.html\">%d</a></li>\n" % (i, i+1))

        if numPage < self.album["page"] - 1:
            f.write("<li><a href=\"page_%d.html\" aria-label=\"Next\"><span aria-hidden=\"true\">&raquo;</span></a></li>\n" % (numPage + 1))

        f.write("</ul>\n")
        f.write("</nav>\n")
        f.write("</div>")

    ''' 
    Create all the pages of the album 
    @param html_dir
    @param photos_dir
    @param solution_name the order of the photo in the album
    '''
    def create_album(self, html_dir, solution_name, line_file):
        # read the solution order
        with open(solution_name) as f:
            lines = f.readlines()
            photosOrder = [ int(x) for x in lines[int(line_file)].split(',')[0].split() ]

        n = 0
        for i in range(self.album["page"]):
            self.create_page(html_dir, i, photosOrder[n:(n+self.album["pagesize"][i])])
            n = n + self.album["pagesize"][i]

#===================================================================
if __name__ == '__main__':
    photos_dir     = "img"                                      # path to images from the html directory

    if len(sys.argv) > 3:
        if not os.path.exists(sys.argv[1]):
            print ("Template not found: " + sys.argv[1])
            sys.exit()
        else:
            template_folder = sys.argv[1]

        if not os.path.exists(template_folder + sys.argv[2]):
            print ("Album type not found: " + sys.argv[2])
            sys.exit()
        else:
            album_name = template_folder + sys.argv[2]

        if not os.path.exists(template_folder + "/info-photo.json"):
            print ("Info photo JSON file not exist for this template: " + template_folder) # file name of the photo information
            sys.exit()
        else:
            album_info = template_folder + "/info-photo.json"

        if not os.path.exists(template_folder + "solutions/" + sys.argv[3]):
            print ("Solution file not found into: " + template_folder + "solutions") # file name of the photo information
            sys.exit()
        else:
            solution_name = template_folder + "solutions/" + sys.argv[3]

        if not len(sys.argv) > 4:
            solution_line = 0
        else:
            solution_line = sys.argv[4]

        album = Album(album_name, album_info)
        album.create_album(template_folder + "html", solution_name, solution_line)
    else:
        print ("Some arguments are required")
        sys.exit()



