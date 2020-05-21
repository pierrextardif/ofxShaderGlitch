# ofxShaderGltich
Glitch Shader generator In [Openframeworks](https://openframeworks.cc).

 </br> The goal is to easily creaqte glitch effects in OF.
 It uses a set of masks, and image processing effects to create glitchy effects.
</br>

- - - -

![.](Assets/example.gif)

- - - -
## Getting Started
### Installs

### 1)Installing OpenFrameworks

Get the latest OpenFrameworks version from [here](https://openframeworks.cc/download)(0.11.0).
</br>

### 2) Installing

Add this repository to your addons folder, or to the local_addons folder.
</br>
If using the local_addons folder, make sure the pre processing `#define LOCALADDON`  is **present** in the `constants.h` and in `main.frag`.
</br>
If using the addons folder, make sure the pre processing `#define LOCALADDON`  is **commented** in the `constants.h` and in `main.frag`.


</br>
The only dependecy is [ofxAutoReloadedShader](https://github.com/andreasmuller/ofxAutoReloadedShader) from [Andreas Muller](https://github.com/andreasmuller), used to reload shader on the fly to help compiling only shaders.
</br>
</br>



## Example

Just try running [the example](./example).


## Effects
The glitches are created with 2 sections : tiling and effects.
The tiling create patterns on which to apply the effects.

There is 10 tiles and 10 effects.

## Author

* _pierre Tardif_   [codingcoolsh.it](codingcoolsh.it)   :floppy_disk:

## License

This project is licensed under the MIT License - see the [LICENSE.md](./LICENSE) file for details.


## Acknowledgments

* [Three.js inspiration](https://threejs.org/examples/?q=glitch#webgl_postprocessing_glitch)
