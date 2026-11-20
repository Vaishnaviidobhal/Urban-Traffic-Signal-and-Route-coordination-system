CC ?= gcc
ifeq ($(OS),Windows_NT)
    CC = gcc
endif
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Isrc -Igui
LDFLAGS = -lm

SRC_DIR = src
GUI_DIR = gui

COMMON_SRCS = $(SRC_DIR)/traffic_common.c \
              $(SRC_DIR)/traffic_network.c \
              $(SRC_DIR)/traffic_signal.c \
              $(SRC_DIR)/traffic_routing.c \
              $(SRC_DIR)/traffic_analytics.c

CLI_SRCS = main.c $(COMMON_SRCS)

GUI_MOD_SRCS = $(GUI_DIR)/gui_common.c \
               $(GUI_DIR)/gui_routing.c \
               $(GUI_DIR)/gui_monitor.c \
               $(GUI_DIR)/gui_reroute.c \
               $(GUI_DIR)/gui_coordination.c \
               $(GUI_DIR)/gui_analytics.c

GUI_SRCS = gui.c $(GUI_MOD_SRCS) $(COMMON_SRCS)

ifeq ($(OS),Windows_NT)
    TARGET = traffic.exe
    GUI_TARGET = traffic_gui.exe
    GUI_FLAGS = -mwindows
    RM = del /Q /F 2>NUL || true
    RUN = .\\traffic.exe
else
    TARGET = traffic
    GUI_TARGET = traffic_gui.exe
    GUI_CC ?= x86_64-w64-mingw32-gcc
    GUI_FLAGS = -mwindows
    RM = rm -f
    RUN = ./traffic
endif

.PHONY: all gui run clean

all: $(TARGET)

$(TARGET): $(CLI_SRCS)
	$(CC) $(CFLAGS) $(CLI_SRCS) $(LDFLAGS) -o $(TARGET)

gui: $(GUI_SRCS)
ifeq ($(OS),Windows_NT)
	$(CC) $(CFLAGS) $(GUI_SRCS) $(LDFLAGS) -o $(GUI_TARGET) $(GUI_FLAGS)
else
	$(GUI_CC) $(CFLAGS) $(GUI_SRCS) $(LDFLAGS) -o $(GUI_TARGET) $(GUI_FLAGS)
endif

run: $(TARGET)
	$(RUN)

clean:
	$(RM) $(TARGET) traffic.exe traffic_gui.exe $(SRC_DIR)/traffic_gui.exe $(GUI_DIR)/traffic_gui.exe traffic_report.csv
