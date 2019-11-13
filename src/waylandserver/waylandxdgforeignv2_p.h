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

#ifndef LIRI_WAYLANDXDGFOREIGNV2_P_H
#define LIRI_WAYLANDXDGFOREIGNV2_P_H

#include <QWaylandSurface>

#include <LiriWaylandServer/WaylandXdgForeignV2>
#include <LiriWaylandServer/private/qwayland-server-xdg-foreign-unstable-v2.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class LIRIWAYLANDSERVER_EXPORT WaylandXdgExporterV2Private
        : public QtWaylandServer::zxdg_exporter_v2
{
    Q_DECLARE_PUBLIC(WaylandXdgExporterV2)
public:
    explicit WaylandXdgExporterV2Private(WaylandXdgExporterV2 *self);

protected:
    WaylandXdgExporterV2 *q_ptr;

    void zxdg_exporter_v2_destroy_resource(Resource *resource) override;
    void zxdg_exporter_v2_destroy(Resource *resource) override;
    void zxdg_exporter_v2_export_toplevel(Resource *resource, uint32_t id, struct ::wl_resource *surface_res) override;
};

class LIRIWAYLANDSERVER_EXPORT WaylandXdgImporterV2Private
        : public QtWaylandServer::zxdg_importer_v2
{
    Q_DECLARE_PUBLIC(WaylandXdgImporterV2)
public:
    explicit WaylandXdgImporterV2Private(WaylandXdgImporterV2 *self);

protected:
    WaylandXdgImporterV2 *q_ptr;

    void zxdg_importer_v2_destroy_resource(Resource *resource) override;
    void zxdg_importer_v2_destroy(Resource *resource) override;
    void zxdg_importer_v2_import_toplevel(Resource *resource, uint32_t id, const QString &handle) override;
};

class LIRIWAYLANDSERVER_EXPORT WaylandXdgExportedV2Private
        : public QtWaylandServer::zxdg_exported_v2
{
    Q_DECLARE_PUBLIC(WaylandXdgExportedV2)
public:
    explicit WaylandXdgExportedV2Private(WaylandXdgExportedV2 *self);

    bool initialized = false;
    QWaylandSurface *surface = nullptr;
    QString handle;

    static WaylandXdgExportedV2Private *get(WaylandXdgExportedV2 *exported) { return exported ? exported->d_func() : nullptr; }

protected:
    WaylandXdgExportedV2 *q_ptr;

    void zxdg_exported_v2_destroy(Resource *resource) override;
};

class LIRIWAYLANDSERVER_EXPORT WaylandXdgImportedV2Private
        : public QtWaylandServer::zxdg_imported_v2
{
    Q_DECLARE_PUBLIC(WaylandXdgImportedV2)
public:
    explicit WaylandXdgImportedV2Private(WaylandXdgImportedV2 *self);

    bool initialized = false;
    QString handle;
    QWaylandSurface *parentSurface = nullptr;

    static WaylandXdgImportedV2Private *get(WaylandXdgImportedV2 *imported) { return imported ? imported->d_func() : nullptr; }

protected:
    WaylandXdgImportedV2 *q_ptr;

    void zxdg_imported_v2_destroy(Resource *resource) override;
    void zxdg_imported_v2_set_parent_of(Resource *resource, struct ::wl_resource *surface_res) override;
};

#endif // LIRI_WAYLANDXDGFOREIGNV2_P_H
