# PhotoAlbumRO-v2

Photo album project implementation using ParadisEO Framework.

## Description

Relative to the [first version](https://github.com/jbuisine/RO.PhotoAlbumProject/blob/master/www/modules/routes/solution.js) project which contains client interface. This second version is created to improve performance using c++ ParadisEO library and found best AOS (adaptive operators strategy) for this real problem. 

So, some statistics will be generated in order to compare the different AOS known and perhaps propose a new one.

## Installation

This project is a cmake project. So you need to install all dependencies before build it:

* [ParadisEO library](http://paradiseo.gforge.inria.fr/index.php?n=Doc.Install)
* [RapidJSON](https://github.com/Tencent/rapidjson)

Some python scripts are used to run application or create new photo album. You also need some python dependencies :

[Clarifai](https://www.clarifai.com/) dependency used for getting some information about your photo in order to compare them.
```
pip install clarifai==2.0.20
```

Some others dependencies required :
```
pip install imagehash
pacman -S grahpicsmagick
```

## Utilisation

Explanations will be available soon.

## Licence

[CeCILL](http://www.cecill.info/index.en.html)
