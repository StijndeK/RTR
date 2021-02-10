# RTR (MVP)
Adapting parameters of audio in real-time, instead of playing cutup pieces of the audio based on game state and events, could be a practical and efficient game audio design method. Time and energy can be saved by using adaptive risers instead of other currently standard nonlinear audio development techniques. RTR is a game audio tool that lets the user design a riser that adapts to game data in real time. Not only does this provide an original adapting riser effect, the tool aims to optimise the workflow. RTR is made using a low level audio combination of [FMOD](https://fmod.com/) low level and layer-based system build on top of it.

The tool can be implemented in UE4 using the [UE4 integration plugin](https://github.com/StijndeK/RTR_UE4Integration).

![Screenshot (23)](https://user-images.githubusercontent.com/31696336/106205591-05452f00-61bf-11eb-9e43-9032ad42582e.png)

Risers are an interesting audio effect to implement in an adaptive system, because instead of synchronising to their start point they are synced to the impact in between the attack and release. By adapting parameters such as pitch and amplitude in the attack of the riser, the riser can adapt to nonlinear game data.

<img width="1235" alt="Screenshot 2021-01-28 at 16 03 13" src="https://user-images.githubusercontent.com/31696336/106205781-55bc8c80-61bf-11eb-9aab-bfdd4743eaab.png">

The user can design the riser and some options on how fast it should adapt in the standalone tool. This tool exports the users choices to a .Json datafile, that is imported by an Unreal engine 4 (or any other engine) plugin. In the engine some game data is passed on to the plugin. The standalone tool and its UE4 plugin use the same audio playing and layering system, so that the audio can be tested instantaniously within the tool as well as the engine.

## Mapping riser audio parameters to game
Audio parameters are mapped to game data in different ways. Aspects such as player position, player speed, and movement over time translate into audio parameters such as modulationspeed and gain modulation.


## Dataflow
![RTR_GeneralDataflow2](https://user-images.githubusercontent.com/31696336/104494070-41309f80-55d6-11eb-9640-c1777158fe26.png)

Both the engine and the tool use the same audio and json-to-audio-translation systems. The tool has a user interface to edit settings and design the riser, and the engine plugin has a blueprint to get the game data to adapt to.

![RTR_DataflowSplit](https://user-images.githubusercontent.com/31696 336/104633126-fb8bd980-569e-11eb-92f6-6616ba3970e0.png)

The AudioBase class contains the specific elements for the surrounding framework, so that the rest of the AudioSystem is exactly the same for every framework or engine. For example, the AudioBase handles debugmessages as every engine has its own debugging system build in. 

## Modulation types
*dataflows*

*functionality description 3 types*

*functionality description*

*codex*

### treshold checkers
*functionality description*

*codex*

### curves
*functionality description*

*codex*


## Layering
![RTR_Layersystem](https://user-images.githubusercontent.com/31696336/104494075-41c93600-55d6-11eb-9480-007c5f8846e3.png)

The audio is divided into two types of layers. Evert layer has its own modulation, sounds and channels. Currently most of the settings for these layer's sound design and their modulation are done in the back-end. In future releases, the tool could start providing controll to the user on the sound design how to modulate the layers. 

## Code examples

## Future developments
The current tool has been made to be as quick and easy to use as possible. Herefore a lot of features do not have user controll and are done in the back-end. However, there are a lot of interesting possibilities when moving them to the front-end and providing complete controll over how to adapt the riser and to what. 
