#ifndef INCLUDED_DYNSTATS_H
#define INCLUDED_DYNSTATS_H

#include <search.h>
#include <sys/queue.h>

typedef struct hsearch_data htable;

struct dynstats_ctr_s {
    STATSCOUNTER_DEF(ctr, mutCtr);
    SLIST_ENTRY(dynstats_ctr_s) link;
};

struct dynstats_bucket_s {
	htable table;
    uchar *name;
	pthread_rwlock_t lock;
	statsobj_t *stats;
	STATSCOUNTER_DEF(ctrOpsOverflow, mutCtrOpsOverflow);
	ctr_t *pOpsOverflowCtr;
	STATSCOUNTER_DEF(ctrNewMetricAdd, mutCtrNewMetricAdd);
	ctr_t *pNewMetricAddCtr;
	SLIST_ENTRY(dynstats_bucket_s) link;
    SLIST_HEAD(, dynstats_ctr_s) ctrs;
    uint32_t maxCardinality;
    uint32_t metricCount;
    pthread_mutex_t mutMetricCount;
    uint32_t unusedMetricLife;
    uint32_t lastResetTs;
    struct timespec metricCleanupTimeout;
    uint8_t resettable;
};

struct dynstats_buckets_s {
	SLIST_HEAD(, dynstats_bucket_s) list;
	statsobj_t *global_stats;
	STATSCOUNTER_DEF(metricsAdded, mutMetricsAdded);
	STATSCOUNTER_DEF(metricsPurged, mutMetricsPurged);
    pthread_rwlock_t lock;
};

rsRetVal dynstats_initCnf(dynstats_buckets_t *b);
rsRetVal dynstats_processCnf(struct cnfobj *o);
dynstats_bucket_t * dynstats_findBucket(const uchar* name);
rsRetVal dynstats_inc(dynstats_bucket_t *bucket, uchar* metric);
void dynstats_destroyAllBuckets();
void dynstats_resetExpired();
rsRetVal dynstatsClassInit(void);

#endif /* #ifndef INCLUDED_DYNSTATS_H */


