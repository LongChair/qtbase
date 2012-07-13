/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qaccessible.h"

#ifndef QT_NO_ACCESSIBILITY

#include "qaccessibleplugin.h"
#include "qaccessibleobject.h"
#include "qaccessiblebridge.h"
#include <QtGui/QGuiApplication>
#include <private/qguiapplication_p.h>
#include <qpa/qplatformaccessibility.h>
#include <qpa/qplatformintegration.h>

#include <QtCore/qdebug.h>
#include <QtCore/qmetaobject.h>
#include <private/qfactoryloader_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QAccessible
    \brief The QAccessible class provides enums and static functions
    related to accessibility.
    \internal

    \ingroup accessibility
    \inmodule QtWidgets

    This class is part of \l {Accessibility for QWidget Applications}.

    Accessible applications can be used by people who are not able to
    use applications by conventional means.

    The functions in this class are used for communication between
    accessible applications (also called AT Servers) and
    accessibility tools (AT Clients), such as screen readers and
    braille displays. Clients and servers communicate in the following way:

    \list
    \li  \e{AT Servers} notify the clients about events through calls to the
        updateAccessibility() function.

    \li  \e{AT Clients} request information about the objects in the server.
        The QAccessibleInterface class is the core interface, and encapsulates
        this information in a pure virtual API. Implementations of the interface
        are provided by Qt through the queryAccessibleInterface() API.
    \endlist

    The communication between servers and clients is initialized by
    the setRootObject() function. Function pointers can be installed
    to replace or extend the default behavior of the static functions
    in QAccessible.

    Qt supports Microsoft Active Accessibility (MSAA), Mac OS X
    Accessibility, and the Unix/X11 AT-SPI standard. Other backends
    can be supported using QAccessibleBridge.

    In addition to QAccessible's static functions, Qt offers one
    generic interface, QAccessibleInterface, that can be used to wrap
    all widgets and objects (e.g., QPushButton). This single
    interface provides all the metadata necessary for the assistive
    technologies. Qt provides implementations of this interface for
    its built-in widgets as plugins.

    When you develop custom widgets, you can create custom subclasses
    of QAccessibleInterface and distribute them as plugins (using
    QAccessiblePlugin) or compile them into the application.
    Likewise, Qt's predefined accessibility support can be built as
    plugin (the default) or directly into the Qt library. The main
    advantage of using plugins is that the accessibility classes are
    only loaded into memory if they are actually used; they don't
    slow down the common case where no assistive technology is being
    used.

    Qt also includes two convenience classes, QAccessibleObject and
    QAccessibleWidget, that inherit from QAccessibleInterface and
    provide the lowest common denominator of metadata (e.g., widget
    geometry, window title, basic help text). You can use them as
    base classes when wrapping your custom QObject or QWidget
    subclasses.

    \sa QAccessibleInterface
*/


/*!
    \class QAccessible::State

    \inmodule QtGui

    This structure defines bit flags that indicate
    the state of an accessible object. The values are:

    \value active                  The object is the active window or the active sub-element in a container (that would get focus when focusing the container).
    \value adjustable              The object represents an adjustable value, e.g. sliders.
    \value animated                The object's appearance changes frequently.
    \value busy                    The object cannot accept input at the moment.
    \value checkable               The object is checkable.
    \value checked                 The object's check box is checked.
    \value checkStateMixed         The third state of checkboxes (half checked in tri-state check boxes).
    \value collapsed               The object is collapsed, e.g. a closed listview item, or an iconified window.
    \value defaultButton           The object represents the default button in a dialog.
    \value defunct                 The object no longer exists.
    \value editable                The object has a text carret (and often implements the text interface).
    \value expandable              The object is expandable, mostly used for cells in a tree view.
    \value expanded                The object is expanded, currently its children are visible.
    \value extSelectable           The object supports extended selection.
    \value focusable               The object can receive focus. Only objects in the active window can receive focus.
    \value focused                 The object has keyboard focus.
    \value hasPopup                The object opens a popup.
    \value hotTracked              The object's appearance is sensitive to the mouse cursor position.
    \value invalid                 The object is no longer valid (because it has been deleted).
    \value invalidEntry            Input validation current input invalid.
    \value invisible               The object is not visible to the user.
    \value linked                  The object is linked to another object, e.g. a hyperlink.
    \value marqueed                The object displays scrolling contents, e.g. a log view.
    \value modal                   The object blocks input from other objects.
    \value movable                 The object can be moved.
    \value multiLine               The object has multiple lines of text (word wrap), as opposed to a single line.
    \value multiSelectable         The object supports multiple selected items.
    \value offscreen               The object is clipped by the visible area. Objects that are off screen are also invisible.
    \value passwordEdit            The object is a password field, e.g. a line edit for entering a Password.
    \value playsSound              The object produces sound when interacted with.
    \value pressed                 The object is pressed.
    \value readOnly                The object can usually be edited, but is explicitly set to read-only.
    \value selectable              The object is selectable.
    \value selectableText          The object has text which can be selected. This is different from selectable which refers to the object's children.
    \value selected                The object is selected.
    \value selfVoicing             The object describes itself through speech or sound.
    \value sizeable                The object can be resized, e.g. top-level windows.
    \value summaryElement          The object summarizes the state of the window and should be treated with priority.
    \value supportsAutoCompletion  The object has auto-completion, for example in line edits or combo boxes.
    \value traversed               The object is linked and has been visited.
    \value updatesFrequently       The object changes frequently and needs to be refreshed when accessing it.
    \value disabled                The object is unavailable to the user, e.g. a disabled widget.

    Implementations of QAccessibleInterface::state() return a combination
    of these flags.
*/

