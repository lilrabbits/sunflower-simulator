/*
 * align.h -- Exported time-aligner functions and data structures.
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
 * $Log$
 * 
 * 13-Sep-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Changed align_sen_active to flag active senones instead of building a list
 * 		of them.
 * 
 * 15-Jul-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Created.
 */


#ifndef _LIBFBS_ALIGN_H_
#define _LIBFBS_ALIGN_H_


#include <libutil/libutil.h>
#include "s3types.h"


/* State level segmentation/alignment; one entry per frame */
typedef struct align_stseg_s {
    s3pid_t pid;		/* Phone id */
    int8 state;			/* State id within phone */
    int8 start;			/* Whether this is an entry into phone start state */
    int32 score;		/* Acoustic score for transition to this state from the
				   previous one in the list */
    int32 bsdiff;		/* Distance between this state and the best in this frame */
    struct align_stseg_s *next;	/* Next state (in the next frame) in alignment */
} align_stseg_t;


/* Phone level segmentation/alignment information */
typedef struct align_phseg_s {
    s3pid_t pid;		/* Phone id */
    s3frmid_t sf, ef;		/* Start and end frame for this phone occurrence */
    int32 score;		/* Acoustic score for this segment of alignment */
    int32 bsdiff;		/* Distance between this and the best unconstrained state
				   sequence for the same time segment */
    struct align_phseg_s *next;	/* Next entry in alignment */
} align_phseg_t;


/* Word level segmentation/alignment information */
typedef struct align_wdseg_s {
    s3wid_t wid;		/* Word id */
    s3frmid_t sf, ef;		/* Start and end frame for this word occurrence */
    int32 score;		/* Acoustic score for this segment of alignment */
    int32 bsdiff;		/* Distance between this and the best unconstrained state
				   sequence for the same time segment */
    struct align_wdseg_s *next;	/* Next entry in alignment */
} align_wdseg_t;


int32 align_init ( void );

int32 align_build_sent_hmm (char *transcript);

int32 align_destroy_sent_hmm ( void );

int32 align_start_utt (char *uttid);

/*
 * Called at the beginning of a frame to flag the active senones (any senone used
 * by active HMMs) in that frame.
 */
void align_sen_active (s3senid_t *senlist,	/* Out: senlist[s] TRUE iff active in frame */
		       int32 n_sen);		/* In: Size of senlist[] array */


/* Step time aligner one frame forward */
int32 align_frame (int32 *senscr);		/* In: array of senone scores this frame */


/*
 * Wind up utterance and return final result (READ-ONLY).  Results only valid until
 * the next utterance is begun.
 */
int32 align_end_utt (align_stseg_t **stseg,	/* Out: list of state segmentation */
		     align_phseg_t **phseg,  	/* Out: list of phone segmentation */
		     align_wdseg_t **wdseg);	/* Out: list of word segmentation */


#endif
