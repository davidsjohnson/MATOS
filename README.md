# MATOS
### Multi Agent Tangible Object Software

MATOS is a software package for interactive multi-agent sound generation specifically designed for 
use with tangible user interfaces. As part of this project, MATOS will be integrated with the AudioCube 
hardware design of Robert Van Rooyen (see the AudioCube Design Document).  MATOS is developed using C++ 
for handling agent communication, logic and reasoning; while PureData with LibPd is used for sound 
generation, with C++ integration using LibPd.  This design ensures that MATOS is cross platform and can 
be built on any system with the correct libraries installed using CMake.  Additionally using PureData 
allows for robust and flexible sound design even for composers not familiar with C++.
## Installation

### Dependencies
* [CMake](https://cmake.org/)
* [RtAudio](https://www.music.mcgill.ca/~gary/rtaudio/)
* [Pure Data - Vanilla](https://puredata.info/)
* [LibPD](https://github.com/libpd/libpd)
  * And the [C++ Wrapper](https://github.com/libpd/libpd/wiki/cpp)
* [oscpack](http://www.rossbencina.com/code/oscpack)

### Instructions
1. Install above dependencies
   * CMake currently assumes that dependencies are in `/usr/local/include` and their libraries are in `/usr/local/lib`
   * Update CMakeLists.txt if installations are located elsewhere
2. Clone MATOS repository from GitHub: 
   * `git clone https://github.com/fortjohnson/MATOS.git`
3. Build MATOS
	1. `mkdir build && cd build`
	2. `cmake .. -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles"`
	3. `make`
4. Run the application
	1. Edit `neighbors.txt` with the IP addresses and osc listening ports of other MATOS agents
	2. `./MATO -i <agent id> -o <osc port> -p <PD patch location> -n <neighbors file> -s <number of states>`
		* __agent id:__ any unique integer
		* __osc port:__ port for MATOS to listen for OSC messages from agents (default = 7000)
			* the OSC port sensor input is the agent port number + 1000 (default = 8000)
		* __PD patch location:__ path to the PD patch for sound generation (default = pd/main.pd)
		* __neighbors file:__ path to the file containing connection information for other MATOS agents
		* __number of states:__ in the `states` folder
		* __*Example:*__ `./MATO -i 0 -o 7000 -p ../pd/main.pd -n ../neighbors.txt -s 4`
		
## MATOS Demo Instruction
The `demo` folder contains the necessary files for a demo of MATOS with 4 agents. After successfully building and 
running MATOS on each computer 

1. Browse to the `demo` folder
2. Select one of the 4 demo folders and open the corresponding `pd` folder
3. Copy the `samples` and `states` folders and `main.pd`
4. Paste into the `pd` folder of the root `MATO` folder to replace the existing the existing files
5. Now from the `pd` folder, copy both the `states` and `samples` folders
6. Paste in the root `MATO` folder 
	* (this is a workaround that I'm hoping to address in the future)
7. Edit the `neighbors.txt` file to configure the agent with a list of neighbors
	* Each line represents one neighbor agent.  
		* The first entry in the file is the agent id
		* The second entry is the agent host's IP address
		* The third entry is the agent's OSC port for agent communication (will most likely be 7000 for all agents)
8. Run MATOS on each computer from the `build` folder
	* `./MATO -i 0 -o 7000 -p pd/main.pd -n ../neighbors.txt -s 4`
	* make sure to change the agent ID for each agent (i.e. the `-i` parameter)
	
To test sensor input, I've also include a [TouchOSC](https://hexler.net/software/touchosc) patch to mimic various inputs.
and controls.  

1. Load the patch into TouchOSC - <https://hexler.net/docs/touchosc-configuration-layout-transfer-wifi>
2. Configure TouchOSC with the IP address and Port of the agent you'd like to communicate with
	* Remember the sensor input port is the port entered in the command line parameters + 1000 (so 8000 by default)
3. Open the Patch
4. You should now be able to start and stop the Agent's sound generation, modify the tempo and volume, and change the 
agent's state

* NOTE:  To see how agents communicate, notice that when you change the tempo on one agent, the other agents will follow.
	
## Sound Design via PD

While any PD patch that meets the MATOS PD API can be used, this iteration of MATOS includes a PD patch 
consisting of a 16 bar sequencer for 4 samples.  The patch can be found at `pd/main.pd`.  The  `pd` folder 
also contains two folders for storing your samples and states.  States are simply the sequences you designed
and saved through the PD patch. 

To edit the patch, first launch `pd/main.pd` (you can close the "modules" window that opens automatically).  
The patch is preloaded with 4 default samples but you can load new samples by putting PD in edit mode (`cmd-e`) and 
typing the path to the sample in the message box.  Then, back in interact mode (`cmd-e` again), click the *bang* to
load the sample.

To start the sequencer click the orange *bang* at the top left.  Once you've designed a sequence you like, click the green 
*save* button and save the file to the `states` folder with the name `state#.txt` where # is the number of the state.  You 
can load saved states with the *load* button or by entering the state number in the state number object.  

(One possible workflow that I've used while designing MATOS, is to build a sound world in Ableton Live.  Then decide which 
samples should go to a given agent, export the samples, load into the PD patch, and edit the sequence.)  

### Designing a New Patch
All new PD patches should implement the following *receive* addresses:
* `[r $0-start-fromCpp]` - receives a *bang*
* `[r $0-tempo-fromCpp]` - receives a float value
* `[r $0-state-fromCpp]` - receives a float value (representing the state number to load)
* `[r $0-volume-fromCpp]` - receives a float value

Additionally, each receive object should have a corresponding *send* object that sends the result of the change back to MATOS
* `[s $0-start-toCpp]` - send a *bang*
* `[s $0-tempo-toCpp]` - sends a float value
* `[s $0-state-toCpp]` - sends a float value (representing the state number to load)
* `[s $0-volume-toCpp]` - sends a float value

New send and receive objects can be added to the patch but must also be implemented in C++ (ie LibPD) for sending and receiving
data between MATOS and PD.

## The Agent
An agent in MATOS encapsulates all components necessary for agent understanding the world, communication between sensors and other 
agents, and sound generation. Most of the development of a new Agent happens in the Agent's `init()` method.  Here you can register
BDI components (see below), add new sensor and agent communication monitors.  See Agent.cpp for a demo implementation that handles 
sensor and agent communication, sound generation and BDI processing.


## Behavior, Desires and Intentions (BDI) Framework
MATOS implements a BDI model for rational agents
* See the [MATOS Design Document](https://docs.google.com/document/d/13v__0yu_7cN7dBeFLIvByGPHt0D2tJNSggpq87T6fMQ/edit?usp=sharing) 
for full details on adding new behaviors and goals

### Beliefs
Beliefs represent information about the world that the agent obtains through sensors and other agents. In MATOS, beliefs are 
composed of an url styled ID indicating the Agent ID and Parameter Name, the parameter name, and a list of parameter values. 
For example, a belief about agent #5’s tempo would be represented as: 
- **ID:**		/tempo/5 
- **parameterName:**	tempo 
- **parameterValues:** 	[90]

The main Agent has a belief database that is a map of type `shared_ptr<map<string, shared_ptr<Belief>>>`

* NOTE: The ID uses a URL style format which makes it easy to assign from an incoming OSC Message (see Agent Communication Section for more details).


### Desires (ie Goals) and Intentions
A desire (or Goal) represents a condition that the agent is trying to achieve. In the case of MATO, a *condition* is a boolean 
expression that evaluates to either true or false based on the current beliefs of the agent. For example, a goal maybe represented 
by the condition: 

* `myTempo % worldTempo == 0 or worldTempo % myTempo == 0`
	* Implemented in code it is a vector of strings (see Agent.cpp)
	
Each goal has an associated intention, ie *action*, which represents what the agent should do based on the result of goal evaluation using the 
agent's belief database. 

```c++
void tempoAction(bool goalMet, Beliefs beliefs)
{
    if (! goalMet)
 	    updateState(“tempo”, beliefs.worldTempo)
}
```
 
In MATOS, actions are represented by callback functions that are registered with the goal, see the Agent.cpp for examples.

### The Interpreter

The interpreter is the reasoning system of the agent; given a set of beliefs, desires and intentions what should the agent do?  

For each Intention registered with the agent, the interpreter processes the beliefs to generate parameters for the intentions.  
Then Each intention is evaluated to see if the goal is met and an action is generated.  The interpreter then selects actions to 
perform and executes those actions.  

These steps run inside an update loop that is run every N milliseconds (currently set to 500 ms).  

## Sensor Input and Agent Communication
Agents must be able to acquire knowledge about the world (including other agents in the world) to generate beliefs about the 
surrounding environment.  General world knowledge is acquired through various types of sensors, such as proximity sensors, light
sensors and tangible interfaces to name a few.  Agent knowledge is shared and acquired by communicating directly with an agent's 
neighbors.  This information is processed by the BDI model to come up with a set of actions that change the state of the world; 
in our case actions will generally map to some sound output. 
 
In MATOS, all input is handled though the Open Sound Control(OSC) messaging protocol (with the exception of audio input, although
an independent audio processing component could be implemented to send OSC messages for certain auditory events).

### Sensors

MATOS has a monitor system that allows sensor drivers to communicate with the Agent via OSC.  This way 
sensors and feature extraction are implemented independently of the Agent and updates the belief database by communicating through 
the OSC API for sensors.  Information from each sensor will update the belief database with the given parameter value. 
For example, an OSC Message `/volume/1 90` indicates that the belief database should be updated with the volume for agent 1 to 
a value of 90.

Using OSC also affords the opportunity to use *[Wekinator](http://www.wekinator.org/)* for processing sensor data with machine 
learning algorithms.

OSC Callbacks are implemented in the Agent.cpp to indicate how certain input from a sensor should be processed.

### Agent Communication

Agent Communication is also perform through OSC. Each agent is started with a list of *neighbors* that includes the IP Address and the 
OSC port of all neighbors (see the `neighbors.txt` file for an example).  Anytime an agent’s state is changed, the agent sends a 
message with the parameterName and parameterValue to all registered neighbors.  The receiving agents then update their belief 
database based on the incoming messages.  Using OSC allows each agent to independently decide what to do with incoming messages.

OSC Callbacks are implemented in the Agent.cpp to indicate how agent input should be processed.
 
* NOTE: Each agents’ list of neighbors can be different affording possibilities for some interesting graph based communication patterns.


## Future Work
* Simplify the MATOS BDI API.  Possibly by integrating a scripting language such as Python for all behavior, goal and action
implementations or by adding a GUI.
* Improve beat syncing between agents for better time alignment.
* Improve the Sound design interface.  Given the right resources, it would be interesting to sync Ableton Live directly
with the libPD and MATOS.