/*!
    \fn QAccessible::State::State()

    Creates a new QAccessible::State with all states set to false.
*/

/*!
    \enum QAccessible::Event

    This enum type defines accessible event types.

    \value AcceleratorChanged               The keyboard accelerator for an action has been changed.
    \value ActionChanged                    An action has been changed.
    \value ActiveDescendantChanged
    \value Alert                            A system alert (e.g., a message from a QMessageBox)
    \value AttributeChanged
    \value ContextHelpEnd                   Context help (QWhatsThis) for an object is finished.
    \value ContextHelpStart                 Context help (QWhatsThis) for an object is initiated.
    \value DefaultActionChanged             The default QAccessible::Action for the accessible
                                            object has changed.
    \value DescriptionChanged               The object's QAccessible::Description changed.
    \value DialogEnd                        A dialog (QDialog) has been hidden
    \value DialogStart                      A dialog (QDialog) has been set visible.
    \value DocumentContentChanged           The contents of a text document have changed.
    \value DocumentLoadComplete             A document has been loaded.
    \value DocumentLoadStopped              A document load has been stopped.
    \value DocumentReload                   A document reload has been initiated.
    \value DragDropEnd                      A drag and drop operation is about to finished.
    \value DragDropStart                    A drag and drop operation is about to be initiated.
    \value Focus                            An object has gained keyboard focus.
    \value ForegroundChanged                A window has been activated (i.e., a new window has
                                            gained focus on the desktop).
    \value HelpChanged                      The QAccessible::Help text property of an object has
                                            changed.
    \value HyperlinkEndIndexChanged         The end position of the display text for a hypertext
                                            link has changed.
    \value HyperlinkNumberOfAnchorsChanged  The number of anchors in a hypertext link has changed,
                                            perhaps because the display text has been split to
                                            provide more than one link.
    \value HyperlinkSelectedLinkChanged     The link for the selected hypertext link has changed.
    \value HyperlinkStartIndexChanged       The start position of the display text for a hypertext
                                            link has changed.
    \value HypertextChanged                 The display text for a hypertext link has changed.
    \value HypertextLinkActivated           A hypertext link has been activated, perhaps by being
                                            clicked or via a key press.
    \value HypertextLinkSelected            A hypertext link has been selected.
    \value HypertextNLinksChanged
    \value LocationChanged                  An object's location on the screen has changed.
    \value MenuCommand                      A menu item is triggered.
    \value MenuEnd                          A menu has been closed (Qt uses PopupMenuEnd for all
                                            menus).
    \value MenuStart                        A menu has been opened on the menubar (Qt uses
                                            PopupMenuStart for all menus).
    \value NameChanged                      The QAccessible::Name property of an object has changed.
    \value ObjectAttributeChanged
    \value ObjectCreated                    A new object is created.
    \value ObjectDestroyed                  An object is deleted.
    \value ObjectHide                       An object is hidden; for example, with QWidget::hide().
                                            Any children the object that is hidden has do not send
                                            this event. It is not sent when an object is hidden as
                                            it is being obcured by others.
    \value ObjectReorder                    A layout or item view  has added, removed, or moved an
                                            object (Qt does not use this event).
    \value ObjectShow                       An object is displayed; for example, with
                                            QWidget::show().
    \value PageChanged
    \value ParentChanged                    An object's parent object changed.
    \value PopupMenuEnd                     A pop-up menu has closed.
    \value PopupMenuStart                   A pop-up menu has opened.
    \value ScrollingEnd                     A scrollbar scroll operation has ended (the mouse has
                                            released the slider handle).
    \value ScrollingStart                   A scrollbar scroll operation is about to start; this may
                                            be caused by a mouse press on the slider handle, for
                                            example.
    \value SectionChanged
    \value SelectionAdd                     An item has been added to the selection in an item view.
    \value SelectionRemove                  An item has been removed from an item view selection.
    \value Selection                        The selection has changed in a menu or item view.
    \value SelectionWithin                  Several changes to a selection has occurred in an item
                                            view.
    \value SoundPlayed                      A sound has been played by an object
    \omitvalue StateChanged                 The QAccessible::State of an object has changed.
                                            This value is used internally for the QAccessibleStateChangeEvent.
    \value TableCaptionChanged              A table caption has been changed.
    \value TableColumnDescriptionChanged    The description of a table column, typically found in
                                            the column's header, has been changed.
    \value TableColumnHeaderChanged         A table column header has been changed.
    \value TableModelChanged                The model providing data for a table has been changed.
    \value TableRowDescriptionChanged       The description of a table row, typically found in the
                                            row's header, has been changed.
    \value TableRowHeaderChanged            A table row header has been changed.
    \value TableSummaryChanged              The summary of a table has been changed.
    \omitvalue TextAttributeChanged
    \omitvalue TextCaretMoved                   The caret has moved in an editable widget.
                                            The caret represents the cursor position in an editable
                                            widget with the input focus.
    \value TextColumnChanged                A text column has been changed.
    \omitvalue TextInserted                     Text has been inserted into an editable widget.
    \omitvalue TextRemoved                      Text has been removed from an editable widget.
    \omitvalue TextSelectionChanged             The selected text has changed in an editable widget.
    \omitvalue TextUpdated                      The text has been update in an editable widget.
    \omitvalue ValueChanged                     The QAccessible::Value of an object has changed.
    \value VisibleDataChanged

    The values for this enum are defined to be the same as those defined in the
    \l{AccessibleEventID.idl File Reference}{IAccessible2} and
    \l{Microsoft Active Accessibility Event Constants}{MSAA} specifications.
*/

