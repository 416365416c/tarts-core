# Add more folders to ship with the application, here
folder_01.source = qml/TouchRTS
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=
QT += quick qml

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    game.cpp \
    map.cpp \
    player.cpp \
    doodad.cpp \
    buildable.cpp \
    combatant.cpp \
    movingunit.cpp \
    waypoint.cpp

HEADERS += \
    game.h \
    map.h \
    player.h \
    doodad.h \
    buildable.h \
    combatant.h \
    movingunit.h \
    waypoint.h
