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

#include "logging_p.h"
#include "waylandwlrforeigntoplevelmanagementv1_p.h"

WaylandWlrForeignToplevelManagerV1Private::WaylandWlrForeignToplevelManagerV1Private(WaylandWlrForeignToplevelManagerV1 *self)
    : QtWaylandServer::zwlr_foreign_toplevel_manager_v1()
    , q_ptr(self)
{
}

void WaylandWlrForeignToplevelManagerV1Private::zwlr_foreign_toplevel_manager_v1_bind_resource(Resource *resource)
{
    Q_Q(WaylandWlrForeignToplevelManagerV1);

    for (auto toplevel : qAsConst(toplevels)) {
        auto d = WaylandWlrForeignToplevelHandleV1Private::get(toplevel);
        auto toplevelResource = d->add(resource->client(),
                                       WaylandWlrForeignToplevelHandleV1Private::interfaceVersion());

        send_toplevel(resource->handle, toplevelResource->handle);

        auto statesBytes = QByteArray::fromRawData(reinterpret_cast<const char *>(d->states.data()),
                                                   d->states.size() * static_cast<int>(sizeof(WaylandWlrForeignToplevelHandleV1Private::state)));

        d->send_title(toplevelResource->handle, d->title);
        d->send_app_id(toplevelResource->handle, d->appId);
        d->send_state(toplevelResource->handle, statesBytes);

        if (d->output) {
            auto client = QWaylandClient::fromWlClient(d->output->compositor(), resource->client());

            if (d->enteredOutput)
                d->send_output_enter(toplevelResource->handle, d->output->resourceForClient(client));
            else
                d->send_output_leave(toplevelResource->handle, d->output->resourceForClient(client));
        }

        d->send_done(toplevelResource->handle);
    }
}

void WaylandWlrForeignToplevelManagerV1Private::zwlr_foreign_toplevel_manager_v1_stop(Resource *resource)
{
    Q_Q(WaylandWlrForeignToplevelManagerV1);

    send_finished(resource->handle);

    stoppedClients.append(resource->client());
    emit q->clientStopped(QWaylandClient::fromWlClient(compositor, resource->client()));
}


WaylandWlrForeignToplevelManagerV1::WaylandWlrForeignToplevelManagerV1()
    : QWaylandCompositorExtensionTemplate<WaylandWlrForeignToplevelManagerV1>()
    , d_ptr(new WaylandWlrForeignToplevelManagerV1Private(this))
{
}

WaylandWlrForeignToplevelManagerV1::WaylandWlrForeignToplevelManagerV1(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<WaylandWlrForeignToplevelManagerV1>(compositor)
    , d_ptr(new WaylandWlrForeignToplevelManagerV1Private(this))
{
    d_ptr->compositor = compositor;
}

WaylandWlrForeignToplevelManagerV1::~WaylandWlrForeignToplevelManagerV1()
{
    delete d_ptr;
}

void WaylandWlrForeignToplevelManagerV1::initialize()
{
    Q_D(WaylandWlrForeignToplevelManagerV1);

    QWaylandCompositorExtensionTemplate::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcWaylandServer) << "Failed to find QWaylandCompositor when initializing WaylandWlrForeignToplevelManagerV1";
        return;
    }
    d->compositor = compositor;
    d->init(compositor->display(), WaylandWlrForeignToplevelManagerV1Private::interfaceVersion());
}

const wl_interface *WaylandWlrForeignToplevelManagerV1::interface()
{
    return WaylandWlrForeignToplevelManagerV1Private::interface();
}

QByteArray WaylandWlrForeignToplevelManagerV1::interfaceName()
{
    return WaylandWlrForeignToplevelManagerV1Private::interfaceName();
}


WaylandWlrForeignToplevelHandleV1Private::WaylandWlrForeignToplevelHandleV1Private(WaylandWlrForeignToplevelHandleV1 *self)
    : QtWaylandServer::zwlr_foreign_toplevel_handle_v1()
    , q_ptr(self)
{
}

void WaylandWlrForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_set_maximized(Resource *resource)
{
    Q_UNUSED(resource)

    Q_Q(WaylandWlrForeignToplevelHandleV1);
    emit q->maximizeRequested();
}

void WaylandWlrForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_unset_maximized(Resource *resource)
{
    Q_UNUSED(resource)

    Q_Q(WaylandWlrForeignToplevelHandleV1);
    emit q->unmaximizeRequested();
}

void WaylandWlrForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_set_minimized(Resource *resource)
{
    Q_UNUSED(resource)

    Q_Q(WaylandWlrForeignToplevelHandleV1);
    emit q->minimizeRequested();
}

void WaylandWlrForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_unset_minimized(Resource *resource)
{
    Q_UNUSED(resource)

    Q_Q(WaylandWlrForeignToplevelHandleV1);
    emit q->unminimizeRequested();
}

void WaylandWlrForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_activate(Resource *resource, wl_resource *seatResource)
{
    Q_UNUSED(resource)

    Q_Q(WaylandWlrForeignToplevelHandleV1);
    emit q->activateRequested(QWaylandSeat::fromSeatResource(seatResource));
}

void WaylandWlrForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_close(Resource *resource)
{
    Q_UNUSED(resource)

    Q_Q(WaylandWlrForeignToplevelHandleV1);
    emit q->closeRequested();
}

void WaylandWlrForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_set_rectangle(Resource *resource,
                                                                                             wl_resource *surfaceResource,
                                                                                             int32_t x, int32_t y, int32_t width, int32_t height)
{
    Q_Q(WaylandWlrForeignToplevelHandleV1);

    if (width < 0 || height < 0) {
        wl_resource_post_error(resource->handle, error_invalid_rectangle,
                               "invalid rectangle passed to set_rectangle: width and/or height < 0");
        return;
    }

    rectSurface = QWaylandSurface::fromResource(surfaceResource);
    rect = QRect(x, y, width, height);

    emit q->rectangleChanged();
}

void WaylandWlrForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void WaylandWlrForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_set_fullscreen(Resource *resource, wl_resource *outputResource)
{
    Q_UNUSED(resource)

    Q_Q(WaylandWlrForeignToplevelHandleV1);
    emit q->fullscreenRequested(outputResource ? QWaylandOutput::fromResource(outputResource) : nullptr);
}

void WaylandWlrForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_unset_fullscreen(Resource *resource)
{
    Q_UNUSED(resource)

    Q_Q(WaylandWlrForeignToplevelHandleV1);
    emit q->unfullscreenRequested();
}


WaylandWlrForeignToplevelHandleV1::WaylandWlrForeignToplevelHandleV1(QObject *parent)
    : QObject(parent)
    , d_ptr(new WaylandWlrForeignToplevelHandleV1Private(this))
{
}

WaylandWlrForeignToplevelHandleV1::~WaylandWlrForeignToplevelHandleV1()
{
    delete d_ptr;
}

QWaylandCompositor *WaylandWlrForeignToplevelHandleV1::compositor() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->compositor;
}

void WaylandWlrForeignToplevelHandleV1::setCompositor(QWaylandCompositor *compositor)
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    if (d->compositor == compositor)
        return;

    if (d->initialized) {
        qCWarning(lcWaylandServer, "Cannot changed WaylandWlrForeignToplevelManagerV1::compositor after initialization");
        return;
    }

    d->compositor = compositor;
    emit compositorChanged();
}

WaylandWlrForeignToplevelManagerV1 *WaylandWlrForeignToplevelHandleV1::manager() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->manager;
}

void WaylandWlrForeignToplevelHandleV1::setManager(WaylandWlrForeignToplevelManagerV1 *manager)
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    if (d->manager == manager)
        return;

    if (d->initialized) {
        qCWarning(lcWaylandServer, "Cannot changed WaylandWlrForeignToplevelManagerV1::manager after initialization");
        return;
    }

    d->manager = manager;
    emit managerChanged();
}

bool WaylandWlrForeignToplevelHandleV1::isMaximized() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_maximized);
}

