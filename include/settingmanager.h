#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QFileSystemWatcher>
#include <QDebug>
#include <vector>


class ISettingManager
{
public:
    virtual ~ISettingManager(){}
        virtual QString getDatabasePath()const = 0;
        virtual QString getMediaSourcePath()const = 0;
        virtual void setDatabasePath(const QString &path) = 0;
        virtual void setMediaSourcePath(const QString &path) = 0;
        virtual void setConfigPath(const QString &conf) = 0;
        virtual QString getConfigPath() const = 0;
        virtual void updateConfig() = 0;
        virtual bool isReady()const = 0;
        virtual QString applicationName()const = 0;
        virtual QString applicationVersion()const = 0;
        virtual QString authorName()const = 0;
        virtual QString authorEmail()const = 0;
        virtual int playerPosition()const = 0;
        virtual void setPlayerPosition(int position) = 0;
        virtual int playerTrackIndex()const = 0;
        virtual void setPlayerTrackIndex(int index) = 0;
        virtual int playerPlaybackMode()const = 0;
        virtual void setPlayerPlaybackMode(int mode) = 0;
        virtual std::vector<int> playlistContent()const = 0;
        virtual void setPlaylistContent(std::vector<int> &tracklist) = 0;

};


class SettingManager : public ISettingManager
{

public:
    SettingManager();
    SettingManager(const QString & confPath);
    virtual ~SettingManager();


    // ISettingManager interface
public:
    virtual QString getDatabasePath() const override;
    virtual QString getMediaSourcePath() const override;
    virtual void setDatabasePath(const QString &path) override;
    virtual void setMediaSourcePath(const QString &path) override;
    virtual void setConfigPath(const QString &conf) override;
    virtual QString getConfigPath() const override;
    virtual void updateConfig() override;
    virtual bool isReady() const override;
    virtual QString applicationName() const override;
    virtual QString applicationVersion() const override;
    virtual QString authorName() const override;
    virtual QString authorEmail() const override;
    virtual int playerPosition() const override;
    virtual int playerTrackIndex() const override;
    virtual int playerPlaybackMode() const override;
    virtual std::vector<int> playlistContent() const override;
    virtual void setPlaylistContent(std::vector<int> &tracklist) override;
    virtual void setPlayerPosition(int position) override;
    virtual void setPlayerTrackIndex(int index) override;
    virtual void setPlayerPlaybackMode(int mode) override;

private:
    QJsonObject getRootObject() const;
    void readJsonDocument(QJsonDocument &doc);
    /**
     * @brief init
     * intialize the Settingscontroller.
     * Throw InexistentConfigException when conf doesn#t point
     * to an existing file
     */
    void init();

protected:
    QString database_path;
    QString mediaSource_path;
    QString conf;
    QFileSystemWatcher *watcher;

    QFile file;

};

#endif // SETTINGMANAGER_H
