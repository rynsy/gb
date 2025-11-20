DESKTOP_BUILD_DIR := build
WASM_BUILD_DIR    := build-wasm

.PHONY: all \
        configure-desktop build-desktop run-desktop \
        configure-wasm build-wasm run-wasm \
        test clean

all: run-desktop

configure-desktop:
	cmake -S . -B $(DESKTOP_BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DGB_TARGET_DESKTOP=ON \
		-DGB_TARGET_WASM=OFF \
		-DGB_BUILD_TESTS=ON

build-desktop: configure-desktop
	cmake --build $(DESKTOP_BUILD_DIR) --parallel

run-desktop: build-desktop
	./$(DESKTOP_BUILD_DIR)/gbemu_desktop

configure-wasm:
	emcmake cmake -S . -B $(WASM_BUILD_DIR) \
		-DGB_TARGET_WASM=ON \
		-DGB_TARGET_DESKTOP=OFF \
		-DGB_BUILD_TESTS=OFF \
		-DCMAKE_BUILD_TYPE=Release \
		-DPLATFORM=Web

build-wasm: configure-wasm
	cmake --build $(WASM_BUILD_DIR) --config Release --parallel

run-wasm: build-wasm
	cd $(WASM_BUILD_DIR) && python -m http.server 8000

test: build-desktop
	cd $(DESKTOP_BUILD_DIR) && ctest --output-on-failure

clean:
	rm -rf $(DESKTOP_BUILD_DIR) $(WASM_BUILD_DIR)

status:
	@echo "CI badge:"
	@echo "  https://github.com/rynsy/gb/actions/workflows/ci.yml/badge.svg"
	@echo
	@echo "Pages badge:"
	@echo "  https://github.com/rynsy/gb/actions/workflows/pages.yml/badge.svg"
	@echo
	@echo "Release badge:"
	@echo "  https://github.com/rynsy/gb/actions/workflows/release.yml/badge.svg"

