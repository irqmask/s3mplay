/*
 * libs3m - S3M player routines for music support in you programs.
 * Copyright (C) 2017  christian <irqmask@web.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file s3m_file.c
 * Load a S3M file from disk.
 *
 * \author  CV (irqmask@web.de)
 * \date    2017-05-01
 */

/* Include section -----------------------------------------------------------*/
#include <assert.h>
#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "s3m.h"

/* Definitions ---------------------------------------------------------------*/

/* Type definitions ----------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Local variables -----------------------------------------------------------*/

/**
 * Load a S3M file from disk.
 * This function calls s3m_from_ram().
 */
int s3m_load(s3m_t* s3m, const char* filename)
{
    int retval = -1;
    struct stat st;
    FILE* s3m_file = NULL;
    size_t len = 0;
    
    assert (s3m != NULL);
    assert (filename != NULL);
  
    do {  
        if (stat(filename, &st) != 0) break;
        s3m->filesize = st.st_size;
        
        s3m->buffer = calloc(s3m->filesize, sizeof(uint8_t));
        if (s3m->buffer == NULL) break;       
        
        s3m_file = fopen(filename, "rb");
        if (s3m_file == NULL) {
            printf("fopen failed, errno=%d\n", errno);
            break;
        }
        
        len = fread(s3m->buffer, 1, s3m->filesize, s3m_file);
        if (len != s3m->filesize) {
            printf("fread failed. expected length = %zubut read %zu\n", s3m->filesize, len);
            break;      
        }
                
        retval = s3m_from_ram(s3m, s3m->buffer, s3m->filesize);
    } while (0);
    
    if (s3m_file != NULL) {
        fclose(s3m_file);
        if (retval < 0) {           
            if (s3m->buffer != NULL) free(s3m->buffer);
            s3m->buffer = NULL;
        }
    }
    return retval;
}

/**
 * Free the memory occupied from the S3M file. Call this function, if you not 
 * want to play the S3M anymore.
 */
void s3m_unload(s3m_t* s3m)
{
    assert (s3m != NULL);
    
    if (s3m->buffer != NULL) {
        free(s3m->buffer);
        s3m->buffer = NULL;
    }
}

/* EOF: s3m_file.c */
