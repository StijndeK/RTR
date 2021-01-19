# RTR (MVP)
Adapting parameters of audio in real-time, instead of playing cutup pieces of the audio based on game state and events, could be a practical and efficient game audio design method. Time and energy can be saved by using adaptive risers instead of other currently standard nonlinear audio development techniques. RTR is a game audio tool that lets the user design a riser that adapts to game data in real time. Not only does this provide an original adapting riser effect, the tool aims to optimise the workflow. RTR is made using a low level audio combination of [FMOD](https://fmod.com/) low level and layer-based system build on top of it.

The tool is implemented in UE4 using the [UE4 integration plugin](https://github.com/StijndeK/RTR_UE4Integration).

**Current status: MVP reached**

![RTR_Pitch 001_Git](https://user-images.githubusercontent.com/31696336/104464204-f3ef0680-55b2-11eb-9cd9-ab35327b2e41.png)

Risers are interesting because instead of synchronising to their start point, they are synced to the impact in between the attack and release. By adapting parameters such as pitch and amplitude in the attack of the riser, the riser can adapt to nonlinear game data.

![RTR_Pitch 002_Git](https://user-images.githubusercontent.com/31696336/104463994-ba1e0000-55b2-11eb-90bb-23ad7fe00a7b.png)

The user can design the riser and some options on how fast it should adapt in the standalone tool. This tool exports the users choices to a .Json datafile, that is imported by an Unreal engine 4 (or any other engine) plugin. In the engine some game data is passed on to the plugin. The standalone tool and its UE4 plugin use the same audio playing and layering system, so that the audio can be tested instantaniously within the tool as well as the engine.

## Mapping riser audio parameters to game
Audio parameters are mapped to game data in different ways. Aspects such as player position, player speed, and movement over time translate into audio parameters such as modulationspeed and gain modulation.

## Layer & modulation system
![RTR_Layersystem](https://user-images.githubusercontent.com/31696336/104494075-41c93600-55d6-11eb-9480-007c5f8846e3.png)

Different layers all have their own modulators, sounds and channels.

## Dataflow
![RTR_GeneralDataflow2](https://user-images.githubusercontent.com/31696336/104494070-41309f80-55d6-11eb-9640-c1777158fe26.png)

Both the engine and the tool use the same audio and json-to-audio-translation systems. The tool has a user interface to edit settings and design the riser, and the engine plugin has a blueprint to get the game data to adapt to.

![RTR_DataflowSplit](https://user-images.githubusercontent.com/31696336/104633126-fb8bd980-569e-11eb-92f6-6616ba3970e0.png)

The AudioBase class contains the specific elements for the surrounding framework and audio class itself is the exact same between all surrounding frameworks and engines.

## Improvements
![RTR_MVPRoadmap](https://user-images.githubusercontent.com/31696336/105043909-66a22b00-5a66-11eb-8fc1-6c9146cb2036.png)

### UI to add
- *ranges and curves for modulation*
- modulation direction
- curve
- visualisation (drawing)
### features to add
- panning
- panning modulation
- *controll over parameter linking and grouping*
- get minimum attack time from engine
- *predict player time to goal and improve / lessen attack up speed*
- some wait or slow down time before turning 
- *curve per layer*
- *ask for plugin location and save it*
- *visualise envelopes while making them*
- *convert curve value to 0 1 value*
### cleanup
- only stop sounds that are playing
### UE4 integration
- plotting in seperate place
- clean up rest of audiosystem to implement in ue4
### readme
- *Coding examples*
- *video*
- *Site page*
