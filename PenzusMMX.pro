APP_NAME = PenzusMMX

CONFIG += qt warn_on cascades10

RESOURCES += app.qrc
include(config.pri)
LIBS   +=  -lbb -lbbsystem -lbbdata  -lbbcascadespickers -lbbplatform -lbbutilityi18n -lbbdevice

INCLUDEPATH  += src\lib
INCLUDEPATH  += src\media


DEFINES += HAVE_CONFIG_H NDEBUG 
