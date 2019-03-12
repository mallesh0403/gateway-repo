include $(TOPDIR)/rules.mk
# Adding extra line
# Name, version and release number
# The name and version of your package are used to define the variable to point to the build directory of your package: $(PKG_BUILD_DIR)
PKG_NAME:=healthmonitor
PKG_VERSION:=1.0
PKG_RELEASE:=1

# Source settings (i.e. where to find the source codes)
# This is a custom variable, used below
SOURCE_DIR:=/v18.06.2.git/healthmonitor

include $(INCLUDE_DIR)/package.mk
TARGET_CC = mips-openwrt-linux-musl-gcc -static
TARGET_INCLUDE_PATH = -I/v18.06.2.git/build_dir/target-mips_24kc_musl/json-c-0.12.1/ -I/v18.06.2.git/build_dir/target-mips_24kc_musl/curl-7.64.0/include/ -I/v18.06.2.git/build_dir/target-mips_24kc_musl/libconfig-1.7.2/lib -I/v18.06.2.git/staging_dir/toolchain-mips_24kc_gcc-7.4.0_musl/include/
TARGET_LIBS = -lc -lcurl  -ljson-c -lssl -lmbedtls -lconfig -lmbedcrypto -lmbedx509 
TARGET_LDFLAGS = -L/v18.06.2.git/staging_dir/target-mips_24kc_musl/usr/lib/

# Package definition; instructs on how and where our package will appear in the overall configuration menu ('make menuconfig')
define Package/healthmonitor
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=read configdata&opcommands from individual files..!
	
endef
# Package description; a more verbose description on what our package does
define Package/healthmonitor/description
  A simple "read configdata&opcommands from individual files..!" -application.
endef

# Package preparation instructions; create the build directory and copy the source code. 
# The last command is necessary to ensure our preparation instructions remain compatible with the patching system.
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
	$(Build/Patch)
endef

# Package build instructions; invoke the target-specific compiler to first compile the source file, and then to link the file into the final executable
define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_INCLUDE_PATH) -c $(PKG_BUILD_DIR)/configdata.c -o $(PKG_BUILD_DIR)/configdata.o -c
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_INCLUDE_PATH) -c $(PKG_BUILD_DIR)/opcommands.c -o $(PKG_BUILD_DIR)/opcommands.o -c
	$(TARGET_CC) $(TARGET_LDFLAGS) $(PKG_BUILD_DIR)/configdata.o $(PKG_BUILD_DIR)/opcommands.o $(TARGET_LIBS) -o $(PKG_BUILD_DIR)/healthmonitor
endef

# Package install instructions; create a directory inside the package to hold our executable, and then copy the executable we built previously into the folder
define Package/healthmonitor/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/healthmonitor $(1)/usr/bin
endef

# This command is always the last, it uses the definitions and variables we give above in order to get the job done
$(eval $(call BuildPackage,healthmonitor))

