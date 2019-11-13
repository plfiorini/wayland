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

#ifndef LIRI_WAYLANDXDGFOREIGNV2_H
#define LIRI_WAYLANDXDGFOREIGNV2_H

#include <QtWaylandCompositor/QWaylandCompositorExtension>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

class WaylandXdgExporterV2Private;
class WaylandXdgImporterV2Private;
class WaylandXdgExportedV2;
class WaylandXdgExportedV2Private;
class WaylandXdgImportedV2;
class WaylandXdgImportedV2Private;

class LIRIWAYLANDSERVER_EXPORT WaylandXdgExporterV2
        : public QWaylandCompositorExtensionTemplate<WaylandXdgExporterV2>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandXdgExporterV2)
public:
    WaylandXdgExporterV2();
    WaylandXdgExporterV2(QWaylandCompositor *compositor);
    ~WaylandXdgExporterV2();

    void initialize() override;

    static const struct ::wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void toplevelExported(WaylandXdgExportedV2 *exported);

private:
    WaylandXdgExporterV2Private *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandXdgImporterV2
        : public QWaylandCompositorExtensionTemplate<WaylandXdgImporterV2>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandXdgImporterV2)
public:
    WaylandXdgImporterV2();
    WaylandXdgImporterV2(QWaylandCompositor *compositor);
    ~WaylandXdgImporterV2();

    void initialize() override;

    static const struct ::wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void toplevelImported(WaylandXdgImportedV2 *imported);

private:
    WaylandXdgImporterV2Private *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandXdgExportedV2
        : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString handle READ handle WRITE setHandle NOTIFY handleChanged)
    Q_DECLARE_PRIVATE(WaylandXdgExportedV2)
public:
    explicit WaylandXdgExportedV2(QObject *parent = nullptr);
    ~WaylandXdgExportedV2();

    QString handle() const;
    void setHandle(const QString &handle);

Q_SIGNALS:
    void handleChanged();

private:
    WaylandXdgExportedV2Private *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandXdgImportedV2
        : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString handle READ handle CONSTANT)
    Q_PROPERTY(QWaylandSurface *parentSurface READ parentSurface NOTIFY parentSurfaceChanged)
    Q_DECLARE_PRIVATE(WaylandXdgImportedV2)
public:
    explicit WaylandXdgImportedV2(QObject *parent = nullptr);
    ~WaylandXdgImportedV2();

    QString handle() const;
    QWaylandSurface *parentSurface() const;

    Q_INVOKABLE void sendDestroyed();

Q_SIGNALS:
    void parentSurfaceChanged(QWaylandSurface *oldSurface, QWaylandSurface *newSurface);

private:
    WaylandXdgImportedV2Private *const d_ptr;
};

#endif // LIRI_WAYLANDXDGFOREIGNV2_H
