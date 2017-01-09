#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H
#include <QString>
#include "dataaccessobject.h"

class IPlaylistManager
{
public:
    virtual ~IPlaylistManager(){}
    virtual void setDataAccessObject(AbstractDataAccessObject* data_acces) = 0;
    virtual int createPlaylist(const QString& title) = 0;
    virtual bool removePlaylist(int playlistID) = 0;
    virtual bool addToPlaylist(int playlistID, int trackID) = 0;
    virtual bool removeFromPlaylist(int playlistID, int trackID) = 0;

    virtual bool removePlaylist(const QString& pls) = 0;
    virtual bool addToPlaylist(const QString& pls, int trackID) = 0;
    virtual bool removeFromPlaylist(const QString& pls, int trackID) = 0;
};

class Playlistmanager : public IPlaylistManager
{
public:
    Playlistmanager(AbstractDataAccessObject* data_acces);
    // IPlaylistManager interface
public:
    virtual void setDataAccessObject(AbstractDataAccessObject *data_acces) override;
    virtual int createPlaylist(const QString& title) override;
    virtual bool removePlaylist(int playlistID) override;
    virtual bool addToPlaylist(int playlistID, int trackID) override;
    virtual bool removeFromPlaylist(int playlistID, int trackID) override;
    virtual bool removePlaylist(const QString &pls) override;
    virtual bool addToPlaylist(const QString &pls, int trackID) override;
    virtual bool removeFromPlaylist(const QString &pls, int trackID) override;


private:
    AbstractDataAccessObject* data_acces;

};

#endif // PLAYLISTMANAGER_H
