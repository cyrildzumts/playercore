#ifndef DATAACCESSOBJECT_H
#define DATAACCESSOBJECT_H


#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <mutex>
#include <thread>
#include <iostream>
#include <exception>
#include <experimental/filesystem>
#include "tagreader.h"

/**
 * @brief The AbstractDataAccessObject class
 */
class AbstractDataAccessObject
{
public:
    virtual ~AbstractDataAccessObject(){}
    /**
     * @brief queryTrack query the track with id trackID from the database.
     * @param trackID the id of the trak we want to query
     * @return On success returns a populated QSqlQuery object
     *         On faillure returns an invalid QSqlQuery object
     */
    virtual QSqlQuery queryTrack(int trackID) = 0;
    /**
     * @brief queryTrack query the track with title "title" from the database.
     * @param title the title of the trak we want to query
     * @return On success returns a populated QSqlQuery object
     *         On faillure returns an invalid QSqlQuery object
     */
    virtual QSqlQuery queryTrack(const QString& title) = 0;

    virtual void addTrack(const QString& path) = 0;
    /**
     * @brief addTrack adds the track "track" into the database.
     * @param track the track to be added into the database.
     */
    virtual void addTrack(const Track& track) = 0;
    /**
     * @brief removeTrack removes the track who's id is trackID
     * @param trackID the id of the track to removed from the database.
     */
    virtual void removeTrack(int trackID) = 0;

    /**
     * @brief query executes a query on the database.
     * @param command the sql script to be executed.
     * @return return the result of the query
     */
    virtual QSqlQuery query(const QString &command) = 0;
    /**
     * @brief commit commits any changed applied to the database.
     */
    virtual void commit() = 0;
    /**
     * @brief isOpen query if the database is opened
     * @return true if the database is open
     *         false if the database is closed
     */
    virtual bool isOpen() const = 0;
    /**
     * @brief close closes the database.
     * before closing evry data would be commited.
     */
    virtual void close() = 0;

};


class DataAccessObject : public AbstractDataAccessObject
{
public:
    DataAccessObject(const QString &path);
    ~DataAccessObject();

    // AbstractDataAccessObject interface
public:
    virtual QSqlQuery queryTrack(const QString& title) override;
    virtual QSqlQuery queryTrack(int trackID) override;
    virtual void addTrack(const QString &path) override;
    virtual void addTrack(const Track &track) override;
    virtual void removeTrack(int trackID) override;
    virtual QSqlQuery query(const QString &command) override;
    virtual void commit() override;

    virtual bool isOpen() const override;
    virtual void close() override;

private:
    QString driver;
    QSqlDatabase db;
    /**
     * @brief  The Database which stores the meta data of the audio files
     */
    QSqlDatabase database;

    /**
     * @brief The query's object to be used to execute a query instead of creating
     * everytime a new query object when  I want to add a new track into the Database
     */
    QSqlQuery _query;

    /**
     * @brief add_query query object used to add / remove track in the
     * database.
     */
    QSqlQuery add_query;
    /**
     * @brief This variable is used only to mesure how long it takes to scan the whole
     * music directory
     */
    int loggerTime;

    /**
     * @brief  The date at which an Item is added into the Database.
     */
    int addedDate;

    /**
     * @brief The datae at which an Item is modified  into the Database.
     */
    int modifiedDate;

    /**
     * @brief The path to the database
     */
    QString databasePath;

    /**
     * @brief The name of our connection to the Database.
     */
    QString connectionName;

    /**
     * @brief databaseMutex is used to lock the database when the database is being accessed
     * from one Thread.
     */
    std::mutex databaseMutex;
    /**
     * @brief appName is the Application's Name
     */
    QString appName;

    /**
     * @brief configPath is the database configuration's path on the
     */
    QString configPath;


};



#endif // DATAACCESSOBJECT_H