/*!
    \enum QAccessible::Role

    This enum defines the role of an accessible object. The roles are:

    \value AlertMessage     An object that is used to alert the user.
    \value Animation        An object that displays an animation.
    \value Application      The application's main window.
    \value Assistant        An object that provids interactive help.
    \value Border           An object that represents a border.
    \value ButtonDropDown   A button that drops down a list of items.
    \value ButtonDropGrid   A button that drops down a grid.
    \value ButtonMenu       A button that drops down a menu.
    \value Canvas           An object that displays graphics that the user can interact with.
    \value Caret            An object that represents the system caret (text cursor).
    \value Cell             A cell in a table.
    \value Chart            An object that displays a graphical representation of data.
    \value CheckBox         An object that represents an option that can be checked or unchecked. Some options provide a "mixed" state, e.g. neither checked nor unchecked.
    \value Client           The client area in a window.
    \value Clock            A clock displaying time.
    \value Column           A column of cells, usually within a table.
    \value ColumnHeader     A header for a column of data.
    \value ComboBox         A list of choices that the user can select from.
    \value Cursor           An object that represents the mouse cursor.
    \value Desktop          The object represents the desktop or workspace.
    \value Dial             An object that represents a dial or knob.
    \value Dialog           A dialog box.
    \value Document         A document window, usually in an MDI environment.
    \value EditableText     Editable text
    \value Equation         An object that represents a mathematical equation.
    \value Graphic          A graphic or picture, e.g. an icon.
    \value Grip             A grip that the user can drag to change the size of widgets.
    \value Grouping         An object that represents a logical grouping of other objects.
    \value HelpBalloon      An object that displays help in a separate, short lived window.
    \value HotkeyField      A hotkey field that allows the user to enter a key sequence.
    \value Indicator        An indicator that represents a current value or item.
    \value LayeredPane      An object that can contain layered children, e.g. in a stack.
    \value Link             A link to something else.
    \value List             A list of items, from which the user can select one or more items.
    \value ListItem         An item in a list of items.
    \value MenuBar          A menu bar from which menus are opened by the user.
    \value MenuItem         An item in a menu or menu bar.
    \value NoRole           The object has no role. This usually indicates an invalid object.
    \value PageTab          A page tab that the user can select to switch to a different page in a dialog.
    \value PageTabList      A list of page tabs.
    \value Pane             A generic container.
    \value PopupMenu        A menu which lists options that the user can select to perform an action.
    \value ProgressBar      The object displays the progress of an operation in progress.
    \value PropertyPage     A property page where the user can change options and settings.
    \value Button           A button.
    \value RadioButton      An object that represents an option that is mutually exclusive with other options.
    \value Row              A row of cells, usually within a table.
    \value RowHeader        A header for a row of data.
    \value ScrollBar        A scroll bar, which allows the user to scroll the visible area.
    \value Separator        A separator that divides space into logical areas.
    \value Slider           A slider that allows the user to select a value within a given range.
    \value Sound            An object that represents a sound.
    \value SpinBox          A spin box widget that allows the user to enter a value within a given range.
    \value Splitter         A splitter distributing available space between its child widgets.
    \value StaticText       Static text, such as labels for other widgets.
    \value StatusBar        A status bar.
    \value Table            A table representing data in a grid of rows and columns.
    \value Terminal         A terminal or command line interface.
    \value TitleBar         The title bar caption of a window.
    \value ToolBar          A tool bar, which groups widgets that the user accesses frequently.
    \value ToolTip          A tool tip which provides information about other objects.
    \value Tree             A list of items in a tree structure.
    \value TreeItem         An item in a tree structure.
    \value UserRole         The first value to be used for user defined roles.
    \value Whitespace       Blank space between other objects.
    \value Window           A top level window.
*/

/*!
    \enum QAccessible::RelationFlag

    This enum type defines bit flags that can be combined to indicate
    the relationship between two accessible objects.

    \value Label            The first object is the label of the second object.
    \value Labelled         The first object is labelled by the second object.
    \value Controller       The first object controls the second object.
    \value Controlled       The first object is controlled by the second object.
    \value AllRelations     Used as a mask to specify that we are interesting in information
                            about all relations

    Implementations of relations() return a combination of these flags.
    Some values are mutually exclusive.
*/

/*!
    \enum QAccessible::Text

    This enum specifies string information that an accessible object
    returns.

    \value Name         The name of the object. This can be used both
                        as an identifier or a short description by
                        accessible clients.
    \value Description  A short text describing the object.
    \value Value        The value of the object.
    \value Help         A longer text giving information about how to use the object.
    \value Accelerator  The keyboard shortcut that executes the object's default action.
    \value UserText     The first value to be used for user defined text.
    \omitvalue DebugDescription
*/

