SHELL := /bin/bash

PROJECT      := SpoonSaveEditor
BIN_NAME     := spoon_save_editor
CT_WORKDIR   := /src
CONTAINER    := spoon-builder
COMP_DIR     := builds/comp
APPDIR       := $(COMP_DIR)/AppDir
HOST_APPTOOL := $(COMP_DIR)/appimagetool-x86_64.AppImage
APPTOOL_URL  := https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage

.PHONY: all linux linux-debug alpine alpine-debug windows windows-debug fetch-appimagetool clean

all: linux

builds/release builds/debug $(COMP_DIR):
	mkdir -p $@

fetch-appimagetool: | $(COMP_DIR)
	@if [ ! -x "$(HOST_APPTOOL)" ]; then \
		echo "==> Fetching statically-linked appimagetool into $(COMP_DIR)..."; \
		(curl -fsSL -o "$(HOST_APPTOOL)" "$(APPTOOL_URL)" || wget -qO "$(HOST_APPTOOL)" "$(APPTOOL_URL)"); \
		chmod +x "$(HOST_APPTOOL)"; \
	fi

# ==============================================================================
# LINUX (glibc)
# ==============================================================================

define BUILD_LINUX_APPDIR_SCRIPT
#!/bin/sh
set -eux

export DEBIAN_FRONTEND=noninteractive
apt-get update
apt-get install -y build-essential cmake qt6-base-dev qt6-base-private-dev file libgl1-mesa-dev

BUILD_DIR="build/linux-release"
AD="$(APPDIR)"

cmake -S . -B "$$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
cmake --build "$$BUILD_DIR" --parallel "$$(nproc)"

strip "$$BUILD_DIR/$(BIN_NAME)" || true

rm -rf "$$AD"
mkdir -p "$$AD/usr/bin" "$$AD/usr/lib" "$$AD/usr/plugins/platforms" \
         "$$AD/usr/share/applications" "$$AD/usr/share/icons/hicolor/256x256/apps"

cp "$$BUILD_DIR/$(BIN_NAME)" "$$AD/usr/bin/"

ldd "$$AD/usr/bin/$(BIN_NAME)" | awk '/=> \// {print $$3} /^\// {print $$1}' | sort -u | while read -r lib; do
    [ -n "$$lib" ] || continue
    case "$$lib" in */libc.so.*|*/libm.so.*|*/libpthread.so.*|*/libdl.so.*|*/librt.so.*|*/ld-linux-*.so.*|*/libresolv.so.*|*/libz.so.*|*/libX*.so.*|*/libxcb*.so.*|*/libxkbcommon*.so.*|*/libfontconfig.so.*|*/libfreetype.so.*|*/libexpat.so.*|*/libharfbuzz.so.*|*/libglib-2.0.so.*|*/libgobject-2.0.so.*|*/libstdc++.so.*|*/libgcc_s.so.*|*/libGL*.so.*|*/libEGL*.so.*|*/libdrm.so.*|*/libwayland-*.so.*) continue ;; esac
    mkdir -p "$$AD$$(dirname "$$lib")"
    cp -L "$$lib" "$$AD$$lib"
done

if [ -d /usr/lib/x86_64-linux-gnu/qt6/plugins/platforms ]; then
    cp /usr/lib/x86_64-linux-gnu/qt6/plugins/platforms/*.so "$$AD/usr/plugins/platforms/" 2>/dev/null || true
    for p in "$$AD"/usr/plugins/platforms/*.so; do
        [ -f "$$p" ] || continue
        ldd "$$p" 2>/dev/null | awk '/=> \// {print $$3} /^\// {print $$1}' | sort -u | while read -r lib; do
            [ -n "$$lib" ] || continue
            [ -f "$$AD$$lib" ] && continue
            case "$$lib" in */libc.so.*|*/libm.so.*|*/libpthread.so.*|*/libdl.so.*|*/librt.so.*|*/ld-linux-*.so.*|*/libresolv.so.*|*/libz.so.*|*/libX*.so.*|*/libxcb*.so.*|*/libxkbcommon*.so.*|*/libfontconfig.so.*|*/libfreetype.so.*|*/libexpat.so.*|*/libharfbuzz.so.*|*/libglib-2.0.so.*|*/libgobject-2.0.so.*|*/libstdc++.so.*|*/libgcc_s.so.*|*/libGL*.so.*|*/libEGL*.so.*|*/libdrm.so.*|*/libwayland-*.so.*) continue ;; esac
            mkdir -p "$$AD$$(dirname "$$lib")"
            cp -L "$$lib" "$$AD$$lib" 2>/dev/null || true
        done
    done
