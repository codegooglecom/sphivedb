/*
 * Copyright 2009 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#ifndef __spdbmstore_hpp__
#define __spdbmstore_hpp__

#include <pthread.h>

#include "sphivestore.hpp"

class SP_NKTokenLockManager;
class SP_NKSortedArray;
class SP_NKDoubleLinkList;

typedef struct spmemvfs_db_t spmemvfs_db_t;

class SP_DbmStore {
public:

	const char * getPath();

	void * getDbm();

protected:

	SP_DbmStore( const char * path, void * dbm );
	~SP_DbmStore();

	void addRef();
	void release();
	int getRef();

	friend class SP_DbmStoreCache;

private:
	char * mPath;
	void * mDbm;

	int mRefCount;
};

typedef struct tagSP_DbmStoreCacheItem SP_DbmStoreCacheItem_t;

class SP_DbmStoreCache {
public:
	SP_DbmStoreCache( int maxCount );
	~SP_DbmStoreCache();

	SP_DbmStore * get( const char * path );

	int save( SP_DbmStore * file );

private:

	static int pathcmp( const void * item1, const void * item2 );

private:
	int mMaxCount;

	SP_NKTokenLockManager * mLockManager;

	pthread_mutex_t mMutex;
	SP_NKSortedArray * mItemList;
	SP_NKDoubleLinkList * mTimeList;
};

class SP_HiveConfig;

class SP_DbmStoreManager : public SP_HiveStoreManager {
public:
	SP_DbmStoreManager();
	virtual ~SP_DbmStoreManager();

	int init( SP_HiveConfig * config );

	virtual int load( SP_HiveReqObject * req, SP_HiveStore * store );

	virtual int save( SP_HiveReqObject * req, SP_HiveStore * store );

	virtual int close( SP_HiveStore * store );

private:
	const char * getPath( int dbfile, const char * dbname,
			char * path, int size );

	static int loadFromDbm( void * dbm, const char * path,
			spmemvfs_db_t * db, const char * dbname );

private:
	SP_DbmStoreCache * mCache;
	SP_HiveConfig * mConfig;
};

#endif