/*!
    \enum QAccessible::InterfaceType

    \l QAccessibleInterface supports several sub interfaces.
    In order to provide more information about some objects, their accessible
    representation should implement one or more of these interfaces.
    When subclassing one of these interfaces, \l QAccessibleInterface::interface_cast also needs to be implemented.

    \value TextInterface            For text that supports selections or is more than one line. Simple labels do not need to implement this interface.
    \value EditableTextInterface    For text that can be edited by the user.
    \value ValueInterface           For objects that are used to manipulate a value, for example slider or scroll bar.
    \value ActionInterface          For interactive objects that allow the user to trigger an action. Basically everything that allows for example mouse interaction.
    \omitvalue ImageInterface       For objects that represent an image. This interface is generally less important.
    \value TableInterface           For lists, tables and trees.
    \value TableCellInterface       For cells in a TableInterface object.

    \sa QAccessibleInterface::interface_cast, QAccessibleTextInterface, QAccessibleEditableTextInterface, QAccessibleValueInterface, QAccessibleActionInterface, QAccessibleTableInterface, QAccessibleTableCellInterface
*/

/*!
    \fn QAccessibleInterface::~QAccessibleInterface()

    Destroys the object.
*/




/* accessible widgets plugin discovery stuff */
#ifndef QT_NO_LIBRARY
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader,
    (QAccessibleFactoryInterface_iid, QLatin1String("/accessible")))
#endif

Q_GLOBAL_STATIC(QList<QAccessible::InterfaceFactory>, qAccessibleFactories)

QAccessible::UpdateHandler QAccessible::updateHandler = 0;
QAccessible::RootObjectHandler QAccessible::rootObjectHandler = 0;

static bool accessibility_active = false;
static bool cleanupAdded = false;

static QPlatformAccessibility *platformAccessibility()
{
    QPlatformIntegration *pfIntegration = QGuiApplicationPrivate::platformIntegration();
    return pfIntegration ? pfIntegration->accessibility() : 0;
}

/*!
    \internal
*/
void QAccessible::cleanup()
{
    if (QPlatformAccessibility *pfAccessibility = platformAccessibility())
        pfAccessibility->cleanup();
}

static void qAccessibleCleanup()
{
    qAccessibleFactories()->clear();
}

/*!
    \typedef QAccessible::InterfaceFactory

    This is a typedef for a pointer to a function with the following
    signature:

    \snippet code/src_gui_accessible_qaccessible.cpp 1

    The function receives a QString and a QObject pointer, where the
    QString is the key identifying the interface. The QObject is used
    to pass on to the QAccessibleInterface so that it can hold a reference
    to it.

    If the key and the QObject does not have a corresponding
    QAccessibleInterface, a null-pointer will be returned.

    Installed factories are called by queryAccessibilityInterface() until
    one provides an interface.
*/

/*!
    \typedef QAccessible::UpdateHandler

    \internal

    A function pointer type. Use a function with this prototype to install
    your own update function.

    The function is called by updateAccessibility().
*/

/*!
    \typedef QAccessible::RootObjectHandler

    \internal

    A function pointer type. Use a function with this prototype to install
    your own root object handler.

    The function is called by setRootObject().
*/


/*!
    Installs the InterfaceFactory \a factory. The last factory added
    is the first one used by queryAccessibleInterface().
*/
void QAccessible::installFactory(InterfaceFactory factory)
{
    if (!factory)
        return;

    if (!cleanupAdded) {
        qAddPostRoutine(qAccessibleCleanup);
        cleanupAdded = true;
    }
    if (qAccessibleFactories()->contains(factory))
        return;
    qAccessibleFactories()->append(factory);
}

/*!
    Removes \a factory from the list of installed InterfaceFactories.
*/
void QAccessible::removeFactory(InterfaceFactory factory)
{
    qAccessibleFactories()->removeAll(factory);
}

/*!
    \internal

    Installs the given \a handler as the function to be used by
    updateAccessibility(), and returns the previously installed
    handler.
*/
QAccessible::UpdateHandler QAccessible::installUpdateHandler(UpdateHandler handler)
{
    UpdateHandler old = updateHandler;
    updateHandler = handler;
    return old;
}

/*!
    Installs the given \a handler as the function to be used by setRootObject(),
    and returns the previously installed handler.
*/
QAccessible::RootObjectHandler QAccessible::installRootObjectHandler(RootObjectHandler handler)
{
    RootObjectHandler old = rootObjectHandler;
    rootObjectHandler = handler;
    return old;
}

/*!
    If a QAccessibleInterface implementation exists for the given \a object,
    this function returns a pointer to the implementation; otherwise it
    returns 0.

    The function calls all installed factory functions (from most
    recently installed to least recently installed) until one is found
    that provides an interface for the class of \a object. If no
    factory can provide an accessibility implementation for the class
    the function loads installed accessibility plugins, and tests if
    any of the plugins can provide the implementation.

    If no implementation for the object's class is available, the
    function tries to find an implementation for the object's parent
    class, using the above strategy.

    \warning The caller is responsible for deleting the returned
    interface after use.
*/
QAccessibleInterface *QAccessible::queryAccessibleInterface(QObject *object)
{
    accessibility_active = true;
    if (!object)
        return 0;

    const QMetaObject *mo = object->metaObject();
    while (mo) {
        const QString cn = QLatin1String(mo->className());
        for (int i = qAccessibleFactories()->count(); i > 0; --i) {
            InterfaceFactory factory = qAccessibleFactories()->at(i - 1);
            if (QAccessibleInterface *iface = factory(cn, object))
                return iface;
        }
#ifndef QT_NO_LIBRARY
        if (QAccessibleInterface * iface = qLoadPlugin1<QAccessibleInterface, QAccessiblePlugin>(loader(), cn, object))
            return iface;
#endif
        mo = mo->superClass();
    }

    if (object == qApp)
        return new QAccessibleApplication;

    return 0;
}

