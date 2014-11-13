Another SNAKE
=============

![intro](https://github.com/macfij/another-snake/blob/master/images/snake_in.png)

This is a snake game made for fun by two programming noobs.
Linux, C++ and SDL 1.2 were used. 8bit sounds/music can be found in game.
Intro music and other sounds were created by macfij, while main game music
was made by Eric Skiff.

Dependecies
===========

On Debian-based distros you are good to go with:
`apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev`. However, on Debian 7  I had to install `libgpg-error-dev` also.

On Fedora : `yum install SDL-devel SDL_mixer-devel SDL_image-devel SDL_ttf-devel`.

Installation
============

Simply type `make`. This will result in creating binary in bin directory.

Controls
========
| Key(s)        | Action                                    |
| -----------   | ----------------------------------------- |
| Arrows        | Navigate around                           |
| Spacebar      | Skip intro; Play again after lost game    |
| Wsad / arrows | Snake controls                            |
| Esc           | Pause the game                            |

Features
========
- file based high scores
- modifiable snake speed and size
- full screen support
- wsad/arrows snake control
- background black/white

Snake changes his color based on coordinates. He has some similiarities with chameleon.
Image below presents snake just before his lunch. See how colorful he is?

![game](https://github.com/macfij/another-snake/blob/master/images/snake_game.png)

Thanks to
=========
- Lazyfoo and his tutorials. See http://lazyfoo.net/SDL_tutorials/.
- Eric Skiff and his music. See http://ericskiff.com/music/.

License
=======
- GPLv3.

Authors
=======
- Jakub Rewieński
- Maciek Fijałkowski