fi

cat > "$$AD/usr/share/applications/$(BIN_NAME).desktop" <<'DESK'
[Desktop Entry]
Type=Application
Name=Spoon Save Editor
Comment=Splatoon 1 save file editor
Exec=spoon_save_editor
Icon=spoon_save_editor
Categories=Game;Utility;
Terminal=false
DESK
chmod 644 "$$AD/usr/share/applications/$(BIN_NAME).desktop"

cp images/icon.png "$$AD/usr/share/icons/hicolor/256x256/apps/$(BIN_NAME).png" 2>/dev/null || true

ln -sf "usr/share/applications/$(BIN_NAME).desktop" "$$AD/$(BIN_NAME).desktop"
ln -sf "usr/share/icons/hicolor/256x256/apps/$(BIN_NAME).png" "$$AD/$(BIN_NAME).png"
ln -sf "usr/share/icons/hicolor/256x256/apps/$(BIN_NAME).png" "$$AD/.DirIcon"

cat > "$$AD/AppRun" <<'RUN'
#!/bin/sh
HERE="$$(dirname "$$(readlink -f "$$0")")"
export LD_LIBRARY_PATH="$$HERE/usr/lib:$$HERE/usr/lib/x86_64-linux-gnu$${LD_LIBRARY_PATH:+:$$LD_LIBRARY_PATH}"
export QT_PLUGIN_PATH="$$HERE/usr/plugins"
export QT_QPA_PLATFORM_PLUGIN_PATH="$$HERE/usr/plugins/platforms"
exec "$$HERE/usr/bin/spoon_save_editor" "$$@"
RUN
chmod 755 "$$AD/AppRun"
endef
export BUILD_LINUX_APPDIR_SCRIPT

linux: builds/release fetch-appimagetool | $(COMP_DIR)
	@echo "==> Building Linux Release AppImage..."
	@echo "$$BUILD_LINUX_APPDIR_SCRIPT" > $(COMP_DIR)/.tmp-linux-appdir.sh
	podman run -i --rm \
		--name "$(CONTAINER)-linux-appdir" \
		-v "$(CURDIR):$(CT_WORKDIR):Z" \
		-w "$(CT_WORKDIR)" \
		debian:bookworm /bin/sh $(COMP_DIR)/.tmp-linux-appdir.sh
	@rm -f $(COMP_DIR)/.tmp-linux-appdir.sh
	./$(HOST_APPTOOL) --appimage-extract-and-run "$(APPDIR)" "builds/release/$(PROJECT)-glibc-x86_64.AppImage"
	@echo "==> Release AppImage built at builds/release/$(PROJECT)-glibc-x86_64.AppImage"
	@echo "==> Cleaning up AppDir..."
	@rm -rf $(APPDIR)

define BUILD_LINUX_DEBUG_SCRIPT
#!/bin/sh
set -eux
export DEBIAN_FRONTEND=noninteractive
apt-get update
apt-get install -y build-essential cmake qt6-base-dev qt6-base-private-dev file libgl1-mesa-dev

BUILD_DIR="build/linux-debug"
cmake -S . -B "$$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$$BUILD_DIR" --parallel "$$(nproc)"

mkdir -p builds/debug
cp "$$BUILD_DIR/$(BIN_NAME)" builds/debug/$(BIN_NAME)_linux
endef
export BUILD_LINUX_DEBUG_SCRIPT

linux-debug: builds/debug | $(COMP_DIR)
	@echo "==> Building raw Linux Debug binary..."
	@echo "$$BUILD_LINUX_DEBUG_SCRIPT" > $(COMP_DIR)/.tmp-linux-dbg.sh
	podman run -i --rm \
		--name "$(CONTAINER)-linux-dbg" \
		-v "$(CURDIR):$(CT_WORKDIR):Z" \
		-w "$(CT_WORKDIR)" \
		debian:bookworm /bin/sh $(COMP_DIR)/.tmp-linux-dbg.sh
	@rm -f $(COMP_DIR)/.tmp-linux-dbg.sh

