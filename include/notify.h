#ifndef NOTIFY_H
#define NOTIFY_H

#include <QObject>
#include <QtDBus>
class Notification : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.Notifications")
public:
    explicit Notification(QObject *parent = 0);

Q_SIGNALS:
void notifyIDChanged(int);

public Q_SLOTS:
    void notificationSent()const;
    //QStringList GetCapabilities();
    //QString GetServerInformation(QString &return_vendor, QString &return_version, QString &return_spec_version);
    uint Notify(const QString &app_name, uint id, const QString &icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout);

   void setNotification(QString &arg_summary
                        ,
                        QString &arg_body,
                        QString &arg_icon);


   int sendNotify();
   void showNotifyID ();
   void getId(QDBusReply<uint> notificationID);
private:
   QString notify_body;
   QString notify_app_name;
   QString notify_icon;
   QString notify_summary;
   int notify_timeout;
   int notify_id;
   QVariantMap notify_hints;
   QStringList notify_actions;
   QDBusMessage notify_msg;
};

#endif // NOTIFY_H
