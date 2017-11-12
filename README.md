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

And finally :
```
mkdir build && cd build
cmake ..
```

## Utilisation

### Generate information 

First of all, you need to respect this folder template architecture :

TemplateFolder :
- img/ 

#### 1. Clarifai API data
Generate tags information from an template folder :

```
cd application/resources/photo-album
python tag-clarifay.py <template-folder> <YOUR_CLARIFAI_API_KEY>
```

Example :
```
cd application/resources/photo-album
python tag-clarifay.py templates/FirstTemplate <YOUR_CLARIFAI_API_KEY>
```

#### 2. Generate info photo file

In order to generate json file information about photos template, you need to run this command line :
```
python extractInfo.py <template-folder>
```

#### 3. Generate disposition info file

```
python disposition.py <template-folder> <output-filename> <x-axis-number-photos> <y-axis-number-photos> <number-of-pages>
```
## Licence

[CeCILL](http://www.cecill.info/index.en.html)
