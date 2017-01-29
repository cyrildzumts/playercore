#ifndef MEDIASCANNER_H
#define MEDIASCANNER_H

#include "dataaccessobject.h"
#include "common.h"
#include "tagreader.h"
#include <QDateTime>
#include <QFileSystemWatcher>

class IMediaScanner : public QObject
{
    Q_OBJECT
public:
    virtual ~IMediaScanner(){}

    virtual void init(const QString &sourcedir) = 0;
    virtual void scanSourceDirectory() = 0;
    virtual bool exec(const QString &cmd) = 0;
    virtual void setSourceDirectory(const QString &dirpath) = 0;
    virtual int getLastAddedDate() = 0;
    virtual int getLastModifiedDate() = 0;
    virtual void updateData() = 0;
    virtual void setDataAccessObject(AbstractDataAccessObject *data_access) = 0;
    virtual bool isSourceDirectoryConfigured() const = 0;
    virtual bool isSourceDirectoryScanned()const = 0;
    virtual bool hasDataAccessObject() const = 0;
};



class MediaScanner : public IMediaScanner
{
public:
    MediaScanner();
    MediaScanner(const QString &database = QString(), const QString &sourDir = QString());
    MediaScanner(AbstractDataAccessObject *data_access);
    virtual ~MediaScanner();

    // IMediaScanner interface
public:
    virtual void init(const QString &sourcedir);
    virtual bool exec(const QString &cmd);
    virtual void scanSourceDirectory() override;
    void scanSourceDirectory2();
    virtual void setSourceDirectory(const QString &dirpath) override;
    virtual int getLastAddedDate() override;
    virtual int getLastModifiedDate() override;
    int getLastAddedDate2();
    int getLastModifiedDate2();
    virtual void updateData() override;
    virtual void setDataAccessObject(AbstractDataAccessObject *data_access) override;
    virtual bool isSourceDirectoryConfigured() const override;
    virtual bool isSourceDirectoryScanned() const override;
    virtual bool hasDataAccessObject() const override;


// Utilities functions
private:
    /**
     * @brief createTables create Tables in the database
     * When the database is not opened an Exception is thrown
     */
    void createTables();

    void createCoverTable();

    void createGenreTable();

    void createArtistTable();

    void createAlbumTable();

    void createBaseTrackTable();

    void createPlaylistTable();

    void createPlaylistTracksTable();

    void createDirectoriesTable();

    void createSubDirectoriesTable();

    void setConfig();
    void databaseConfig();
    void setup();
    bool isDatabaseConfigured() const;
    void databaseConfig2();

private:
    void launchScanThread();
    void launchUpdateThread();
    void removeDeletedFile();
    std::vector<QString> getFileInSourceDir();
    std::vector<QString> getUrlFromDatabase();
    void setNewFiles(std::vector<QString> &lhs,
                     std::vector<QString> &rhs,
                     std::vector<QString> &newFiles);
    void addFileToDataAccessObject(std::vector<QString> &files);
    void print_difference(std::vector<QString> &lhs, std::vector<QString> &rhs)const;

private:
    AbstractDataAccessObject *data_access;
    std::vector<QString> directories;
    std::vector<QString> fileInDatabase;
    QString sourceDirPath;
    QSqlQuery queryScanner;

    /**
     * @brief The list of the supperted audio Format. This is limited to
     * files from which I can read the meta tag.
     */
    QStringList audioFormatFilter;

    /**
     * @brief updateThread runs update() in the background. I mainly use it to
     * update the database when the audio files's source Directory has changed.
     * Maybe I should use a future for that task.
     */
    std::thread updateThread;

    /**
     * @brief directoryScannThread runs scanSourceDirectory() in the background.
     */
    std::thread directoryScannThread;

    /**
     * @brief databaseMutex is used to lock the database when the database is being accessed
     * from one Thread.
     */
    std::mutex databaseMutex;

    /**
     * @brief directoryWatcher watches for any changes in the directory source.
     */
    QFileSystemWatcher *directoryWatcher;
};

#endif // MEDIASCANNER_H
