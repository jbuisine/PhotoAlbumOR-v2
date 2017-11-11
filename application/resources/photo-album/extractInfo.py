#!/usr/bin/env python

import os, sys
from PIL import Image
import PIL.ExifTags
#from scipy.fftpack import dct
import numpy as np
import imagehash
import json
import pickle

def index_list(path):
    l = []
    for filename in os.listdir(path):
        print(filename)
        if filename.lower().endswith('.jpg'):
            l.append(int(filename.split('.')[0].split('_')[1]))
    return l

def open_images(path, ids):
    l = [ ]
    for i in ids:
        print("here")
        print(i)
        im = Image.open(os.path.join(path, "rIMG_%d.jpg" % i))
        # hash of the image
        l.append({'id': i, 'image': im, 'ahash': imagehash.average_hash(im), 'phash': imagehash.phash(im), 'dhash': imagehash.dhash(im)})
    return l

def infos(images, tagfilename, scorefilename):
    tags = tag_info(tagfilename)
    #scores = score_info(scorefilename)

    list_info = [ ]
    i = 0
    for image in images:
        d = info(i, images)

        elem = filter(lambda e: e['id'] == image['id'], tags)
        d['tags'] = { 'classes':elem[0]['classes'], 'probs': elem[0]['probs']}
        #elem = filter(lambda e: e['id'] == image['id'], scores)
        #d['score'] = elem[0]['score']

        list_info.append(d)
        i += 1

    return json.dumps(list_info)

def info(i, images):
    d = {}

    id = images[i]['id']

    # basic information
    name = "rIMG_%d.jpg" % id
    #print "{ \"index\":%d, \"id\":%d, \"name\":\"%s\"" % (i, id, name),
    d['index'] = i
    d['id'] = id
    d['name'] = name

    # from exif
    im = images[i]['image']
    exif_data = im._getexif()

    exif = {
        PIL.ExifTags.TAGS[k]: v
        for k, v in im._getexif().items()
        if k in PIL.ExifTags.TAGS
    }

    # size
    #print ", \"size\":{\"width\":%d, \"height\":%d}" % (exif["ExifImageWidth"][0], exif["ExifImageHeight"][0]),
    d['size'] = {'width': exif["ExifImageWidth"], 'height': exif["ExifImageHeight"]}

    ## Unavailable at this time
    # date
    #fdate = exif["DateTime"]
    #dt = fdate.split()
    #date = dt[0].split(':')
    #time = dt[1].split(':')
    #print ", \"date\":{\"year\":%s, \"month\":%s, \"day\":%s, \"hour\":%s, \"min\":%s, \"sec\":%s}" % (date[0], date[1], date[2], time[0], time[1], time[2]),
    #d['date'] = { 'year':int(date[0]), 'month':int(date[1]), 'day':int(date[2]), 'hour':int(time[0]), 'min':int(time[1]), 'sec':int(time[2]) }

    # dominant color
    resize = 150
    numcolor = 16
    ima = im.resize((resize, resize))
    result = ima.convert('P', palette=Image.ADAPTIVE, colors=numcolor)
    result.putalpha(0)
    colors = sorted(result.getcolors(resize*resize), reverse=True)
    #print ", \"color1\":{\"r\":%d, \"g\":%d, \"b\":%d}" % (colors[0][1][0], colors[0][1][1], colors[0][1][2]),
    #print ", \"color2\":{\"r\":%d, \"g\":%d, \"b\":%d}" % (colors[1][1][0], colors[1][1][1], colors[1][1][2]),
    d['color1'] = { 'r':colors[0][1][0], 'g':colors[0][1][1], 'b':colors[0][1][2] }
    d['color2'] = { 'r':colors[1][1][0], 'g':colors[1][1][1], 'b':colors[1][1][2] }

    # average black&white color
    image = im.resize((8, 8), Image.ANTIALIAS).convert('L')  # Resize and Convert it to grayscale.
    pixels = list(image.getdata())
    avg = sum(pixels) / len(pixels)
    #print ", \"greyavg\":%d" % avg,
    d['greyavg'] = avg

    # hash distances
    d['ahash'] = str(images[i]['ahash'])
    d['phash'] = str(images[i]['phash'])
    d['dhash'] = str(images[i]['dhash'])
    d['ahashdist'] = hashdist(images[i], images, 'ahash')
    d['phashdist'] = hashdist(images[i], images, 'phash')
    d['dhashdist'] = hashdist(images[i], images, 'dhash')

    #print "}"

    return d

def hashdist(im, images, namehash):
    #print ", \"%sdist\":[" % hashname,
    dl = [ ]
    s = len(im[namehash].hash.flatten())
    for otherim in images:
        dl.append(float(im[namehash] - otherim[namehash]) / s)
        #print "%.5f," % (float(hash - otherhash) / s),
    return dl

def tag_info(tagfilename):
    with open(tagfilename, 'rb') as f:
        l = pickle.load(f)

    res = [ ]
    for elem in l:
        res.append({'id': elem['id'], 'classes': elem['results'][0]['result']['tag']['classes'], 'probs': elem['results'][0]['result']['tag']['probs']})
    return res

def score_info(scorefilename):
    res = [ ]
    with open(scorefilename) as f:
        for line in f.readlines():
            l = line.split()
            res.append({'id': int(l[0]), 'score': int(l[1])})
    return res

#===================================================================
if __name__ == '__main__':

    if len(sys.argv) > 1:

        path = sys.argv[1] + "/img"
        tagfile = sys.argv[1] + "/taglist.pkl"
        scorefile = sys.argv[1] + "/score_photo.dat"
        fileoutname = sys.argv[1] + "/info-photo.json"

        ids = index_list(path)

        l = open_images(path, ids)

        jsoninfo = infos(l, tagfile, scorefile)

        with open(fileoutname, "w") as f:
            f.write(jsoninfo)
    else:
        print("No path found")
        sys.exit(0)