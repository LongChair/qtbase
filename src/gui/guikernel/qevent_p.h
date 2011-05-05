/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QEVENT_P_H
#define QEVENT_P_H

#include <QtCore/qglobal.h>
#include <QtCore/qurl.h>
#include <QtGui/qevent.h>

#ifdef Q_OS_SYMBIAN
#include <f32file.h>
#endif

QT_BEGIN_NAMESPACE

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

// ### Qt 5: remove
class QKeyEventEx : public QKeyEvent
{
public:
    QKeyEventEx(Type type, int key, Qt::KeyboardModifiers modifiers,
                const QString &text, bool autorep, ushort count,
                quint32 nativeScanCode, quint32 nativeVirtualKey, quint32 nativeModifiers);
    QKeyEventEx(const QKeyEventEx &other);

    ~QKeyEventEx();

protected:
    quint32 nScanCode;
    quint32 nVirtualKey;
    quint32 nModifiers;
    friend class QKeyEvent;
};

// ### Qt 5: remove
class QMouseEventEx : public QMouseEvent
{
public:
    QMouseEventEx(Type type, const QPointF &pos, const QPoint &globalPos,
                  Qt::MouseButton button, Qt::MouseButtons buttons,
                  Qt::KeyboardModifiers modifiers);
    ~QMouseEventEx();

protected:
    QPointF posF;
    friend class QMouseEvent;
};

class QTouchEventTouchPointPrivate
{
public:
    inline QTouchEventTouchPointPrivate(int id)
        : ref(1),
          id(id),
          state(Qt::TouchPointReleased),
          pressure(qreal(-1.))
    { }

    inline QTouchEventTouchPointPrivate *detach()
    {
        QTouchEventTouchPointPrivate *d = new QTouchEventTouchPointPrivate(*this);
        d->ref = 1;
        if (!this->ref.deref())
            delete this;
        return d;
    }

    QAtomicInt ref;
    int id;
    Qt::TouchPointStates state;
    QRectF rect, sceneRect, screenRect;
    QPointF normalizedPos,
            startPos, startScenePos, startScreenPos, startNormalizedPos,
            lastPos, lastScenePos, lastScreenPos, lastNormalizedPos;
    qreal pressure;
};

class QFileOpenEventPrivate
{
public:
    inline QFileOpenEventPrivate(const QUrl &url)
        : url(url)
    {
    }
    ~QFileOpenEventPrivate();

    QUrl url;
#ifdef Q_OS_SYMBIAN
    RFile file;
#endif
};


class QScrollPrepareEventPrivate
{
public:
    inline QScrollPrepareEventPrivate()
        : target(0)
    {
    }

    QObject* target;
    QPointF startPos;
    QSizeF viewportSize;
    QRectF contentPosRange;
    QPointF contentPos;
};

class QScrollEventPrivate
{
public:
    inline QScrollEventPrivate()
    {
    }

    QPointF contentPos;
    QPointF overshoot;
    QScrollEvent::ScrollState state;
};

QT_END_NAMESPACE

#endif // QEVENT_P_H
