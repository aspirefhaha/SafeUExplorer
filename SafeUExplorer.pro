#-------------------------------------------------
#
# Project created by QtCreator 2020-11-12T22:44:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SafeUExplorer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = udisk.ico

INCLUDEPATH += ../exfat_tf/libexfat

LIBS += -lSdCryptoStor

SOURCES +=  \
        CGlobalModel.cpp \
        CLocalModel.cpp \
        CommonTools.cpp \
        main.cpp \
        CSafeUExplorer.cpp \
        ../exfat_tf/libexfat/cluster.c \
        ../exfat_tf/libexfat/io.c \
        ../exfat_tf/libexfat/log.c \
        ../exfat_tf/libexfat/lookup.c \
        ../exfat_tf/libexfat/mount.c \
        ../exfat_tf/libexfat/node.c \
        ../exfat_tf/libexfat/repair.c \
        ../exfat_tf/libexfat/time.c \
        ../exfat_tf/libexfat/utf.c \
        ../exfat_tf/libexfat/utils.c \
        ../exfat_tf/mkfs/fat.c \
        ../exfat_tf/mkfs/mkfs.c \
        ../exfat_tf/mkfs/mkexfat.c \
        ../exfat_tf/mkfs/rootdir.c \
        ../exfat_tf/mkfs/uct.c \
        ../exfat_tf/mkfs/uctc.c \
        ../exfat_tf/mkfs/vbr.c \
        ../exfat_tf/mkfs/cbm.c

HEADERS += \
        CFSPrivate.h \
        CGlobalModel.h \
        CLocalModel.h \
        CSafeUExplorer.h \
        CommonTools.h \
        FsItemType.h

FORMS += \
        CSafeUExplorer.ui

RESOURCES += \
    safeuexplorer.qrc

TRANSLATIONS += SafeUExplorer.ts
