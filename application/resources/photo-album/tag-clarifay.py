#!/usr/bin/env python

import os
import sys
import pickle

from clarifai.client import ClarifaiApi

def save_obj(path, obj, name ):
    with open(path + '/'+ name + '.pkl', 'wb') as f:
        pickle.dump(obj, f, pickle.HIGHEST_PROTOCOL)

def load_obj(path, name):
    with open(path + "/" + name + '.pkl', 'rb') as f:
        return pickle.load(f)

def index_list(path):
    l = []
    for filename in os.listdir(path):
        if filename.lower().endswith('.jpg'):
            l.append(int(filename.split('.')[0].split('_')[1]))
    return l

def name_list(path, ids):
    l = [ ]
    for i in ids:
        l.append( { 'id':i, 'fullname':os.path.join(path, "rIMG_%d.jpg" % i), 'name':"rIMG_%d.jpg" % i } )
    return l


def main(argv):

    if len(sys.argv) > 1:
        api = ClarifaiApi()

        path = argv[1]

        ids = index_list(path + "/img")

        l = name_list(path + "/img", ids)

        resp = []
        for elem in l:
            with open(elem['fullname'], 'rb') as image_file:
                print(elem['name'])
                response = api.tag_images(image_file)

                response['id'] = elem['id']
                response['name'] = elem['name']

                print(response)
                resp.append(response)
        save_obj(path, resp, "taglist")

    else:
        print("No path found")
        sys.exit()



if __name__ == '__main__':
    main(sys.argv)

    #l = load_obj(sys.argv[1], "taglist")

    """
    print l[0].keys()
    print l[0]['results'][0].keys()
    print l[0]['results'][0]['result'].keys()
    print l[0]['results'][0]['result']['tag'].keys()
    print l[0]['results'][0]['result']['tag']['classes']
    print l[0]['results'][0]['result']['tag']['probs']
    """

    #for elem in l:
    #print elem['id'], elem['name'], elem['results'][0]['result']['tag']['classes'], elem['results'][0]['result']['tag']['probs']

    #print filter(lambda e: e['id'] == 1940, l)