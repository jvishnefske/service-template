# Makefile for service-template
# C++ project built with CMake and vcpkg

VCPKG_ROOT ?= $(HOME)/vcpkg
BUILD_DIR := build
BUILD_TYPE ?= Release
CMAKE_TOOLCHAIN := $(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake

.PHONY: all deps build test coverage clean help

all: build

## deps: Install dependencies via vcpkg
deps:
	@if [ ! -f "$(VCPKG_ROOT)/vcpkg" ]; then \
		echo "Error: vcpkg not found at $(VCPKG_ROOT)"; \
		echo "Set VCPKG_ROOT or install vcpkg to ~/vcpkg"; \
		exit 1; \
	fi
	$(VCPKG_ROOT)/vcpkg install

## build: Configure and build the project
build:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_TOOLCHAIN_FILE=$(CMAKE_TOOLCHAIN) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cmake --build $(BUILD_DIR) --config $(BUILD_TYPE)

## test: Run unit tests
test: build
	cd $(BUILD_DIR)/test && ctest -C $(BUILD_TYPE) --output-on-failure

## coverage: Build with coverage and generate report
coverage:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_TOOLCHAIN_FILE=$(CMAKE_TOOLCHAIN) \
		-DCMAKE_CXX_FLAGS="--coverage -fprofile-arcs -ftest-coverage" \
		-DCMAKE_EXE_LINKER_FLAGS="--coverage"
	cmake --build $(BUILD_DIR) --config Debug
	cd $(BUILD_DIR)/test && ctest -C Debug --output-on-failure || true
	@mkdir -p $(BUILD_DIR)/coverage
	lcov --capture --directory $(BUILD_DIR) --output-file $(BUILD_DIR)/coverage/coverage.info \
		--ignore-errors mismatch
	lcov --remove $(BUILD_DIR)/coverage/coverage.info '/usr/*' '*/vcpkg_installed/*' '*/test/*' \
		--output-file $(BUILD_DIR)/coverage/coverage.info --ignore-errors unused
	genhtml $(BUILD_DIR)/coverage/coverage.info --output-directory $(BUILD_DIR)/coverage/html
	@echo "Coverage report: $(BUILD_DIR)/coverage/html/index.html"

## clean: Remove build artifacts
clean:
	rm -rf $(BUILD_DIR)

## help: Show this help
help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@grep -E '^## ' $(MAKEFILE_LIST) | sed 's/## /  /'
