find_package(QtWaylandScanner REQUIRED)

ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri-decoration.xml"
    BASENAME "liri-decoration")
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/gtk-shell.xml"
    BASENAME "gtk-shell")
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/server-decoration.xml"
    BASENAME "server-decoration")
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-foreign-toplevel-management-unstable-v1.xml"
    BASENAME "wlr-foreign-toplevel-management-unstable-v1")
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-layer-shell-unstable-v1.xml"
    BASENAME "wlr-layer-shell-unstable-v1")
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-output-management-unstable-v1.xml"
    BASENAME "wlr-output-management-unstable-v1")
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri-shell.xml"
    BASENAME "liri-shell")

if(IS_ABSOLUTE "${INSTALL_LIBEXECDIR}")
    set(LIBEXECDIR "${INSTALL_LIBEXECDIR}")
else()
    set(LIBEXECDIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIBEXECDIR}")
endif()

liri_add_module(WaylandServer
    DESCRIPTION
        "Wayland server extensions"
    SOURCES
        gtkshell.cpp
        gtkshell.h
        gtkshell_p.h
        kdeserverdecoration.cpp
        kdeserverdecoration.h
        kdeserverdecoration_p.h
        liridecoration.cpp
        liridecoration.h
        liridecoration_p.h
        waylandlirishell.cpp
        waylandlirishell.h
        waylandlirishell_p.h
        waylandwlrforeigntoplevelmanagementv1.cpp
        waylandwlrforeigntoplevelmanagementv1.h
        waylandwlrforeigntoplevelmanagementv1_p.h
        waylandwlrlayershellv1.cpp
        waylandwlrlayershellv1.h
        waylandwlrlayershellv1_p.h
        waylandwlroutputmanagerv1.cpp
        waylandwlroutputmanagerv1.h
        waylandwlroutputmanagerv1_p.h
        logging.cpp
        logging_p.h
        ${SOURCES}
    FORWARDING_HEADERS
        GtkShell
        KdeServerDecoration
        LiriDecoration
        WaylandLiriShell
        WaylandWlrForeignToplevelManagementV1
        WaylandWlrLayerShellV1
        WaylandWlrOutputManagerV1
    PRIVATE_HEADERS
        gtkshell_p.h
        waylandlirishell_p.h
        waylandwlrforeigntoplevelmanagementv1_p.h
        waylandwlrlayershellv1_p.h
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-gtk-shell.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-gtk-shell-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-server-decoration.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-server-decoration-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-liri-decoration.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-decoration-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-liri-shell.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-shell-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-wlr-foreign-toplevel-management-unstable-v1.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-wlr-foreign-toplevel-management-unstable-v1-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-wlr-layer-shell-unstable-v1.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-wlr-layer-shell-unstable-v1-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-wlr-output-management-unstable-v1.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-wlr-output-management-unstable-v1-server-protocol.h"
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
        QT_WAYLAND_COMPOSITOR_QUICK
        INSTALL_LIBEXECDIR="${LIBEXECDIR}"
    PUBLIC_LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Quick
        Qt5::WaylandCompositor
    LIBRARIES
        Wayland::Server
    PKGCONFIG_DEPENDENCIES
        Qt5Core
        Qt5Gui
        Qt5WaylandCompositor
)