# ==============================================================================
# ALPINE (musl)
# ==============================================================================

define BUILD_ALPINE_APPDIR_SCRIPT
#!/bin/sh
set -eux

apk update
apk add --no-cache build-base cmake qt6-qtbase-dev

BUILD_DIR="build/alpine-release"
AD="$(APPDIR)"

cmake -S . -B "$$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$$BUILD_DIR" --parallel "$$(nproc)"

strip "$$BUILD_DIR/$(BIN_NAME)" || true

rm -rf "$$AD"
mkdir -p "$$AD/usr/bin" "$$AD/usr/lib" "$$AD/usr/plugins/platforms" \
         "$$AD/usr/share/applications" "$$AD/usr/share/icons/hicolor/256x256/apps"

cp "$$BUILD_DIR/$(BIN_NAME)" "$$AD/usr/bin/"

ldd "$$AD/usr/bin/$(BIN_NAME)" | awk '/=> \// {print $$3} /^\// {print $$1}' | sort -u | while read -r lib; do
    [ -n "$$lib" ] || continue
    case "$$lib" in */libc.so.*|*/ld-musl-*.so.*|*/libm.so.*|*/libpthread.so.*|*/libdl.so.*|*/librt.so.*|*/libz.so.*|*/libX*.so.*|*/libxcb*.so.*|*/libxkbcommon*.so.*|*/libfontconfig.so.*|*/libfreetype.so.*|*/libexpat.so.*|*/libharfbuzz.so.*|*/libglib-2.0.so.*|*/libgobject-2.0.so.*|*/libstdc++.so.*|*/libgcc_s.so.*|*/libGL*.so.*|*/libEGL*.so.*|*/libdrm.so.*|*/libwayland-*.so.*) continue ;; esac
    mkdir -p "$$AD$$(dirname "$$lib")"
    cp -L "$$lib" "$$AD$$lib"
done

if [ -d /usr/lib/qt6/plugins/platforms ]; then
    cp /usr/lib/qt6/plugins/platforms/*.so "$$AD/usr/plugins/platforms/" 2>/dev/null || true
    for p in "$$AD"/usr/plugins/platforms/*.so; do
        [ -f "$$p" ] || continue
        ldd "$$p" 2>/dev/null | awk '/=> \// {print $$3} /^\// {print $$1}' | sort -u | while read -r lib; do
            [ -n "$$lib" ] || continue
            [ -f "$$AD$$lib" ] && continue
            case "$$lib" in */libc.so.*|*/ld-musl-*.so.*|*/libm.so.*|*/libpthread.so.*|*/libdl.so.*|*/librt.so.*|*/libz.so.*|*/libX*.so.*|*/libxcb*.so.*|*/libxkbcommon*.so.*|*/libfontconfig.so.*|*/libfreetype.so.*|*/libexpat.so.*|*/libharfbuzz.so.*|*/libglib-2.0.so.*|*/libgobject-2.0.so.*|*/libstdc++.so.*|*/libgcc_s.so.*|*/libGL*.so.*|*/libEGL*.so.*|*/libdrm.so.*|*/libwayland-*.so.*) continue ;; esac
            mkdir -p "$$AD$$(dirname "$$lib")"
            cp -L "$$lib" "$$AD$$lib" 2>/dev/null || true
        done
    done
fi

cat > "$$AD/usr/share/applications/$(BIN_NAME).desktop" <<'DESK'
[Desktop Entry]
Type=Application
Name=Spoon Save Editor
Comment=Splatoon 1 save file editor
Exec=spoon_save_editor
Icon=spoon_save_editor
Categories=Game;Utility;
Terminal=false
DESK
chmod 644 "$$AD/usr/share/applications/$(BIN_NAME).desktop"

cp images/icon.png "$$AD/usr/share/icons/hicolor/256x256/apps/$(BIN_NAME).png" 2>/dev/null || true

ln -sf "usr/share/applications/$(BIN_NAME).desktop" "$$AD/$(BIN_NAME).desktop"
ln -sf "usr/share/icons/hicolor/256x256/apps/$(BIN_NAME).png" "$$AD/$(BIN_NAME).png"
ln -sf "usr/share/icons/hicolor/256x256/apps/$(BIN_NAME).png" "$$AD/.DirIcon"

