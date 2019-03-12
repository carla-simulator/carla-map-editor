CARLA Map Editor
===============


[![carla.org](Docs/img/btn/web.png)](http://carla.org)
[![download](Docs/img/btn/download.png)](https://github.com/carla-simulator/carla/blob/master/Docs/download.md)
[![discord](Docs/img/btn/chat.png)](https://discord.gg/8kqACuC)
<!-- [![forum](Docs/img/btn/forum.png)](link here) -->

The CARLA Map Editor can be used to load the FBX and XODR files created in the RoadRunner application and update them with traffic lights and speed signs. 

Building CARLA Map Editor
--------------------------

Currently the editor is provided for Linux only. It will be available for Windows in the near future. 

Use `git clone` or download the project from this page. Note that the master
branch contains the latest fixes and features, for the latest stable code may be
best to switch to the `stable` branch.

Run Update.sh to download the assets to the Content folder. 

Run make --package to build the Carla Editor distribution package and create the application.

Run make --launch to build the Carla Editor application and open it in the Unreal Editor.

Using the CARLA Map Editor
--------------------------
- Traffic lights, groups and speed signs can be placed in the map.

- The different assets available can be selected from the drop downs in the GUI. 

- The create button will spawn an instance of an actor for the asset selected.
 
- A spawned actor can be selected by clicking on it in the world or by selecting it from the actor table.

- A traffic light actor must belong to a traffic group. Otherwise it will be ignored. A selected traffic light can be assigned a traffic group by selecting the group in the combo box.

- Speed signs and traffic lights also have box triggers for detecting nearby vehicles. The box extents and relative transforms can also be modified.

Controls:

- A,W,S,D keys navigate the camera. 
- Camera is rotated by holding left or right mouse button while moving the mouse.
- The actor can be selected by clicking the left mouse button while the cursor is over the actor object or over its name in the actor table.
- The actor can be deselected by double-clicking the right mouse button.
- The GUI visibility can be toggled with the M key.
- The actor table visibility can be toggled with the T key.
- DELETE key deletes the selected actor.
- P key exits the application.
- The create button spawns actors.
- The export button exports the current data to the XODR file.

Importing/Exporting FBX and XODR:
- Create directories called Import and Export (case-sensitive) and place them in the CarlaEditor folder. 
- Place FBX and XODR files in the Import directory. The files can have any name.
- The modifed XODR file will be created in the Export directory.


Contributing
------------

Please take a look at our [Contribution guidelines][contriblink].

[contriblink]: http://carla.readthedocs.io/en/latest/CONTRIBUTING

F.A.Q.
------

If you run into problems, check our
[FAQ](http://carla.readthedocs.io/en/latest/faq/).

License
-------

CARLA specific code is distributed under MIT License.

CARLA specific assets are distributed under CC-BY License.

Note that UE4 itself follows its own license terms.

















