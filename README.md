Research in next-generation tools for developing smart physical devices and 
autonomous robots - with Sumorobots as a case-study.

Components
-----------
* Position tracking using active-IR markers and IR-filtered cameras
* Streaming real-life video (mjpegstreamer?)
* Visualization using JavaScript+WebGL
* Streaming of robot sensory data and state from the firmware
* Streaming commands to the robot

Milestones
--------------

**0.0.1: 3d cube following robot overlayed on real video stream**

* Allow to set height, calibrate the IR-camera view and real video to eachother
* Stream the coordinates from OpenCV-based detector over HTTP/WS
* Map the video and IR-coordinates into a 3d model (three.js?)
* Draw a 3d cube of robot at position given by tracking and configured height

**Next**

* Correction of lens projection, to get uniform spacing in the plane
* Use multiple markers per robot, in a shape (L or _|_) that allows detecting orientation
* Visualize and overlay sensor data from robot into 3d view.

