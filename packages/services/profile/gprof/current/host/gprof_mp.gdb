# =====================================================================
#
#     gprof_mp.gdb
#
# =====================================================================
# ####ECOSHOSTGPLCOPYRIGHTBEGIN####                                         
# -------------------------------------------                               
# This file is part of the eCos host tools.                                 
# Copyright (C) 2011 Free Software Foundation, Inc.                         
#
# This program is free software; you can redistribute it and/or modify      
# it under the terms of the GNU General Public License as published by      
# the Free Software Foundation; either version 2 or (at your option) any    
# later version.                                                            
#
# This program is distributed in the hope that it will be useful, but       
# WITHOUT ANY WARRANTY; without even the implied warranty of                
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
# General Public License for more details.                                  
#
# You should have received a copy of the GNU General Public License         
# along with this program; if not, write to the                             
# Free Software Foundation, Inc., 51 Franklin Street,                       
# Fifth Floor, Boston, MA  02110-1301, USA.                                 
# -------------------------------------------                               
# ####ECOSHOSTGPLCOPYRIGHTEND####                                           
# =====================================================================
######DESCRIPTIONBEGIN####
#
# Author(s):    ccoutand
# Original(s):  bartv
# Date:         2011-12-15
# Purpose:      extract gprof data from SMP target
#
#####DESCRIPTIONEND####
# =====================================================================

define gprof_mp_fetch
  if $argc == 0
      echo Missing processor core ID\n
  else
    set $core_id = $arg0
    if 0 == profile_hist_data[$core_id]
      echo Target-side profiling has not been initialized.\n
    else
      if 0 != profile_reset_pending
        echo No profiling data has accumulated since the last reset.\n
      else
        if 0 == $core_id
          dump   binary memory gmon_core0.out &profile_gmon_hdr ((char*)&profile_gmon_hdr + sizeof(struct gmon_hdr))
          append binary memory gmon_core0.out &profile_tags[0] &profile_tags[1]
          append binary memory gmon_core0.out &profile_hist_hdr ((char*)&profile_hist_hdr + sizeof(struct gmon_hist_hdr))
          append binary memory gmon_core0.out profile_hist_data[$core_id] (profile_hist_data[0] + profile_hist_hdr.hist_size)
        end
        if 1 == $core_id
          dump   binary memory gmon_core1.out &profile_gmon_hdr ((char*)&profile_gmon_hdr + sizeof(struct gmon_hdr))
          append binary memory gmon_core1.out &profile_tags[0] &profile_tags[1]
          append binary memory gmon_core1.out &profile_hist_hdr ((char*)&profile_hist_hdr + sizeof(struct gmon_hist_hdr))
          append binary memory gmon_core1.out profile_hist_data[$core_id] (profile_hist_data[$core_id] + profile_hist_hdr.hist_size)
        end
        if 2 == $core_id
          dump   binary memory gmon_core2.out &profile_gmon_hdr ((char*)&profile_gmon_hdr + sizeof(struct gmon_hdr))
          append binary memory gmon_core2.out &profile_tags[0] &profile_tags[1]
          append binary memory gmon_core2.out &profile_hist_hdr ((char*)&profile_hist_hdr + sizeof(struct gmon_hist_hdr))
          append binary memory gmon_core2.out profile_hist_data[$core_id] (profile_hist_data[$core_id] + profile_hist_hdr.hist_size)
        end
        if 3 == $core_id
          dump   binary memory gmon_core3.out &profile_gmon_hdr ((char*)&profile_gmon_hdr + sizeof(struct gmon_hdr))
          append binary memory gmon_core3.out &profile_tags[0] &profile_tags[1]
          append binary memory gmon_core3.out &profile_hist_hdr ((char*)&profile_hist_hdr + sizeof(struct gmon_hist_hdr))
          append binary memory gmon_core3.out profile_hist_data[$core_id] (profile_hist_data[$core_id] + profile_hist_hdr.hist_size)
        end
        if 0 != profile_arc_records[$core_id]
          if 0 != profile_arc_overflow[$core_id]
            echo Warning: the table of callgraph arcs has overflowed\n
            echo   This can be avoided by increasing CYGNUM_PROFILE_CALLGRAPH_ARC_PERCENTAGE\n
          end
          # This loop is slow, but there is not much that can be done about it.
          # The gmon.out file should contain 13 bytes per arc record. Keeping
          # all the data in a packed array would involve non-aligned data,
          # which would add significantly to the run-time overheads.
          set $profile_i = 1
          set $profile_count = profile_arc_next[$core_id]
          while $profile_i != $profile_count
            if 0 == $core_id
              append binary memory gmon_core0.out &(profile_arc_records[$core_id][$profile_i].tags[3]) &(profile_arc_records[$core_id][$profile_i + 1])
            end
            if 1 == $core_id
              append binary memory gmon_core1.out &(profile_arc_records[$core_id][$profile_i].tags[3]) &(profile_arc_records[$core_id][$profile_i + 1])
            end
            if 2 == $core_id
              append binary memory gmon_core2.out &(profile_arc_records[$core_id][$profile_i].tags[3]) &(profile_arc_records[$core_id][$profile_i + 1])
            end
            if 3 == $core_id
              append binary memory gmon_core3.out &(profile_arc_records[$core_id][$profile_i].tags[3]) &(profile_arc_records[$core_id][$profile_i + 1])
            end
            set $profile_i = $profile_i + 1
          end
        end
      end
    end
  end
end

document gprof_mp_fetch
Extract gprof profiling data from a multi-processor target. The first argument
is the processor ID. Check gprof_fetch for more details for single core 
processor.
end