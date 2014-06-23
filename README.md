Research in next-generation tools for developing smart physical devices and 
autonomous robots - with Sumorobots as a case-study.

Some of these ideas closely mirror what [Brett Victor](http://worrydream.com/)
argues for in [Seeing spaces](http://vimeo.com/97903574). The project can be seen
as possible practical research in realizing these, shaped by the special case of Sumo robot fighting.

Status
--------
*Prototyping*. Active IR-tracking has working prototype.

Components
-----------
* Position tracking using active-IR markers and IR-filtered cameras
* Streaming real-life video (mjpegstreamer?)
* Physical rig for camera setup above Sumo ring
* 3d visualization using JavaScript+WebGL
* Sensor/data visualization timeline using JavaScript
* Timeline syncronization based on sensor stream and video marker
* Streaming of robot sensory data and state from the firmware
* Streaming commands to the robot

Milestones
--------------

**0.0.1: Proof-of-concept. 3d cube following robot overlayed on real video stream**

* Allow to set height, calibrate the IR-camera view and real video to eachother
* Stream the coordinates from OpenCV-based detector over HTTP/WS
* Map the video and IR-coordinates into a 3d model (three.js?)
* Draw a 3d cube of robot at position given by tracking and configured height

**0.1.0: Minimally useful. Basic robot control and visualizations of sensory input**

* Correction of lens projection, to get uniform spacing in the plane
* Use multiple markers per robot, in a shape (L or _ | _) that allows detecting orientation
* Visualize and overlay sensor data from robot into 3d view: Opponent and line detection
* Ability to control robot from IDE


Build & run
------------

Only tested on Linux.

Have /dev/video0 be a IR camera, and /dev/video1 be a visible spectrum camera
    
    g++ -o opencv opencv.cpp `pkg-config --cflags --libs opencv` && ./opencv

Need to have OpenCV installed.
