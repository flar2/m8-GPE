/*
 * sufile.h - NILFS segment usage file.
 *
 * Copyright (C) 2006-2008 Nippon Telegraph and Telephone Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Written by Koji Sato <koji@osrg.net>.
 */

#ifndef _NILFS_SUFILE_H
#define _NILFS_SUFILE_H

#include <linux/fs.h>
#include <linux/buffer_head.h>
#include <linux/nilfs2_fs.h>
#include "mdt.h"


static inline unsigned long nilfs_sufile_get_nsegments(struct inode *sufile)
{
	return ((struct the_nilfs *)sufile->i_sb->s_fs_info)->ns_nsegments;
}

unsigned long nilfs_sufile_get_ncleansegs(struct inode *sufile);

int nilfs_sufile_set_alloc_range(struct inode *sufile, __u64 start, __u64 end);
int nilfs_sufile_alloc(struct inode *, __u64 *);
int nilfs_sufile_mark_dirty(struct inode *sufile, __u64 segnum);
int nilfs_sufile_set_segment_usage(struct inode *sufile, __u64 segnum,
				   unsigned long nblocks, time_t modtime);
int nilfs_sufile_get_stat(struct inode *, struct nilfs_sustat *);
ssize_t nilfs_sufile_get_suinfo(struct inode *, __u64, void *, unsigned,
				size_t);

int nilfs_sufile_updatev(struct inode *, __u64 *, size_t, int, size_t *,
			 void (*dofunc)(struct inode *, __u64,
					struct buffer_head *,
					struct buffer_head *));
int nilfs_sufile_update(struct inode *, __u64, int,
			void (*dofunc)(struct inode *, __u64,
				       struct buffer_head *,
				       struct buffer_head *));
void nilfs_sufile_do_scrap(struct inode *, __u64, struct buffer_head *,
			   struct buffer_head *);
void nilfs_sufile_do_free(struct inode *, __u64, struct buffer_head *,
			  struct buffer_head *);
void nilfs_sufile_do_cancel_free(struct inode *, __u64, struct buffer_head *,
				 struct buffer_head *);
void nilfs_sufile_do_set_error(struct inode *, __u64, struct buffer_head *,
			       struct buffer_head *);

int nilfs_sufile_resize(struct inode *sufile, __u64 newnsegs);
int nilfs_sufile_read(struct super_block *sb, size_t susize,
		      struct nilfs_inode *raw_inode, struct inode **inodep);

static inline int nilfs_sufile_scrap(struct inode *sufile, __u64 segnum)
{
	return nilfs_sufile_update(sufile, segnum, 1, nilfs_sufile_do_scrap);
}

static inline int nilfs_sufile_free(struct inode *sufile, __u64 segnum)
{
	return nilfs_sufile_update(sufile, segnum, 0, nilfs_sufile_do_free);
}

static inline int nilfs_sufile_freev(struct inode *sufile, __u64 *segnumv,
				     size_t nsegs, size_t *ndone)
{
	return nilfs_sufile_updatev(sufile, segnumv, nsegs, 0, ndone,
				    nilfs_sufile_do_free);
}

static inline int nilfs_sufile_cancel_freev(struct inode *sufile,
					    __u64 *segnumv, size_t nsegs,
					    size_t *ndone)
{
	return nilfs_sufile_updatev(sufile, segnumv, nsegs, 0, ndone,
				    nilfs_sufile_do_cancel_free);
}

static inline int nilfs_sufile_set_error(struct inode *sufile, __u64 segnum)
{
	return nilfs_sufile_update(sufile, segnum, 0,
				   nilfs_sufile_do_set_error);
}

#endif	