/*!
    Returns true if an accessibility implementation has been requested
    during the runtime of the application; otherwise returns false.

    Use this function to prevent potentially expensive notifications via
    updateAccessibility().
*/
bool QAccessible::isActive()
{
    return accessibility_active;
}



/*!
  Sets the root accessible object of this application to \a object.
  All other accessible objects in the application can be reached by the
  client using object navigation.

  You should never need to call this function. Qt sets the QApplication
  object as the root object immediately before the event loop is entered
  in QApplication::exec().

  Use QAccessible::installRootObjectHandler() to redirect the function
  call to a customized handler function.

  \sa queryAccessibleInterface()
*/
void QAccessible::setRootObject(QObject *object)
{
    if (rootObjectHandler) {
        rootObjectHandler(object);
        return;
    }

    if (QPlatformAccessibility *pfAccessibility = platformAccessibility())
        pfAccessibility->setRootObject(object);
}

/*!
  \fn void QAccessible::updateAccessibility(QObject *object, int child, Event reason)
  \deprecated

  Use the version with a single \l QAccessibleEvent paremeter instead.
*/

/*!
  Notifies about a change that might be relevant for accessibility clients.

  \a event gives the details about the change.
  This includes the source of the change and what the actual change is.
  There should be sufficient details delivered with this event to give meaningful notifications.

  For example, the type \c ValueChange indicates that the position of
  a slider has been changed.

  Call this function whenever the state of your accessible object or
  one of its sub-elements has been changed either programmatically
  (e.g. by calling QLabel::setText()) or by user interaction.

  If there are no accessibility tools listening to this event, the
  performance penalty for calling this function is small, but if determining
  the parameters of the call is expensive you can test isActive() to
  avoid unnecessary computations.
*/
void QAccessible::updateAccessibility(QAccessibleEvent *event)
{
    if (updateHandler) {
        updateHandler(event);
        return;
    }

    if (!isActive())
        return;

    if (QPlatformAccessibility *pfAccessibility = platformAccessibility())
        pfAccessibility->notifyAccessibilityUpdate(event);
}

/*!
    \class QAccessibleEvent
    \brief The QAccessibleEvent is use to notify about changes that are
    relevant for accessibility in the application.
    \internal

    \ingroup accessibility
    \inmodule QtGui

    This class should be created on the stack and used as parameter for
    \l QAccessible::updateAccessibility().
    \sa QAccessibleStateChangedEvent
*/

/*!
    \class QAccessibleStateChangedEvent
    \brief This subclass of QAccessibleEvent is used to inform about state changes.
    \internal

    \ingroup accessibility
    \inmodule QtGui

    This class should be created on the stack and used as parameter for
    \l QAccessible::updateAccessibility().
    In addition to the regular \l QAccessibleEvent it contains details about which states
    changed.
    \sa QAccessibleEvent
*/

/*!
    \fn QAccessibleStateChangeEvent::changedStates() const
    All states that have changed are set to true. This does not reflect the state of the object,
    but indicates which states are changed.
    Use the \l QAccessibleInterface::state() function to get the current state.
 */

/*!
    Returns the QAccessibleInterface associated with the event.

    The caller of this function takes ownership of the returned interface.
*/
QAccessibleInterface *QAccessibleEvent::accessibleInterface() const
{
    QAccessibleInterface *iface = QAccessible::queryAccessibleInterface(m_object);
    if (!iface)
        return 0;

    if (m_child >= 0) {
        QAccessibleInterface *child = iface->child(m_child);
        if (child) {
            delete iface;
            iface = child;
        } else {
            qWarning() << "Cannot creat accessible child interface for object: " << m_object << " index: " << m_child;
        }
    }
    return iface;
}

