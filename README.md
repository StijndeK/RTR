# Real-Time Risers Prototype
Adapting parameters of audio in real-time instead of playing cutup pieces of the audio based on game state and events, could be a practical and efficient game audio design method. Time and energy can be saved by using adaptive risers instead of other currently standard nonlinear audio development techniques. RTR is a prototype for an adaptive audio tool that lets the user design a riser that adapts to game data in real time. Not only does this provide an original adapting riser effect it also saves time and energy in the audio design workflow. RTR is made in C++ using a combination of [FMOD](https://fmod.com/) low-level API and [OpenFrameworks](https://openframeworks.cc/). The GUI has been developed using [ofxDatGui](https://github.com/braitsch/ofxDatGui).

Audio parameters are mapped to game data in different ways. Aspects such as player position, player speed, and movement over time translate into audio parameters such as modulationspeed and gain modulation. For more information on the tool visit its [webpage](http://sdkoning.com/PF/RTR.html).

The tool can be implemented in UE4 using the [UE4 integration plugin](https://github.com/StijndeK/RTR_UE4Integration).

![Screenshot (25)](https://user-images.githubusercontent.com/31696336/107762003-9a523700-6d2c-11eb-8367-d1198573482b.png)

The user can design the riser and its modulation in the Editor Tool. This tool exports the users settings to a .Json datafile, that is imported by an Unreal engine 4 (or any other engine) plugin. In the engine game data is passed to the plugin, using actors (game objects) and/or blueprints. The standalone tool and its UE4 plugin use the same audio playing and layering system, so that the audio can be tested instantaniously within the tool as well as the engine.

<img width="1235" alt="Screenshot 2021-01-28 at 16 03 13" src="https://user-images.githubusercontent.com/31696336/106205781-55bc8c80-61bf-11eb-9aab-bfdd4743eaab.png">

## Dataflow
Both the game engine plugin and the standalone tool use the same audio system which is build on the FMOD core API. The tool has a user interface to edit settings and design the riser while the engine plugin has blueprints and actors to integrate the riser in the game. The tool writes the user data to a json file that includes the 'recipe' on how the riser should sound. In the standalone tool the riser also be played, because of which the tool also reads the json. The applications can be run seperatly, but also simultaniously. The game engine plugin reads the json data and links the riser to game data.

![RTR_Dataflow](https://user-images.githubusercontent.com/31696336/115718005-02e81080-a37b-11eb-8967-385d031cfd9d.png)

## Layering
Preset sounds have been designed by deconstruction riser examples to static loop-able pads. These pads loop and parameters such as pitch and gain are modulated to create there rising effect. The audio has been divided up into different layers such as synths, noise and Shepard scales.

There are two types of layers. Evert layer has its own modulation, sounds and channels. Currently most of the settings for these layer's sound design and their modulation are done in the back-end. In future releases, the tool could start providing controll to the user on the sound design how to modulate the layers. 

![RTR_Layersystem](https://user-images.githubusercontent.com/31696336/104494075-41c93600-55d6-11eb-9480-007c5f8846e3.png)

## Modulation
To allow a riser to go on for a longer or shorter amount of time, while still sounding naturally within its context, 3 modulation types have been defined. The first is *Position Modulation* which modulates up or down in intensity based on game progression. The second is *Action Modulation* which modulates up or down in intensity based on amount of game action over time. The third is *Time Modulation*, which slowly modulates down in intensity after a certain amount of time is spent without reaching the impact of the riser. 

## Future developments
The current tool has been made to be as quick and easy to use as possible. Because of which a lot is done in the background. However, there are a lot of interesting possibilities when moving them to the front-end and providing complete controll over how to adapt the riser and to what. 
