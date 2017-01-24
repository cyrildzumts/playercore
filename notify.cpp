#include "notify.h"
#include "Notify_Adaptor.h"

Notification::Notification(QObject *parent) :
    QObject(parent)
{
    notify_timeout = -1;
    notify_id = 1;
    notify_app_name = "Player";
    notify_actions << "";
    notify_hints = QVariantMap();

}

uint Notification::Notify(const QString &app_name, uint id, const QString &icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout)
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *interface = new QDBusInterface("org.freedesktop.Notifications",
                                                  "/org/freedesktop/Notifications",
                                                  "org.freedesktop.Notifications",bus, this);

    /*
     msg = QDBusMessage::createMethodCall("org.freedesktop.Notifications",
                                         "/org/freedesktop/Notifications",
                                         "org.freedesktop.Notifications",
                                         "Notify");
    */
    QList<QVariant> args;
    args << app_name << id << icon << summary.toStdString().data() << body.toStdString().data() << actions << hints << timeout;
    QDBusPendingReply<uint> reply = interface->asyncCallWithArgumentList("Notify", args);
    reply.waitForFinished();
     //interface->callWithCallback("Notify", args,this,SLOT(getId(QDBusReply<uint>)));
    //bool queued = QDBusConnection::sessionBus().send(msg);

        if(reply.isError())
        {
            qDebug()<< " Notification INVALID " ;
            qDebug()<< reply.error().message();
        }
        else
        {
            notify_id = reply.value();
            showNotifyID();

        }
    interface->deleteLater();
    return notify_id;
}

void Notification::setNotification(const QString &summary, const QString &body, const QString &icon)
{
    notify_body = body;
    notify_icon = icon;
    notify_summary = summary;
}

int Notification::sendNotify()
{
    return Notify(notify_app_name,notify_id,notify_icon.toStdString().data(),notify_summary.toStdString().data(),notify_body.toStdString().data(),notify_actions,notify_hints,notify_timeout);

}

void Notification::notificationSent()const
{
    qDebug() << "Notification sent from Notification" ;
}

void Notification::getId(QDBusReply<uint> notificationID)
{
    if(notificationID.isValid())
    {
        qDebug()<< " Notification VALIDE REPLY from Server";
        notify_id = notificationID.value();
        qDebug()<< " Notification ID : " << notify_id;
    }

    //Q_EMIT notifyIDChanged();
}

void Notification::showNotifyID()
{
    qDebug()<< " Notification ID : " << notify_id;
}