void WaylandWlrForeignToplevelHandleV1::setMaximized(bool maximized)
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    bool changed = false;
    if (maximized && !d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_maximized))
        changed = true;
    else if (!maximized && d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_maximized))
        changed = true;

    if (changed) {
        if (maximized)
            d->states.append(WaylandWlrForeignToplevelHandleV1Private::state_maximized);
        else
            d->states.removeAll(WaylandWlrForeignToplevelHandleV1Private::state_maximized);
        emit maximizedChanged();

        auto statesBytes = QByteArray::fromRawData(reinterpret_cast<const char *>(d->states.data()),
                                                   d->states.size() * static_cast<int>(sizeof(WaylandWlrForeignToplevelHandleV1Private::state)));

        const auto values = d->resourceMap().values();
        for (auto resource : values) {
            d->send_state(resource->handle, statesBytes);
            d->send_done(resource->handle);
        }
    }
}

bool WaylandWlrForeignToplevelHandleV1::isMinimized() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_minimized);
}

void WaylandWlrForeignToplevelHandleV1::setMinimized(bool minimized)
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    bool changed = false;
    if (minimized && !d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_minimized))
        changed = true;
    else if (!minimized && d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_minimized))
        changed = true;

    if (changed) {
        if (minimized)
            d->states.append(WaylandWlrForeignToplevelHandleV1Private::state_minimized);
        else
            d->states.removeAll(WaylandWlrForeignToplevelHandleV1Private::state_minimized);
        emit minimizedChanged();

        auto statesBytes = QByteArray::fromRawData(reinterpret_cast<const char *>(d->states.data()),
                                                   d->states.size() * static_cast<int>(sizeof(WaylandWlrForeignToplevelHandleV1Private::state)));

        const auto values = d->resourceMap().values();
        for (auto resource : values) {
            d->send_state(resource->handle, statesBytes);
            d->send_done(resource->handle);
        }
    }
}

bool WaylandWlrForeignToplevelHandleV1::isFullscreen() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_fullscreen);
}

void WaylandWlrForeignToplevelHandleV1::setFullscreen(bool fullscreen)
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    bool changed = false;
    if (fullscreen && !d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_fullscreen))
        changed = true;
    else if (!fullscreen && d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_fullscreen))
        changed = true;

    if (changed) {
        if (fullscreen)
            d->states.append(WaylandWlrForeignToplevelHandleV1Private::state_fullscreen);
        else
            d->states.removeAll(WaylandWlrForeignToplevelHandleV1Private::state_fullscreen);
        emit fullscreenChanged();

        auto statesBytes = QByteArray::fromRawData(reinterpret_cast<const char *>(d->states.data()),
                                                   d->states.size() * static_cast<int>(sizeof(WaylandWlrForeignToplevelHandleV1Private::state)));

        const auto values = d->resourceMap().values();
        for (auto resource : values) {
            d->send_state(resource->handle, statesBytes);
            d->send_done(resource->handle);
        }
    }
}

bool WaylandWlrForeignToplevelHandleV1::isActivated() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_activated);
}

void WaylandWlrForeignToplevelHandleV1::setActivated(bool activated)
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    bool changed = false;
    if (activated && !d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_activated))
        changed = true;
    else if (!activated && d->states.contains(WaylandWlrForeignToplevelHandleV1Private::state_activated))
        changed = true;

    if (changed) {
        if (activated)
            d->states.append(WaylandWlrForeignToplevelHandleV1Private::state_activated);
        else
            d->states.removeAll(WaylandWlrForeignToplevelHandleV1Private::state_activated);
        emit activatedChanged();

        auto statesBytes = QByteArray::fromRawData(reinterpret_cast<const char *>(d->states.data()),
                                                   d->states.size() * static_cast<int>(sizeof(WaylandWlrForeignToplevelHandleV1Private::state)));

        const auto values = d->resourceMap().values();
        for (auto resource : values) {
            d->send_state(resource->handle, statesBytes);
            d->send_done(resource->handle);
        }
    }
}

QString WaylandWlrForeignToplevelHandleV1::title() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->title;
}

void WaylandWlrForeignToplevelHandleV1::setTitle(const QString &title)
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    if (d->title == title)
        return;

    d->title = title;
    emit titleChanged();

    const auto values = d->resourceMap().values();
    for (auto resource : values) {
        d->send_title(resource->handle, title);
        d->send_done(resource->handle);
    }
}

QString WaylandWlrForeignToplevelHandleV1::appId() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->appId;
}

