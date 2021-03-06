/*
 * senone.h -- Mixture density weights associated with each tied state.
 *
 * **********************************************
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * ALL RIGHTS RESERVED.
 * **********************************************
 * 
 * HISTORY
 * 
 * $Log: senone.h,v $
 * Revision 1.2  2003/01/07 04:22:18  ffang
 * My changes initial commit
 *
 * 
 * 13-Dec-95	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University.
 * 		Added senone_eval_all().
 * 
 * 12-Nov-95	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University.
 * 		Created.
 */


#ifndef _LIBFBS_SENONE_H_
#define _LIBFBS_SENONE_H_


#include <libutil/prim_type.h>
#include <s3.h>

#include "s3types.h"
#include "gauden.h"


typedef uint8 senprob_t;	/* Senone logs3-probs, truncated to 8 bits */

/*
 * 8-bit senone PDF structure.  Senone pdf values are normalized, floored, converted to
 * logs3 domain, and finally truncated to 8 bits precision to conserve memory space.
 */
typedef struct {
    senprob_t ***pdf;		/* gaussian density mixture weights, organized two possible
				   ways depending on n_gauden:
				   if (n_gauden > 1): pdf[sen][feat][codeword].  Not an
				       efficient representation--memory access-wise--but
				       evaluating the many codebooks will be more costly.
				   if (n_gauden == 1): pdf[feat][codeword][sen].  Optimized
				       for the shared-distribution semi-continuous case. */
    int32 n_sen;		/* #senones in this set */
    int32 n_feat;		/* #feature streams */ 
    int32 n_cw;			/* #codewords per codebook,stream */
    int32 n_gauden;		/* #gaussian density codebooks referred to by senones */
    float32 mixwfloor;		/* floor applied to each PDF entry */
    int32 shift;		/* LSB bits truncated from original logs3 value */
    s3mgauid_t *mgau;		/* senone-id -> mgau-id mapping for senones in this set */
} senone_t;


/*
 * Load a set of senones (mixing weights and mixture gaussian codebook mappings) from
 * the given files.  Normalize weights for each codebook, apply the given floor, convert
 * PDF values to logs3 domain and quantize to 8-bits.
 * Return value: pointer to senone structure created.  Caller MUST NOT change its contents.
 */
senone_t *senone_init (char *mixwfile,		/* In: mixing weights file */
		       char *mgau_mapfile,	/* In: file specifying mapping from each
						   senone to mixture gaussian codebook.
						   If NULL all senones map to codebook 0 */
		       float32 mixwfloor);	/* In: Floor value for senone weights */

/*
 * Evaluate the score for the given senone wrt to the given top N gaussian codewords.
 * Return value: senone score (in logs3 domain).
 */
int32 senone_eval (senone_t *s, s3senid_t id,	/* In: senone for which score desired */
		   gauden_dist_t **dist,	/* In: top N codewords and densities for
						   all features, to be combined into
						   senone score.  IE, dist[f][i] = i-th
						   best <codeword,density> for feaure f */
		   int32 n_top);		/* In: Length of dist[f], for each f */

/*
 * Like senone_eval, but compute all senone scores for the shared density case (ie,
 * #codebooks = 1).
 */
void senone_eval_all (senone_t *s,		/* In: Senone structure */
		      gauden_dist_t **dist,	/* In: as in senone_eval above */
		      int32 n_top,		/* In: as in senone_eval above */
		      int32 *senscr);		/* Out: Upon return, senscr[i] will contain
						   score for senone i */

#endif
