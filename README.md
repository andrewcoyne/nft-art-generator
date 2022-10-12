# NFT Art Generator
Combines pre-drawn image layers into unique images; ideal for 2D NFT generation.

## Requirements
For the program to work correctly, you must have the following:

1. Enough hard drive space to store all of the generated images. This is actually a *lot* - 10,000 2048x2048 PNGs require around 100 GB.

2. The image layers properly sorted. Each type of layer should be sorted into a folder with all other layers of that type, and they should be further sorted by rarity.
For example:

Layers/  
├─ Face/  
│  ├─ Rare/  
│  │  ├─ rare_face.png  
│  ├─ Uncommon/  
│  │  ├─ uncommon_face.png  
│  ├─ Common/  
│  │  ├─ common_face.png  
├─ Hat/  
│  ├─ Rare/  
│  │  ├─ rare_hat.png  
│  ├─ Uncommon/  
│  │  ├─ uncommon_hat.png  
│  ├─ Common/  
│  │  ├─ common_hat.png  

3. A json file containing your project's settings.
It should be formatted like this:
```json
{
    "name": "Dogs",
    "ind_name": "Dog",
    "num": 10000,
    "layers": ["BACKGROUND","FUR","EYES","ITEM","ITEM2"],
    "rarity_level_names": ["Rare","Uncommon","Common"],
    "rarity_levels": [0.01, 0.09, 0.9],
    "check_above": ["EYES"],
    "exceptions":
    [
        ["ITEM","Toy1","MoveBehind","FUR"],
        ["ITEM","Toy2","MoveInFront","ITEM2"],
        ["ITEM","Toy3","DeleteIf","ITEM2","Toy4"]
    ]
 }
```
| Setting       | Details       |
| ------------- |:-------------|
| name     | The NFT collection's name |
| ind_name      | The name used for each individual NFT within a collection      |
| num | The number of images to generate      |
| layers | An array of all the different layer types, in the order they should be combined |
| rarity_level_names | The names of the different levels of rarity |
| rarity_levels | The probability that a layer in the given rarity level is used in a given image. They should be ordered with respect to the rarity level names (ex. if Rare is at position 0 in rarity_level_names, then the probability associated with Rare, 0.01, should be at position 0 in rarity_levels), and the rarity level probabilities should add up to 1. |
| check_above | If this layer type may be obscured by layers above it, it should go in this setting array. The generator will examine all layers above each layer in this array and remove it if it is mostly or entirely obscured. If a layer is not added to this setting array, and the traits file is used to generate NFT metadata, the metadata may be inaccurate. |
| exceptions | If the layer ordering needs to be modified in special circumstances, use an exception. Exception commands are described below. |



Exception commands:

```json
["layer-type-1", "layer1", "MoveBehind", "layer-type-2"]
```
If layer1 of type layer-type-1 is in the image, it will be moved behind the layer of type layer-type-2.

```json
["layer-type-1", "layer1", "MoveInFront", "layer-type-2"]
```
If layer1 of type layer-type-1 is in the image, it will be moved in front of the layer of type layer-type-2.

```json
["layer-type-1", "layer1", "DeleteIf", "layer-type-2", "layer2"]
```
If layer1 of layer-type-1 and layer2 of layer-type-2 are both in the image, layer1 of type layer-type-1 will be removed from the image.

## Usage
Put the nft-art-generator executable in the folder containing all image layers. Then, use the following command to run the generator:
```
./nft-art-generator [Complete filepath of the settings json file]
```
The generated images will be in the finished-images folder. A .txt file containing the lists of all traits for each image will also be generated.
