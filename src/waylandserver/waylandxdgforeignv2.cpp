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

#include <QtWaylandCompositor>

#include "logging_p.h"
#include "waylandxdgforeignv2_p.h"

/*
 * WaylandXdgExporterV2Private
 */

WaylandXdgExporterV2Private::WaylandXdgExporterV2Private(WaylandXdgExporterV2 *self)
    : QtWaylandServer::zxdg_exporter_v2()
    , q_ptr(self)
{
}

void WaylandXdgExporterV2Private::zxdg_exporter_v2_destroy_resource(Resource *resource)
{
}

void WaylandXdgExporterV2Private::zxdg_exporter_v2_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void WaylandXdgExporterV2Private::zxdg_exporter_v2_export_toplevel(Resource *resource, uint32_t id, struct ::wl_resource *surface_res)
{
    Q_Q(WaylandXdgExporterV2);

    auto *surface = QWaylandSurface::fromResource(surface_res);

    auto *exported = new WaylandXdgExportedV2(q);
    auto *exportedPriv = WaylandXdgExportedV2Private::get(exported);
    exportedPriv->surface = surface;
    exportedPriv->init(resource->client(), id, wl_resource_get_version(resource->handle));
    exportedPriv->initialized = true;

    emit q->toplevelExported(exported);
}

/*
 * WaylandXdgExporterV2
 */

WaylandXdgExporterV2::WaylandXdgExporterV2()
    : QWaylandCompositorExtensionTemplate<WaylandXdgExporterV2>()
    , d_ptr(new WaylandXdgExporterV2Private(this))
{
}

WaylandXdgExporterV2::WaylandXdgExporterV2(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<WaylandXdgExporterV2>(compositor)
    , d_ptr(new WaylandXdgExporterV2Private(this))
{
}

WaylandXdgExporterV2::~WaylandXdgExporterV2()
{
    delete d_ptr;
}

void WaylandXdgExporterV2::initialize()
{
    Q_D(WaylandXdgExporterV2);

    QWaylandCompositorExtensionTemplate::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcXdgForeign) << "Failed to find QWaylandCompositor when initializing WaylandXdgExporterV2";
        return;
    }
    d->init(compositor->display(), WaylandXdgExporterV2Private::interfaceVersion());
}

const struct ::wl_interface *WaylandXdgExporterV2::interface()
{
    return WaylandXdgExporterV2Private::interface();
}

QByteArray WaylandXdgExporterV2::interfaceName()
{
    return WaylandXdgExporterV2Private::interfaceName();
}

/*
 * WaylandXdgImporterV2Private
 */

WaylandXdgImporterV2Private::WaylandXdgImporterV2Private(WaylandXdgImporterV2 *self)
    : QtWaylandServer::zxdg_importer_v2()
    , q_ptr(self)
{
}

void WaylandXdgImporterV2Private::zxdg_importer_v2_destroy_resource(Resource *resource)
{

}

void WaylandXdgImporterV2Private::zxdg_importer_v2_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void WaylandXdgImporterV2Private::zxdg_importer_v2_import_toplevel(Resource *resource, uint32_t id, const QString &handle)
{
    Q_Q(WaylandXdgImporterV2);

    auto *imported = new WaylandXdgImportedV2(q);
    auto *importedPriv = WaylandXdgImportedV2Private::get(imported);
    importedPriv->handle = handle;
    importedPriv->init(resource->client(), id, wl_resource_get_version(resource->handle));
    importedPriv->initialized = true;

    emit q->toplevelImported(imported);
}

/*
 * WaylandXdgImporterV2
 */

WaylandXdgImporterV2::WaylandXdgImporterV2()
    : QWaylandCompositorExtensionTemplate<WaylandXdgImporterV2>()
    , d_ptr(new WaylandXdgImporterV2Private(this))
{
}

WaylandXdgImporterV2::WaylandXdgImporterV2(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<WaylandXdgImporterV2>(compositor)
    , d_ptr(new WaylandXdgImporterV2Private(this))
{
}

WaylandXdgImporterV2::~WaylandXdgImporterV2()
{
    delete d_ptr;
}

void WaylandXdgImporterV2::initialize()
{
    Q_D(WaylandXdgImporterV2);

    QWaylandCompositorExtensionTemplate::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcXdgForeign) << "Failed to find QWaylandCompositor when initializing WaylandXdgImporterV2";
        return;
    }
    d->init(compositor->display(), WaylandXdgImporterV2Private::interfaceVersion());
}

const struct ::wl_interface *WaylandXdgImporterV2::interface()
{
    return WaylandXdgImporterV2Private::interface();
}

QByteArray WaylandXdgImporterV2::interfaceName()
{
    return WaylandXdgImporterV2Private::interfaceName();
}

/*
 * WaylandXdgExportedV2Private
 */

WaylandXdgExportedV2Private::WaylandXdgExportedV2Private(WaylandXdgExportedV2 *self)
    : QtWaylandServer::zxdg_exported_v2()
    , q_ptr(self)
{
}

void WaylandXdgExportedV2Private::zxdg_exported_v2_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

/*
 * WaylandXdgExportedV2
 */

WaylandXdgExportedV2::WaylandXdgExportedV2(QObject *parent)
    : QObject(parent)
    , d_ptr(new WaylandXdgExportedV2Private(this))
{
}

WaylandXdgExportedV2::~WaylandXdgExportedV2()
{
    delete d_ptr;
}

QString WaylandXdgExportedV2::handle() const
{
    Q_D(const WaylandXdgExportedV2);
    return d->handle;
}

void WaylandXdgExportedV2::setHandle(const QString &handle)
{
    Q_D(WaylandXdgExportedV2);

    if (d->handle == handle)
        return;

    d->handle = handle;
    emit handleChanged();

    if (d->initialized)
        d->send_handle(handle);
}

/*
 * WaylandXdgExportedV2Private
 */

WaylandXdgImportedV2Private::WaylandXdgImportedV2Private(WaylandXdgImportedV2 *self)
    : QtWaylandServer::zxdg_imported_v2()
    , q_ptr(self)
{
}

void WaylandXdgImportedV2Private::zxdg_imported_v2_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void WaylandXdgImportedV2Private::zxdg_imported_v2_set_parent_of(Resource *resource, struct ::wl_resource *surface_res)
{
    Q_Q(WaylandXdgImportedV2);

    auto *oldSurface = parentSurface;
    parentSurface = QWaylandSurface::fromResource(surface_res);
    emit q->parentSurfaceChanged(oldSurface, parentSurface);
}

/*
 * WaylandXdgImportedV2
 */

WaylandXdgImportedV2::WaylandXdgImportedV2(QObject *parent)
    : QObject(parent)
    , d_ptr(new WaylandXdgImportedV2Private(this))
{
}

WaylandXdgImportedV2::~WaylandXdgImportedV2()
{
    delete d_ptr;
}

QString WaylandXdgImportedV2::handle() const
{
    Q_D(const WaylandXdgImportedV2);
    return d->handle;
}

QWaylandSurface *WaylandXdgImportedV2::parentSurface() const
{
    Q_D(const WaylandXdgImportedV2);
    return d->parentSurface;
}

void WaylandXdgImportedV2::sendDestroyed()
{
    Q_D(WaylandXdgImportedV2);

    if (d->initialized)
        d->send_destroyed();
}
