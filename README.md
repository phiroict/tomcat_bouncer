# Tomcat bouncer

Monitors a directory, passed as a parameter, and responses by 
restarting Tomcat by a `sudo systemctl restart tomcat.service`
command. 

This is c application that is build from the Makefile using `gcc` if you 
do not have or not on linux, use the container. 
Commands are in the `Makefile`. 
