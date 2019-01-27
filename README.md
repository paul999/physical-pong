# physical-pong

Do you remember pong from a long time ago? Well, I did, and I decided to make a physical version of the game.

This is the code required to run on the arduino mega. You can find more information about this project on the site: www.physical-pong.com

# Installing

This code is written for a arduino mega. You will need the platformio editor if you want to have it work out of the box.
It won't work with the default arduino IDE out of the box, and this is not something I plan on supporting.

If you are using this code without the ramps board as suggested, please make sure to check the pins in config.h, otherwise it won't work. 
In config.php you can also enable DEBUG mode, which does write debug statements into the Serial console for debugging. It is suggested to disable this if you don't need it.

Before compiling, make sure you set the correct processor in paltformio.ini, otherwise you won't be able to upload your code to your Arduino.