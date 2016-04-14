# fcgiLearnings
make a rapid prototyping for fcgi application using dbus to fetch  network manager related settings

* packages installed
sudo apt-get install libfcgi-dev
sudo apt-get install spawn-fcgi
sudo apt-get install nginx
sudo apt-get install curl
sudo nginx -c nginx.conf
sudo apt-get install g++
sudo apt-get install libdbus-c++-bin
sudo apt-get install libdbus-c++-dev

* Building
dbusxx-introspect --system /org/freedesktop/NetworkManager org.freedesktop.NetworkManager > ~/GitHub/fcgiLearnings/NetworkManagerIF.xml

dbusxx-xml2cpp NetworkManagerIF.xml –proxy=NetworkProxy.h

g++ Application.cpp -lfcgi++ -lfcgi -ldbus-c++-1 -o fcgiapp -I /usr/include/dbus-c++-1 

* Test and Run
sudo nginx -c nginx.conf
spawn-fcgi -p 8000 -n fcgiapp
open browser and key-in http://localhost


