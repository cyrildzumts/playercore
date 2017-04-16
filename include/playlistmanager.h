#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H
#include <QString>
#include "dataaccessobject.h"

class IPlaylistManager
{
public:
    virtual ~IPlaylistManager(){}
    /**
     * @brief setDataAccessObject
     * @param data_acces
     */
    virtual void setDataAccessObject(AbstractDataAccessObject* data_acces) = 0;
    /**
     * @brief createPlaylist
     * @param title
     * @return
     */
    virtual int createPlaylist(const QString& title) = 0;
    /**
     * @brief removePlaylist
     * @param playlistID
     * @return
     */
    virtual bool removePlaylist(int playlistID) = 0;
    /**
     * @brief addToPlaylist
     * @param playlistID
     * @param trackID
     * @return
     */
    virtual bool addToPlaylist(int playlistID, int trackID) = 0;
    /**
     * @brief removeFromPlaylist
     * @param playlistID
     * @param trackID
     * @return
     */
    virtual bool removeFromPlaylist(int playlistID, int trackID) = 0;
    /**
     * @brief removePlaylist
     * @param pls
     * @return
     */

    virtual bool removePlaylist(const QString& pls) = 0;
    /**
     * @brief addToPlaylist
     * @param pls
     * @param trackID
     * @return
     */
    virtual bool addToPlaylist(const QString& pls, int trackID) = 0;
    /**
     * @brief removeFromPlaylist
     * @param pls
     * @param trackID
     * @return
     */
    virtual bool removeFromPlaylist(const QString& pls, int trackID) = 0;
};

class Playlistmanager : public IPlaylistManager
{
public:
    Playlistmanager(AbstractDataAccessObject* data_acces);
    ~Playlistmanager();
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