/*!
    \class QAccessibleInterface
    \brief The QAccessibleInterface class defines an interface that exposes information
    about accessible objects.
    \internal

    \ingroup accessibility
    \inmodule QtGui

    This class is part of \l {Accessibility for QWidget Applications}.

    Accessibility tools (also called AT Clients), such as screen readers
    or braille displays, require high-level information about
    accessible objects in an application. Accessible objects provide
    specialized input and output methods, making it possible for users
    to use accessibility tools with enabled applications (AT Servers).

    Every element that the user needs to interact with or react to is
    an accessible object, and should provide this information. These
    are mainly visual objects, such as widgets and widget elements, but
    can also be content, such as sounds.

    The AT client uses three basic concepts to acquire information
    about any accessible object in an application:
    \list
    \li \e Properties The client can read information about
    accessible objects. In some cases the client can also modify these
    properties; such as text in a line edit.
    \li \e Actions The client can invoke actions like pressing a button
    or .
    \li \e{Relationships and Navigation} The client can traverse from one
    accessible object to another, using the relationships between objects.
    \endlist

    The QAccessibleInterface defines the API for these three concepts.

    \section1 Relationships and Navigation

    The functions childCount() and indexOfChild() return the number of
    children of an accessible object and the index a child object has
    in its parent. The childAt() function returns a child QAccessibleInterface
    that is found at a position. The child does not have to be a direct
    child. This allows bypassing intermediate layers when the parent already knows the
    top-most child. childAt() is used for hit testing (finding the object
    under the mouse).

    The relations() function provides information about the relations an
    object has to other objects, and parent() and child() allows
    traversing from one object to another object.

    \section1 Properties

    The central property of an accessible objects is what role() it
    has. Different objects can have the same role, e.g. both the "Add
    line" element in a scroll bar and the \c OK button in a dialog have
    the same role, "button". The role implies what kind of
    interaction the user can perform with the user interface element.

    An object's state() property is a combination of different state
    flags and can describe both how the object's state differs from a
    "normal" state, e.g. it might be unavailable, and also how it
    behaves, e.g. it might be selectable.

    The text() property provides textual information about the object.
    An object usually has a name, but can provide extended information
    such as a description, help text, or information about any
    keyboard accelerators it provides. Some objects allow changing the
    text() property through the setText() function, but this
    information is in most cases read-only.

    The rect() property provides information about the geometry of an
    accessible object. This information is usually only available for
    visual objects.

    \section1 Interfaces

    To enable the user to interact with an accessible object the
    object must implement QAccessibleActionInterface in addition to
    QAccessibleInterface.
    Objects that support selections can define actions to change the selection.

    There are several other interfaces that should be implemented as required.
    QAccessibleTextInterface should be used for bigger texts edits such as document views.
    This interface should not be implemented for labels/single line edits.
    The complementary QAccessibleEditableTextInterface should be added when the
    Text is editable.

    For sliders, scrollbars and other numerical value selectors QAccessibleValueInterface
    should be implemented.

    Lists, tables and trees should implement QAccessibleTableInterface.

    \sa QAccessible, QAccessibleActionInterface, QAccessibleTextInterface, QAccessibleEditableTextInterface, QAccessibleValueInterface, QAccessibleTableInterface
*/

/*!
    \fn bool QAccessibleInterface::isValid() const

    Returns true if all the data necessary to use this interface
    implementation is valid (e.g. all pointers are non-null);
    otherwise returns false.

    \sa object()
*/

/*!
    \fn QObject *QAccessibleInterface::object() const

    Returns a pointer to the QObject this interface implementation provides
    information for.

    \sa isValid()
*/

/*!
    \fn int QAccessibleInterface::childCount() const

    Returns the number of children that belong to this object. A child
    can provide accessibility information on its own (e.g. a child
    widget), or be a sub-element of this accessible object.

    All objects provide this information.

    \sa indexOfChild()
*/

/*!
    \fn int QAccessibleInterface::indexOfChild(const QAccessibleInterface *child) const

    Returns the 0-based index of the object \a child in this object's
    children list, or -1 if \a child is not a child of this object.

    All objects provide this information about their children.

    \sa childCount()
*/

/*!
    Returns the meaningful relations to other widgets. Usually this will not return parent/child
    relations, unless they are handled in a specific way such as in tree views.
    It will typically return the labelled-by and label relations.
    It should never return itself.

    \sa parent(), child()
*/
QVector<QPair<QAccessibleInterface*, QAccessible::Relation> >
QAccessibleInterface::relations(QAccessible::Relation /*match = QAccessible::AllRelations*/) const
{
    return QVector<QPair<QAccessibleInterface*, QAccessible::Relation> >();
}

/*!
    Returns the object that has the keyboard focus.

    The object returned can be any descendant, including itself.
*/
QAccessibleInterface *QAccessibleInterface::focusChild() const
{
    return 0;
}

/*!
    \fn QAccessibleInterface *QAccessibleInterface::childAt(int x, int y) const

    Returns the QAccessibleInterface of a child that contains the screen coordinates (\a x, \a y).
    If there are no children at this position this function returns 0.
    The returned accessible must be a child, but not necessarily a direct child.

    This function is only relyable for visible objects (invisible
    object might not be laid out correctly).

    All visual objects provide this information.

    A default implementation is provided for objects inheriting QAccessibleObject. This will iterate
    over all children. If the widget manages its children (e.g. a table) it will be more efficient
    to write a specialized implementation.

    \sa rect()
*/

/*!
    \fn QAccessibleInterface* QAccessibleInterface::parent() const

    Returns the QAccessibleInterface of the parent in the accessible object hierarchy.

    Returns 0 if no parent exists (e.g. for the top level application object).

    \sa child()
*/

/*!
    \fn QAccessibleInterface* QAccessibleInterface::child(int index) const

    Returns the accessible child with index \a index.
    0-based index. The number of children of an object can be checked with childCount.

    Returns 0 when asking for an invalid child (e.g. when the child became invalid in the meantime).

    \sa childCount(), parent()
*/

