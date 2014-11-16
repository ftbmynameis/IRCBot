Welcome to IRCBot
======================

This is actually less of a bot and more of a raw connection to IRC with some console stuff to receive and send messages. The code provides only the most basic functionality as I
didn't continue developement at some point.
The connection has been coded for euirc and I have notices issues with other networks (quakenet for example) which may be due to message endings and similiar stuff which varies a litle
from server to server.

Dependencies
=======================
There is only a dependency of WinSocket 2 for networking purposes. The project has been coded before c++11 so also threading is causing a Windows dependency (but is not requiring any libs).

License
=======================
Do whatever you want with it.. Not like I am worried about someone making thousands of bugs with this. (if you do feel free to share it with me :D)

What's next
======================
Maybe I will rewrite the code using paradigms of the new c++11 standard and possibly get rid of all the platform dependency stuff. 
Thing is having 1000 IRC libs is quite pointless and it was just a fun phase were everyone started writing their own bot (or tried to :D)