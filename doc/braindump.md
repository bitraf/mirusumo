Robotics, smart-physical device development
=============================================

Setup
------

Standard sumo ring placed on floor/table.
Video cameras above, fixed height over ring, with whole ring in view.
One visible light camera, one IR camera for accurate motion tracking.
Active IR diode on robot. Could also use IR diodes at know positions on ring for auto-calibration of camera.
Place ring+objects into 3d space use a rectilinear projection and lens distortion correction

Use high-level programming models for firmware, dataflow/FSM-automata (MicroFlo/Finito)
Stream all device its sensor input, internal state changes, and output decisions over to computer/IDE.
If possible, stream live over wireless connection. BT/WiFi.
Currently 9600 baud enough for 1k messages/second, 115200baud for 10k msg/sec.
Messages are timestamped according to internal clock of robot.
A blinking sequence at start(+stop?) on robot acts as a "clapper" for syncronizing time source with IDE

As the robot moves around, move a corresponding 3d-model around in the virtual world in IDE.
Visualize sensor information from robot in this world, including line/edge detection and opponent location
Also visualize intended motion vectors based on robot motor control output.
Could also allow a robot first-person-view, based on its actual sensory knowledge overlayed with the
syntesized information provided by the motion tracking cameras. Could also allow manual override of
the control over the robot, such that one can navigate around to explore behavior of sensing systems.

Data collection, storage and processing done on dedicated embedded Linux box. Intel NUC or generic PC.
Web APIs and interface allows access to the system. 

IDE tools
----------
- Timeline view of all sensor input values
- Syncronized video+sensor value feeds
- Scrubber for moving back/forward in time


Further ideas
------------
End-to-end automated tests:

By exposing the camera tracking system, as well as manual control of both robot-under-test
and an opponent robot, one could write tests of the entire software+hardware system.
For instance in BDD style:
* When robot is facing opponent straight on, opponent is detected and attacked
* When robot is near line on left/right side, turn right/left to avoid
The tests would actually move the robots into desired position/orientation to set up the scenario,
let it play out and verify that correct results occurred. One can start with very basic cases,
and then progress to trickier/edgier cases. Should probably be data-driven to allow to easily add variation to cases.
Cases where the opponent is active during scenarion probably the most advanced, can potentially
test actual battle scenarios being played out:
* When pushed from the front-right side, spin clockwise
Might also want to run tests where a set of invariants are verified periodically, like:
* When opponent moves around, robot is always facing it

Programming-by-example

Program by teaching the robot how to behave in certain scenarios. Programming interface
can be moving the robot around by hand, recording a "macro" - then cleaning up and tweaking
the data to form the desired scenario.
Actually realizing the code might require constraint solving and/or more sophisticated AI.
Could alternatively be seen as a way to build up testcases, that one could then make green
by traditional means.

Augmented reality projection:

By using a projector mounted together with the cameras, the sensor information and processed
simulation data could be projected back onto the sumo ring. Can possibly make it easier to
make discoveries and reason about behavior of the robots.
Also opens possibilities to project a config/programming interface to the surface, where one
can change the software, not just observe its (current) outputs.

Online-physical fighting:

Because the entire fight is streamed, one could allow online particpants to upload new code to the robots.
If the setup is standardized, could host virtual matches between hackerspaces, one round at each physical location.
Or, run a simultan run at both locations - where have to win in both places for a point.

Evolutionary development of fighting algorithm:

Automate determination of which robot wins round, and pick-up-and-return-to-start hardware.
For each generation, evolve the software, run a set of fights for, determine fitness
If 300 seconds per generation (10 fights), can iterate ~250 times per day.



Implementation
---------------
Can use blob or Houghcircles from OpenCV to track IR points.
* http://www.keithlantz.net/2011/04/using-opencv-to-process-images-for-blob-detection-with-sdl-and-opengl-for-rendering/
* http://docs.opencv.org/modules/features2d/doc/common_interfaces_of_feature_detectors.html#simpleblobdetector
* https://code.google.com/p/touchlib/
TODO: figure out how to calculate orientation.


Related research
==================

* [Phybots](http://junkato.jp/phybots/), robotics toolkit based on ceiling mounted camera
* [RoboTable: An Infrastructure for Intuitive Interaction with Mobile Robots in a Mixed-Reality Environment]
(http://www.hindawi.com/journals/ahci/2012/301608/)