/*!
    \fn QString QAccessibleInterface::text(QAccessible::Text t) const

    Returns the value of the text property \a t of the object.

    The \l QAccessible::Name is a string used by clients to identify, find, or
    announce an accessible object for the user. All objects must have
    a name that is unique within their container. The name can be
    used differently by clients, so the name should both give a
    short description of the object and be unique.

    An accessible object's \l QAccessible::Description provides textual information
    about an object's visual appearance. The description is primarily
    used to provide greater context for vision-impaired users, but is
    also used for context searching or other applications. Not all
    objects have a description. An "OK" button would not need a
    description, but a tool button that shows a picture of a smiley
    would.

    The \l QAccessible::Value of an accessible object represents visual information
    contained by the object, e.g. the text in a line edit. Usually,
    the value can be modified by the user. Not all objects have a
    value, e.g. static text labels don't, and some objects have a
    state that already is the value, e.g. toggle buttons.

    The \l QAccessible::Help text provides information about the function and
    usage of an accessible object. Not all objects provide this
    information.

    The \l QAccessible::Accelerator is a keyboard shortcut that activates the
    object's default action. A keyboard shortcut is the underlined
    character in the text of a menu, menu item or widget, and is
    either the character itself, or a combination of this character
    and a modifier key like Alt, Ctrl or Shift. Command controls like
    tool buttons also have shortcut keys and usually display them in
    their tooltip.

    All objects provide a string for \l QAccessible::Name.

    \sa role(), state()
*/

/*!
    \fn void QAccessibleInterface::setText(QAccessible::Text t, const QString &text)

    Sets the text property \a t of the object to \a text.

    Note that the text properties of most objects are read-only
    so calling this function might have no effect.

    \sa text()
*/

/*!
    \fn QRect QAccessibleInterface::rect() const

    Returns the geometry of the object. The geometry is in screen coordinates.

    This function is only reliable for visible objects (invisible
    objects might not be laid out correctly).

    All visual objects provide this information.

    \sa childAt()
*/

/*!
    \fn QAccessible::Role QAccessibleInterface::role() const

    Returns the role of the object.
    The role of an object is usually static.

    All accessible objects have a role.

    \sa text(), state()
*/

/*!
    \fn QAccessible::State QAccessibleInterface::state() const

    Returns the current state of the object.
    The returned value is a combination of the flags in
    the QAccessible::StateFlag enumeration.

    All accessible objects have a state.

    \sa text(), role()
*/


/*!
    \fn QAccessibleEditableTextInterface *QAccessibleInterface::editableTextInterface()
    \internal
*/

/*!
    Returns the accessible's foreground color if applicable or an invalid QColor.

    \sa backgroundColor()
*/
QColor QAccessibleInterface::foregroundColor() const
{
    return QColor();
}

/*!
    Returns the accessible's background color if applicable or an invalid QColor.

    \sa foregroundColor()
*/
QColor QAccessibleInterface::backgroundColor() const
{
    return QColor();
}

/*!
    \fn QAccessibleTextInterface *QAccessibleInterface::textInterface()
    \internal
*/

/*!
    \fn QAccessibleValueInterface *QAccessibleInterface::valueInterface()
    \internal
*/

/*!
    \fn QAccessibleTableInterface *QAccessibleInterface::tableInterface()
    \internal
*/

/*!
    \fn QAccessibleTableCellInterface *QAccessibleInterface::tableCellInterface()
    \internal
*/

/*!
    \fn QAccessibleActionInterface *QAccessibleInterface::actionInterface()
    \internal
*/

/*!
    \fn QAccessibleImageInterface *QAccessibleInterface::imageInterface()
    \internal
*/

/*!
    \class QAccessibleEvent

    \brief The QAccessibleEvent class is used to give detailed updates to the
    accessibility framework. It is used together with \l QAccessible::updateAccessibility.

    The event is one of the \l QAccessible::Event which depending on the type of event needs to use
    one of the subclasses of QAccessibleEvent.

    \ingroup accessibility
    \inmodule QtGui
*/

/*!
    \fn QAccessibleEvent::QAccessibleEvent(QAccessible::Event type, QObject *object, int child = -1)

    Constructs an accessibility event of the given \a type.
    It also requires an \a object as source of the event and optionally a \a child index,
    if the event comes from a child of the object.

    Using a \a child index maybe more efficient than creating the accessible interface for the child.
*/

/*!
    Returns the window associated with the underlying object.
    For instance, QAccessibleWidget reimplements this and returns
    the windowHandle() of the QWidget.

    It is used on some platforms to be able to notify the AT client about
    state changes.
    The backend will traverse up all ancestors until it finds a window.
    (This means that at least one interface among the ancestors should
    return a valid QWindow pointer).

    The default implementation of this returns 0.
    \preliminary
  */
QWindow *QAccessibleInterface::window() const
{
    return 0;
}

/*!
    \since 4.2

    Invokes a \a method on \a child with the given parameters \a params
    and returns the result of the operation as QVariant.

    Note that the type of the returned QVariant depends on the action.

    Returns an invalid QVariant if the object doesn't support the action.
*/

/*!
    \internal
    Method to allow extending this class without breaking binary compatibility.
    The actual behavior and format of \a data depends on \a id argument
    which must be defined if the class is to be extended with another virtual
    function.
    Currently, this is unused.
*/
void QAccessibleInterface::virtual_hook(int /*id*/, void * /*data*/)
{
}

