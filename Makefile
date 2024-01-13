.PHONY: help

SHELL := /bin/bash

CORE ?= esp8266
BOARD ?= $(CORE):$(CORE):nodemcuv2
URLS ?= http://arduino.esp8266.com/stable/package_esp8266com_index.json
ARCH ?= linux64

BUILD_DIR ?= $(PWD)/build
SPIFFS_SIZE ?= 0xFB000
SPIFFS_OFFSET ?= 0x300000
SPIFFS_IMAGE ?= $(BUILD_DIR)/LuckyBox.spiffs.bin
FIRMWARE_IMAGE := $(BUILD_DIR)/LuckyBox.ino.bin

MKSPIFFS_DIR ?= mkspiffs-0.2.3-arduino-$(CORE)-$(ARCH)
MKSPIFFS_URL ?= https://github.com/igrr/mkspiffs/releases/download/0.2.3/mkspiffs-0.2.3-arduino-$(CORE)-$(ARCH).tar.gz

TOOLS := $(PWD)/tools/
CLI := $(TOOLS)/arduino-cli
MKSPIFFS ?= $(TOOLS)/mkspiffs.$(CORE)

CORE_VERSION ?= $(shell $(CLI) core list | grep $(CORE) | cut -d' ' -f2)
CORE_PATH ?= $(shell $(CLI) config dump | grep data | cut -d':' -f2)/packages/$(CORE)

PYTHON ?= python3
ESPTOOL ?= $(PYTHON) $(CORE_PATH)/hardware/$(CORE)/$(CORE_VERSION)/tools/esptool/esptool.py


help:
	@echo "== build tasks"
	@echo ""
	@echo "   clean           - clean build dir ($(BUILD_DIR))"
	@echo "   build           - build firmware and spiffs image"
	@echo ""
	@echo "== esptool tasks"
	@echo ""
	@echo "   erase-flash     - erase flash"
	@echo "   flash           - flash firmware and SPIFFS images"
	@echo "   flash-firmware  - flash firmware only"
	@echo "   flash-web       - flash SPIFFS image (web) data only"
	@echo "   read-mac        - read mac address"
	@echo ""
	@echo "== bootstrap tasks"
	@echo ""
	@echo "   install-tools   - install arduino-cli and mkspiff.$(CORE) to $(TOOLS)/ dir"
	@echo "   install-libs    - install libraries from libraries/*.zip"
	@echo "   bootstrap       - install-tools and install-libs task"
	@echo ""
	@echo "== config: "
	@echo ""
	@echo "   CORE:             $(CORE)"
	@echo "   CORE_VERSION:     $(CORE_VERSION)"
	@echo "   BOARD:            $(BOARD)"
	@echo "   ESPTOOL:          $(ESPTOOL)"
	@echo "   MKSPIFFS:         $(MKSPIFFS)"
	@echo ""
	@echo "== example: "
	@echo ""
	@echo "   $ make CORE=esp8266 CORE_VERSION=2.7.4 BOARD=esp8266:esp8266:generic clean build"
	@echo "   $ make clean build # esp8266:esp8266:nodemcuv2 by default"
	@echo ""

flash:
	$(ESPTOOL) write_flash 0x00000 $(FIRMWARE_IMAGE) $(SPIFFS_OFFSET) $(SPIFFS_IMAGE)

flash-firmwary:
	$(ESPTOOL) write_flash 0x00000 $(FIRMWARE_IMAGE)

flash-web:
	$(ESPTOOL) write_flash $(SPIFFS_OFFSET) $(SPIFFS_IMAGE)

erase-flash:
	$(ESPTOOL) erase_flash

read-mac:
	$(ESPTOOL) read_mac

build:
	@echo "== build firmware... "
	@$(CLI) compile -b $(BOARD) --output-dir build ./
	@echo ""
	@echo -n "== build spiffs image... "
	@$(MKSPIFFS) --create data/ --size $(SPIFFS_SIZE) --block 8192 --page 256 $(SPIFFS_IMAGE) &> /dev/null
	@echo "ok"
	@#@$(MKSPIFFS) --list         --size $(SPIFFS_SIZE) --block 8192 --page 256 $(SPIFFS_IMAGE) | sort
	@echo "== Firmware:      $(FIRMWARE_IMAGE)"
	@echo "== SPIFFS image:  $(SPIFFS_IMAGE)"

clean:
	@rm -fR build

bootstrap: install-tools install-libs

install-libs: install-tools
	@echo "== install $(CORE) core..."
	@$(CLI) --additional-urls $(URLS) core update-index
	@$(CLI) --additional-urls $(URLS) core install $(CORE):$(CORE)
	@echo ""
	@echo "== install libraries..."
	$(eval LIBDIR := $(shell $(CLI) config dump | grep "user: " | cut -d':' -f2))
	@unzip -o libraries/\*.zip -d $(LIBDIR) > /dev/null
	@$(CLI) lib list

install-tools: install-arduino-cli install-mkspiffs

clean-tools:
	rm -f $(TOOLS)/arduino-cli
	rm -f $(TOOLS)/mkspiffs.*

install-mkspiffs:
	@test -f $(MKSPIFFS) || ( \
		wget $(MKSPIFFS_URL) -O /tmp/$(MKSPIFFS_DIR).tar.gz && \
		tar xvf /tmp/$(MKSPIFFS_DIR).tar.gz -C /tmp/ && \
		mv /tmp/$(MKSPIFFS_DIR)/mkspiffs $(MKSPIFFS) \
	)

install-arduino-cli:
	@test -f $(CLI) || (sh $(TOOLS)/arduino-cli-install-linux.sh)