void WaylandWlrForeignToplevelHandleV1::setAppId(const QString &appId)
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    if (d->appId == appId)
        return;

    d->appId = appId;
    emit appIdChanged();

    const auto values = d->resourceMap().values();
    for (auto resource : values) {
        d->send_app_id(resource->handle, appId);
        d->send_done(resource->handle);
    }
}

QWaylandSurface *WaylandWlrForeignToplevelHandleV1::rectangleSurface() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->rectSurface;
}

QRect WaylandWlrForeignToplevelHandleV1::rectangle() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->rect;
}

qint32 WaylandWlrForeignToplevelHandleV1::rectangleX() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->rect.x();
}

qint32 WaylandWlrForeignToplevelHandleV1::rectangleY() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->rect.y();
}

qint32 WaylandWlrForeignToplevelHandleV1::rectangleWidth() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->rect.width();
}

qint32 WaylandWlrForeignToplevelHandleV1::rectangleHeight() const
{
    Q_D(const WaylandWlrForeignToplevelHandleV1);
    return d->rect.height();
}

void WaylandWlrForeignToplevelHandleV1::sendOutputEnter(QWaylandOutput *output)
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    d->output = output;

    const auto values = d->resourceMap().values();
    for (auto resource : values) {
        auto client = QWaylandClient::fromWlClient(output->compositor(), resource->client());
        d->send_output_enter(resource->handle, output->resourceForClient(client));
    }
}

void WaylandWlrForeignToplevelHandleV1::sendOutputLeave(QWaylandOutput *output)
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    d->output = nullptr;

    const auto values = d->resourceMap().values();
    for (auto resource : values) {
        auto client = QWaylandClient::fromWlClient(output->compositor(), resource->client());
        d->send_output_leave(resource->handle, output->resourceForClient(client));
    }
}

void WaylandWlrForeignToplevelHandleV1::sendClosed()
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    const auto values = d->resourceMap().values();
    for (auto resource : values)
        d->send_closed(resource->handle);
}

void WaylandWlrForeignToplevelHandleV1::initialize()
{
    Q_D(WaylandWlrForeignToplevelHandleV1);

    if (d->initialized) {
        qCWarning(lcWaylandServer, "Cannot initialize WaylandWlrForeignToplevelHandleV1 twice");
        return;
    }

    if (!d->compositor) {
        qCWarning(lcWaylandServer, "Please set WaylandWlrForeignToplevelHandleV1::compositor before the initialization");
        return;
    }

    // Try to find the manager
    if (!d->manager)
        d->manager = qobject_cast<WaylandWlrForeignToplevelManagerV1 *>(d->compositor->extension(WaylandWlrForeignToplevelManagerV1::interface()));

    if (!d->manager) {
        qCWarning(lcWaylandServer, "Please set WaylandWlrForeignToplevelHandleV1::manager before the initialization");
        return;
    }

    d->initialized = true;

    auto managerPrivate = WaylandWlrForeignToplevelManagerV1Private::get(d->manager);
    const auto values = managerPrivate->resourceMap().values();

    if (values.size() > 0) {
        // Send toplevel
        for (auto resource : values) {
            auto toplevelResource = d->add(resource->client(), WaylandWlrForeignToplevelHandleV1Private::interfaceVersion());
            managerPrivate->send_toplevel(resource->handle, toplevelResource->handle);
        }

        // Send details
        auto statesBytes = QByteArray::fromRawData(reinterpret_cast<const char *>(d->states.data()),
                                                   d->states.size() * static_cast<int>(sizeof(WaylandWlrForeignToplevelHandleV1Private::state)));
        const auto handleResources = d->resourceMap().values();
        for (auto resource : handleResources) {
            d->send_title(resource->handle, d->title);
            d->send_app_id(resource->handle, d->appId);
            d->send_state(resource->handle, statesBytes);

            if (d->output) {
                auto client = QWaylandClient::fromWlClient(d->output->compositor(), resource->client());

                if (d->enteredOutput)
                    d->send_output_enter(resource->handle, d->output->resourceForClient(client));
                else
                    d->send_output_leave(resource->handle, d->output->resourceForClient(client));
            }

            d->send_done(resource->handle);
        }
    }

    managerPrivate->toplevels.append(this);
    emit d->manager->handleAdded(this);
}