/*!
    \fn void *QAccessibleInterface::interface_cast(QAccessible::InterfaceType type)

    \brief Returns a specialized accessibility interface \a type from the generic QAccessibleInterface.

    This function must be reimplemented when providing more information about a widget or object through the
    specialized interfaces. For example a line edit should implement the QAccessibleTextInterface and QAccessibleEditableTextInterface.

    Qt's QLineEdit for example has its accessibility support implemented in QAccessibleLineEdit.
    \code
void *QAccessibleLineEdit::interface_cast(QAccessible::InterfaceType t)
{
    if (t == QAccessible::TextInterface)
        return static_cast<QAccessibleTextInterface*>(this);
    else if (t == QAccessible::EditableTextInterface)
        return static_cast<QAccessibleEditableTextInterface*>(this);
    return QAccessibleWidget::interface_cast(t);
}
    \endcode

    \sa QAccessible::InterfaceType, QAccessibleTextInterface, QAccessibleEditableTextInterface, QAccessibleValueInterface, QAccessibleActionInterface, QAccessibleTableInterface, QAccessibleTableCellInterface
  */

/*! \internal */
const char *qAccessibleRoleString(QAccessible::Role role)
{
    if (role >=0x40)
         role = QAccessible::UserRole;
    static int roleEnum = QAccessible::staticMetaObject.indexOfEnumerator("Role");
    return QAccessible::staticMetaObject.enumerator(roleEnum).valueToKey(role);
}

/*! \internal */
const char *qAccessibleEventString(QAccessible::Event event)
{
    static int eventEnum = QAccessible::staticMetaObject.indexOfEnumerator("Event");
    return QAccessible::staticMetaObject.enumerator(eventEnum).valueToKey(event);
}

/*! \internal */
bool operator==(const QAccessible::State &first, const QAccessible::State &second)
{
    return memcmp(&first, &second, sizeof(QAccessible::State)) == 0;
}

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug d, const QAccessibleInterface *iface)
{
    if (!iface) {
        d << "QAccessibleInterface(null)";
        return d;
    }
    d.nospace();
    d << "QAccessibleInterface(" << hex << (void *) iface << dec;
    if (iface->isValid()) {
        d << " name=" << iface->text(QAccessible::Name) << " ";
        d << "role=" << qAccessibleRoleString(iface->role()) << " ";
        if (iface->childCount())
            d << "childc=" << iface->childCount() << " ";
        if (iface->object()) {
            d << "obj=" << iface->object();
        }
        QStringList stateStrings;
        QAccessible::State st = iface->state();
        if (st.focusable)
            stateStrings << QLatin1String("focusable");
        if (st.focused)
            stateStrings << QLatin1String("focused");
        if (st.selected)
            stateStrings << QLatin1String("selected");
        if (st.invisible)
            stateStrings << QLatin1String("invisible");

        if (!stateStrings.isEmpty())
            d << stateStrings.join(QLatin1String("|"));

        if (!st.invisible)
            d << "rect=" << iface->rect();

    } else {
        d << " invalid";
    }
    d << ")";
    return d.space();
}

/*! \internal */
QDebug operator<<(QDebug d, const QAccessibleEvent &ev)
{
    if (!&ev) {
        d << "QAccessibleEvent(null)";
        return d;
    }
    d.nospace() << "QAccessibleEvent(object=" << hex << ev.object();
    d.nospace() << dec;
    d.nospace() << "child=" << ev.child();
    d << " event=" << qAccessibleEventString(ev.type());
    if (ev.type() == QAccessible::StateChanged) {
        QAccessible::State changed = static_cast<const QAccessibleStateChangeEvent*>(&ev)->changedStates();
        d << "State changed:";
        if (changed.disabled) d << "disabled";
        if (changed.selected) d << "selected";
        if (changed.focusable) d << "focusable";
        if (changed.focused) d << "focused";
        if (changed.pressed) d << "pressed";
        if (changed.checkable) d << "checkable";
        if (changed.checked) d << "checked";
        if (changed.checkStateMixed) d << "checkStateMixed";
        if (changed.readOnly) d << "readOnly";
        if (changed.hotTracked) d << "hotTracked";
        if (changed.defaultButton) d << "defaultButton";
        if (changed.expanded) d << "expanded";
        if (changed.collapsed) d << "collapsed";
        if (changed.busy) d << "busy";
        if (changed.expandable) d << "expandable";
        if (changed.marqueed) d << "marqueed";
        if (changed.animated) d << "animated";
        if (changed.invisible) d << "invisible";
        if (changed.offscreen) d << "offscreen";
        if (changed.sizeable) d << "sizeable";
        if (changed.movable) d << "movable";
        if (changed.selfVoicing) d << "selfVoicing";
        if (changed.selectable) d << "selectable";
        if (changed.linked) d << "linked";
        if (changed.traversed) d << "traversed";
        if (changed.multiSelectable) d << "multiSelectable";
        if (changed.extSelectable) d << "extSelectable";
        if (changed.passwordEdit) d << "passwordEdit"; // used to be Protected
        if (changed.hasPopup) d << "hasPopup";
        if (changed.modal) d << "modal";

        // IA2 - we chose to not add some IA2 states for now
        // Below the ones that seem helpful
        if (changed.active) d << "active";
        if (changed.invalid) d << "invalid"; // = defunct
        if (changed.editable) d << "editable";
        if (changed.multiLine) d << "multiLine";
        if (changed.selectableText) d << "selectableText";
        if (changed.supportsAutoCompletion) d << "supportsAutoCompletion";

    }
    d.nospace() << ")";
    return d.space();
}

#endif

QT_END_NAMESPACE

#endif
