WATCHDOG_PETTER_VERSION = 1.0.0
WATCHDOG_PETTER_SITE = ./package/watchdog_petter/src
WATCHDOG_PETTER_SITE_METHOD = local

define WATCHDOG_PETTER_BUILD_CMDS
        $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D) all
endef

define WATCHDOG_PETTER_INSTALL_TARGET_CMDS
        $(INSTALL) -D -m 0755 $(@D)/watchdog_petter $(TARGET_DIR)/bin
endef

$(eval $(generic-package))
