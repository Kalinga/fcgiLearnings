# fcgiLearnings
make a rapid prototyping for fcgi application using dbus to fetch  network manager related settings

* packages installed

sudo apt-get install -y libfcgi-dev spawn-fcgi

sudo apt-get install -y  nginx curl

sudo apt-get install -y g++

sudo apt-get install -y libdbus-c++-dev libdbus-c++-bin

* Generation

dbusxx-introspect --system /org/freedesktop/NetworkManager org.freedesktop.NetworkManager > ~/GitHub/fcgiLearnings/NetworkManagerIF.xml

dbusxx-xml2cpp NetworkManagerIF.xml –proxy=NetworkProxy.h

* Building

g++ -std=c++11 Application.cpp -lfcgi++ -lfcgi -ldbus-c++-1 -o fcgiapp -I /usr/include/dbus-c++-1 -I cereal/include 


* Test and Run

sudo cp nginx.conf /usr/share/nginx/

[Kill the port owner by 'sudo fuser -k 80/tcp' if error occurs in 'sudo nginx -c nginx.conf']
sudo fuser -k 80/tcp

sudo nginx -c nginx.conf

spawn-fcgi -p 8000 -n fcgiapp

open browser and key-in http://localhost