cat > "$$AD/AppRun" <<'RUN'
#!/bin/sh
HERE="$$(dirname "$$(readlink -f "$$0")")"
export LD_LIBRARY_PATH="$$HERE/usr/lib:$$HERE/lib$${LD_LIBRARY_PATH:+:$$LD_LIBRARY_PATH}"
export QT_PLUGIN_PATH="$$HERE/usr/plugins"
export QT_QPA_PLATFORM_PLUGIN_PATH="$$HERE/usr/plugins/platforms"
exec "$$HERE/usr/bin/spoon_save_editor" "$$@"
RUN
chmod 755 "$$AD/AppRun"
endef
export BUILD_ALPINE_APPDIR_SCRIPT

alpine: builds/release fetch-appimagetool | $(COMP_DIR)
	@echo "==> Building Alpine Release AppImage..."
	@echo "$$BUILD_ALPINE_APPDIR_SCRIPT" > $(COMP_DIR)/.tmp-alpine-appdir.sh
	podman run -i --rm \
		--name "$(CONTAINER)-alpine-appdir" \
		-v "$(CURDIR):$(CT_WORKDIR):Z" \
		-w "$(CT_WORKDIR)" \
		alpine:latest /bin/sh $(COMP_DIR)/.tmp-alpine-appdir.sh
	@rm -f $(COMP_DIR)/.tmp-alpine-appdir.sh
	./$(HOST_APPTOOL) --appimage-extract-and-run "$(APPDIR)" "builds/release/$(PROJECT)-musl-x86_64.AppImage"
	@echo "==> Release AppImage built at builds/release/$(PROJECT)-musl-x86_64.AppImage"
	@echo "==> Cleaning up AppDir..."
	@rm -rf $(APPDIR)

define BUILD_ALPINE_DEBUG_SCRIPT
#!/bin/sh
set -eux
apk update
apk add --no-cache build-base cmake qt6-qtbase-dev

BUILD_DIR="build/alpine-debug"
cmake -S . -B "$$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$$BUILD_DIR" --parallel "$$(nproc)"

mkdir -p builds/debug
cp "$$BUILD_DIR/$(BIN_NAME)" builds/debug/$(BIN_NAME)_alpine
endef
export BUILD_ALPINE_DEBUG_SCRIPT

alpine-debug: builds/debug | $(COMP_DIR)
	@echo "==> Building Alpine Debug binary (musl)..."
	@echo "$$BUILD_ALPINE_DEBUG_SCRIPT" > $(COMP_DIR)/.tmp-alpine-dbg.sh
	podman run -i --rm \
		--name "$(CONTAINER)-alpine-dbg" \
		-v "$(CURDIR):$(CT_WORKDIR):Z" \
		-w "$(CT_WORKDIR)" \
		alpine:latest /bin/sh $(COMP_DIR)/.tmp-alpine-dbg.sh
	@rm -f $(COMP_DIR)/.tmp-alpine-dbg.sh

# ==============================================================================
# WINDOWS (Native MSYS2/CMake)
# ==============================================================================

# Detect active MSYS2 environment directory dynamically (falls back to /mingw64)
MSYS_ENV_PREFIX := $(or $(MINGW_PREFIX),/mingw64)

windows: builds/release
	@echo "==> Building Windows Release binary natively..."
	cmake -S . -B "build/win-release" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
	cmake --build "build/win-release" --parallel
	cp "build/win-release/$(BIN_NAME).exe" "builds/release/$(BIN_NAME).exe"
	windeployqt "builds/release/$(BIN_NAME).exe"
	cp $(MSYS_ENV_PREFIX)/bin/*.dll "builds/release/" 2>/dev/null || true

windows-debug: builds/debug
	@echo "==> Building Windows Debug binary natively..."
	cmake -S . -B "build/win-debug" -DCMAKE_BUILD_TYPE=Debug
	cmake --build "build/win-debug" --parallel
	cp "build/win-debug/$(BIN_NAME).exe" "builds/debug/$(BIN_NAME).exe"

# ==============================================================================
# CLEAN
# ==============================================================================
clean:
	@echo "==> Cleaning build outputs..."
	rm -rf build builds
