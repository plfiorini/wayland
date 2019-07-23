/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2019 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QtGui/QGuiApplication>
#include <QtGui/QPlatformSurfaceEvent>
#include <QtGui/QWindow>
#include <QtGui/qpa/qplatformnativeinterface.h>

#include "lirishell_p.h"

static inline struct ::wl_surface *getWlSurface(QWindow *window)
{
    void *surface = QGuiApplication::platformNativeInterface()->nativeResourceForWindow("surface", window);
    return static_cast<struct ::wl_surface *>(surface);
}

// LiriShellPrivate

LiriShellPrivate::LiriShellPrivate(LiriShell *qq)
    : QtWayland::liri_shell()
    , q_ptr(qq)
{
}

void LiriShellPrivate::setCursorShape(QWindow *window, LiriShell::GrabCursor cursor)
{
    QCursor newCursor;

    switch (cursor) {
    case LiriShell::ArrowGrabCursor:
        newCursor.setShape(Qt::ArrowCursor);
        break;
    case LiriShell::ResizeTopGrabCursor:
    case LiriShell::ResizeBottomGrabCursor:
        newCursor.setShape(Qt::SizeVerCursor);
        break;
    case LiriShell::ResizeLeftGrabCursor:
    case LiriShell::ResizeRightGrabCursor:
        newCursor.setShape(Qt::SizeHorCursor);
        break;
    case LiriShell::ResizeTopLeftGrabCursor:
    case LiriShell::ResizeBottomRightGrabCursor:
        newCursor.setShape(Qt::SizeFDiagCursor);
        break;
    case LiriShell::ResizeTopRightGrabCursor:
    case LiriShell::ResizeBottomLeftGrabCursor:
        newCursor.setShape(Qt::SizeBDiagCursor);
        break;
    case LiriShell::MoveGrabCursor:
        newCursor.setShape(Qt::DragMoveCursor);
        break;
    case LiriShell::BusyGrabCursor:
        newCursor.setShape(Qt::BusyCursor);
        break;
    default:
        newCursor.setShape(Qt::BlankCursor);
        break;
    }

    window->setCursor(newCursor);
}

void LiriShellPrivate::liri_shell_grab_cursor(uint32_t cursor)
{
    Q_Q(LiriShell);

    auto grabCursor = static_cast<LiriShell::GrabCursor>(cursor);

    Q_EMIT q->cursorChangeRequested(grabCursor);

    if (grabWindow)
        setCursorShape(grabWindow, grabCursor);
}

// LiriShell

LiriShell::LiriShell()
    : QWaylandClientExtensionTemplate(1)
    , d_ptr(new LiriShellPrivate(this))
{
}

LiriShell::~LiriShell()
{
    delete d_ptr;
}

void LiriShell::init(wl_registry *registry, int id, int version)
{
    Q_D(LiriShell);
    d->init(registry, id, version);
}

void LiriShell::registerGrabSurface(QWindow *window)
{
    Q_D(LiriShell);

    d->grabWindow = window;
    d->grabWindow->setFlags(Qt::BypassWindowManagerHint);
    d->grabWindow->create();
    d->grabWindow->setCursor(QCursor(Qt::ArrowCursor));

    if (isActive())
        d->set_grab_surface(getWlSurface(d->grabWindow));
}

void LiriShell::sendReady()
{
    Q_D(LiriShell);
    d->ready();
}

const wl_interface *LiriShell::interface()
{
    return QtWayland::liri_shell::interface();
}
