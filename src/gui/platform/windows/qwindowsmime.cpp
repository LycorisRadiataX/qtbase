// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwindowsmime_p.h"

#include <QtGui/private/qguiapplication_p.h>
#include <QtGui/qpa/qplatformintegration.h>

QT_BEGIN_NAMESPACE

using namespace QNativeInterface::Private;

/*!
    \class QWindowsMime
    \brief The QWindowsMime class maps open-standard MIME to Window Clipboard formats.
    \internal

    Qt's drag-and-drop and clipboard facilities use the MIME standard.
    On X11, this maps trivially to the Xdnd protocol, but on Windows
    although some applications use MIME types to describe clipboard
    formats, others use arbitrary non-standardized naming conventions,
    or unnamed built-in formats of Windows.

    By instantiating subclasses of QWindowsMime that provide conversions
    between Windows Clipboard and MIME formats, you can convert
    proprietary clipboard formats to MIME formats.

    Qt has predefined support for the following Windows Clipboard formats:

    \table
    \header \li Windows Format \li Equivalent MIME type
    \row \li \c CF_UNICODETEXT \li \c text/plain
    \row \li \c CF_TEXT        \li \c text/plain
    \row \li \c CF_DIB         \li \c{image/xyz}, where \c xyz is
                                 a \l{QImageWriter::supportedImageFormats()}{Qt image format}
    \row \li \c CF_HDROP       \li \c text/uri-list
    \row \li \c CF_INETURL     \li \c text/uri-list
    \row \li \c CF_HTML        \li \c text/html
    \endtable

    An example use of this class would be to map the Windows Metafile
    clipboard format (\c CF_METAFILEPICT) to and from the MIME type
    \c{image/x-wmf}. This conversion might simply be adding or removing
    a header, or even just passing on the data. See \l{Drag and Drop}
    for more information on choosing and definition MIME types.

    You can check if a MIME type is convertible using canConvertFromMime() and
    can perform conversions with convertToMime() and convertFromMime().

    \sa QWindowsMimeRegistry
*/


/*!
    \fn bool QWindowsMime::canConvertFromMime(const FORMATETC &formatetc, const QMimeData *mimeData) const
    \internal

    Returns \c true if the converter can convert from the \a mimeData to
    the format specified in \a formatetc.

    All subclasses must reimplement this pure virtual function.
*/

/*!
    \fn bool QWindowsMime::canConvertToMime(const QString &mimeType, IDataObject *pDataObj) const
    \internal

    Returns \c true if the converter can convert to the \a mimeType from
    the available formats in \a pDataObj.

    All subclasses must reimplement this pure virtual function.
*/

/*!
    \fn QString QWindowsMime::mimeForFormat(const FORMATETC &formatetc) const
    \internal

    Returns the mime type that will be created form the format specified
    in \a formatetc, or an empty string if this converter does not support
    \a formatetc.

    All subclasses must reimplement this pure virtual function.
*/

/*!
    \fn QList<FORMATETC> QWindowsMime::formatsForMime(const QString &mimeType, const QMimeData *mimeData) const
    \internal

    Returns a QList of FORMATETC structures representing the different windows clipboard
    formats that can be provided for the \a mimeType from the \a mimeData.

    All subclasses must reimplement this pure virtual function.
*/

/*!
    \fn QVariant QWindowsMime::convertToMime(const QString &mimeType, IDataObject *pDataObj,
                                             QMetaType preferredType) const
    \internal

    Returns a QVariant containing the converted data for \a mimeType from \a pDataObj.
    If possible the QVariant should be of the \a preferredType to avoid needless conversions.

    All subclasses must reimplement this pure virtual function.
*/

/*!
    \fn bool QWindowsMime::convertFromMime(const FORMATETC &formatetc, const QMimeData *mimeData, STGMEDIUM * pmedium) const
    \internal

    Convert the \a mimeData to the format specified in \a formatetc.
    The converted data should then be placed in \a pmedium structure.

    Return true if the conversion was successful.

    All subclasses must reimplement this pure virtual function.
*/

/*!
    Constructs a QWindowsMime instance.

    The instance is automatically registered, and will be called to convert data during
    clipboard or drag'n'drop operations.
*/
QWindowsMime::QWindowsMime()
{
    auto nativeWindowsApp = dynamic_cast<QWindowsApplication *>(QGuiApplicationPrivate::platformIntegration());
    Q_ASSERT(nativeWindowsApp);
    nativeWindowsApp->registerMime(this);
}

/*!
    Constructs a QWindowsMime instance.

    The instance is automatically unregistered.
*/
QWindowsMime::~QWindowsMime()
{
    auto nativeWindowsApp = dynamic_cast<QWindowsApplication *>(QGuiApplicationPrivate::platformIntegration());
    Q_ASSERT(nativeWindowsApp);
    nativeWindowsApp->unregisterMime(this);
}

/*!
    Registers the MIME type \a mime, and returns an ID number
    identifying the format on Windows.

    A mime type \c {application/x-qt-windows-mime;value="WindowsType"} will be
    registered as the clipboard format for \c WindowsType.
*/
int QWindowsMime::registerMimeType(const QString &mimeType)
{
    auto nativeWindowsApp = dynamic_cast<QWindowsApplication *>(QGuiApplicationPrivate::platformIntegration());
    Q_ASSERT(nativeWindowsApp);
    return nativeWindowsApp->registerMimeType(mimeType);
}

QT_END_NAMESPACE
